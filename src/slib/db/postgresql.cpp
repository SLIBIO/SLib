/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/db/postgresql.h"

#if defined(SLIB_DATABASE_SUPPORT_POSTGRESQL)

#include "slib/core/mio.h"
#include "slib/core/math.h"
#include "slib/core/log.h"
#include "slib/core/scoped.h"
#include "slib/core/safe_static.h"

extern "C"
{
	#include "libpq/libpq-fe.h"
}

#define TAG "PostgreSQL"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PostgreSqlParam)

	PostgreSqlParam::PostgreSqlParam()
	{
		port = 0;
	}

	SLIB_DEFINE_OBJECT(PostgreSQL, Database)

	PostgreSQL::PostgreSQL()
	{
		m_dialect = DatabaseDialect::PostgreSQL;
	}

	PostgreSQL::~PostgreSQL()
	{
	}

	namespace priv
	{
		namespace postgresql
		{
		
			class CursorImpl : public DatabaseCursor
			{
			public:
				PGconn* m_connection;
				PGresult* m_result;
				sl_bool m_flagFirstRow;
				
				CList<String> m_listColumnNames;
				sl_uint32 m_nColumnNames;
				String* m_columnNames;

			public:
				CursorImpl(Database* db, PGconn* connection)
				{
					m_db = db;
					m_connection = connection;

					db->lock();
					
					PQsetSingleRowMode(m_connection);
					
					m_flagFirstRow = sl_true;
					for (;;) {
						m_result = PQgetResult(m_connection);
						if (!m_result) {
							break;
						}
						if (PQresultStatus(m_result) == PGRES_SINGLE_TUPLE) {
							break;
						}
					}
					
					if (m_result) {
						int cols = PQnfields(m_result);
						for (int i = 0; i < cols; i++) {
							m_listColumnNames.add_NoLock(PQfname(m_result, i));
						}
						m_nColumnNames = (sl_uint32)(m_listColumnNames.getCount());
						m_columnNames = m_listColumnNames.getData();
					} else {
						m_nColumnNames = 0;
						m_columnNames = sl_null;
					}
				}

				~CursorImpl()
				{
					if (m_result) {
						PQclear(m_result);
					}
					m_db->unlock();
				}

			public:
				sl_uint32 getColumnsCount() override
				{
					return m_nColumnNames;
				}

				String getColumnName(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						return m_columnNames[index];
					}
					return sl_null;
				}

				sl_int32 getColumnIndex(const StringParam& _name) override
				{
					if (!m_result) {
						return 0;
					}
					StringData name(_name);
					return (sl_int32)(PQfnumber(m_result, name.data));
				}

				HashMap<String, Variant> getRow() override
				{
					HashMap<String, Variant> ret;
					if (m_nColumnNames > 0) {
						for (sl_uint32 index = 0; index < m_nColumnNames; index++) {
							ret.put_NoLock(m_columnNames[index], _getValue(index));
						}
					}
					return ret;
				}

				Variant _getValue(sl_uint32 _index)
				{
					PGresult* result = m_result;
					if (!result) {
						return sl_null;
					}
					int index = (int)_index;
					if (PQgetisnull(result, 0, index)) {
						return sl_null;
					}
					char* v = PQgetvalue(result, 0, index);
					int len = PQgetlength(result, 0, index);
					if (PQfformat(result, index) == 0) { // text format
						if (len >= 2 && v[0] == '\\' && v[1] == 'x') {
							Oid t = PQftype(result, index);
							if (t == 17) { // bytea
								sl_uint32 n = (sl_uint32)((len - 2) >> 1);
								Memory mem = Memory::create(n);
								if (mem.isNotNull()) {
									if (SLIB_PARSE_ERROR != String::parseHexString(mem.getData(), v, 2, len)) {
										return mem;
									}
								}
							}
						}
						return String::create(v, len);
					} else {
						return Memory::create(v, len);
					}
				}
				
				Variant getValue(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						return _getValue(index);
					}
					return sl_null;
				}

				sl_bool moveNext() override
				{
					if (m_flagFirstRow) {
						m_flagFirstRow = sl_false;
						return m_result != sl_null;
					}
					if (m_result) {
						PQclear(m_result);
						for (;;) {
							m_result = PQgetResult(m_connection);
							if (!m_result) {
								return sl_false;
							}
							if (PQresultStatus(m_result) == PGRES_SINGLE_TUPLE) {
								break;
							}
						}
						return sl_true;
					}
					return sl_false;
				}
				
			};
		
			static void BindParams(const Variant* params, sl_uint32 nParams, String* strings, const char** values, int* lengths, int* formats)
			{
				for (sl_uint32 i = 0; i < nParams; i++) {
					if (params[i].isNull()) {
						values[i] = sl_null;
						lengths[i] = 0;
					} else {
						if (params[i].isMemory()) {
							Memory mem = params[i].getMemory();
							values[i] = (char*)(mem.getData());
							lengths[i] = (int)(mem.getSize());
						} if (params[i].isSz8()) {
							values[i] = params[i].getSz8();
							lengths[i] = (int)(Base::getStringLength(values[i]));
						} else {
							strings[i] = params[i].getString();
							values[i] = strings[i].getData();
							lengths[i] = (int)(strings[i].getLength());
						}
					}
					formats[i] = 0;
				}
			}
			
			class StatementImpl : public DatabaseStatement
			{
			public:
				PGconn* m_connection;
				String m_sql;
				String m_name;
				
			public:
				StatementImpl(Database* db, PGconn* connection, const String& sql)
				{
					m_db = db;
					m_connection = connection;
					m_sql = sql;
					char t[16];
					Math::randomMemory(t, 16);
					String name = "slib_temp_stmt_" + String::makeHexString(t, 16);
					PGresult* res = PQprepare(connection, name.getData(), sql.getData(), 0, sl_null);
					if (res) {
						if (PQresultStatus(res) == PGRES_COMMAND_OK) {
							m_name = name;
						}
						PQclear(res);
					}
				}

				~StatementImpl()
				{
					if (m_name.isNotEmpty()) {
						String sql = "DEALLOCATE " + m_name;
						PGresult* res = PQexec(m_connection, sql.getData());
						if (res) {
							PQclear(res);
						}
					}
				}
				
			public:
				sl_bool isLoggingErrors()
				{
					if (m_db.isNotNull()) {
						return m_db->isLoggingErrors();
					}
					return sl_false;
				}
				
				sl_int64 executeBy(const Variant* params, sl_uint32 nParams) override
				{
					SLIB_SCOPED_BUFFER(String, 32, strings, nParams)
					SLIB_SCOPED_BUFFER(const char*, 32, values, nParams)
					SLIB_SCOPED_BUFFER(int, 32, lengths, nParams)
					SLIB_SCOPED_BUFFER(int, 32, formats, nParams)
					BindParams(params, nParams, strings, values, lengths, formats);
					
					ObjectLocker lock(m_db.get());
					PGresult* res = PQexecPrepared(m_connection, m_name.getData(), (int)nParams, values, lengths, formats, 0);
					sl_int64 ret = -1;
					if (res) {
						if (PQresultStatus(res) == PGRES_COMMAND_OK) {
							char* s = PQcmdTuples(res);
							sl_uint64 n = 0;
							if (s) {
								String::parseUint64(10, &n, s);
							}
							ret = n;
						}
						PQclear(res);
					}
					return ret;
				}

				Ref<DatabaseCursor> queryBy(const Variant* params, sl_uint32 nParams) override
				{
					SLIB_SCOPED_BUFFER(String, 32, strings, nParams)
					SLIB_SCOPED_BUFFER(const char*, 32, values, nParams)
					SLIB_SCOPED_BUFFER(int, 32, lengths, nParams)
					SLIB_SCOPED_BUFFER(int, 32, formats, nParams)
					BindParams(params, nParams, strings, values, lengths, formats);
					
					ObjectLocker lock(m_db.get());
					if (PQsendQueryPrepared(m_connection, m_name.getData(), (int)nParams, values, lengths, formats, 0) == 1) {
						return new CursorImpl(m_db.get(), m_connection);
					}
					return sl_null;
				}

			};

			class DatabaseImpl : public PostgreSQL
			{
			public:
				PGconn* m_connection;
				
			public:
				DatabaseImpl()
				{
					m_connection = sl_null;
				}
				
				~DatabaseImpl()
				{
					if (m_connection) {
						PQfinish(m_connection);
					}
				}
				
			public:
				static Ref<DatabaseImpl> connect(PostgreSqlParam& param)
				{
					String port = String::fromUint32(param.port);
					PGconn* conn = PQsetdbLogin(param.host.getData(), param.port ? port.getData() : sl_null, sl_null, sl_null, param.db.getData(), param.user.getData(), param.password.getData());
					if (PQstatus(conn) != CONNECTION_OK) {
						char* err = PQerrorMessage(conn);
						LogError(TAG, "Connection to database failed: %s", err);
						param.error = err;
						return sl_null;
					}
					Ref<DatabaseImpl> ret = new DatabaseImpl;
					if (ret.isNotNull()) {
						ret->m_connection = conn;
						return ret;
					}
					return sl_null;
				}
				
				sl_int64 _execute(const StringParam& _sql) override
				{
					StringData sql(_sql);
					ObjectLocker lock(this);
					PGresult* res = PQexec(m_connection, sql.data);
					sl_int64 ret = -1;
					if (res) {
						if (PQresultStatus(res) == PGRES_COMMAND_OK) {
							char* s = PQcmdTuples(res);
							sl_uint64 n = 0;
							if (s) {
								String::parseUint64(10, &n, s);
							}
							ret = n;
						}
						PQclear(res);
					}
					return ret;
				}

				Ref<DatabaseCursor> _query(const StringParam& _sql) override
				{
					StringData sql(_sql);
					ObjectLocker lock(this);
					if (PQsendQuery(m_connection, sql.data) == 1) {
						return new CursorImpl(this, m_connection);
					}
					return sl_null;
				}
				
				sl_int64 _executeBy(const StringParam& _sql, const Variant* params, sl_uint32 nParams) override
				{
					StringData sql(_sql);

					SLIB_SCOPED_BUFFER(String, 32, strings, nParams)
					SLIB_SCOPED_BUFFER(const char*, 32, values, nParams)
					SLIB_SCOPED_BUFFER(int, 32, lengths, nParams)
					SLIB_SCOPED_BUFFER(int, 32, formats, nParams)
					BindParams(params, nParams, strings, values, lengths, formats);
					
					ObjectLocker lock(this);
					PGresult* res = PQexecParams(m_connection, sql.getData(), (int)nParams, sl_null, values, lengths, formats, 0);
					sl_int64 ret = -1;
					if (res) {
						if (PQresultStatus(res) == PGRES_COMMAND_OK) {
							char* s = PQcmdTuples(res);
							sl_uint64 n = 0;
							if (s) {
								String::parseUint64(10, &n, s);
							}
							ret = n;
						}
						PQclear(res);
					}
					return ret;
				}

				Ref<DatabaseCursor> _queryBy(const StringParam& _sql, const Variant* params, sl_uint32 nParams) override
				{
					StringData sql(_sql);

					SLIB_SCOPED_BUFFER(String, 32, strings, nParams)
					SLIB_SCOPED_BUFFER(const char*, 32, values, nParams)
					SLIB_SCOPED_BUFFER(int, 32, lengths, nParams)
					SLIB_SCOPED_BUFFER(int, 32, formats, nParams)
					BindParams(params, nParams, strings, values, lengths, formats);
					
					ObjectLocker lock(this);
					if (PQsendQueryParams(m_connection, sql.data, (int)nParams, sl_null, values, lengths, formats, 0) == 1) {
						return new CursorImpl(this, m_connection);
					}
					return sl_null;
				}
				
				Ref<DatabaseStatement> _prepareStatement(const StringParam& sql) override
				{
					ObjectLocker lock(this);
					Ref<StatementImpl> ret = new StatementImpl(this, m_connection, sql);
					if (ret.isNotNull()) {
						if (ret->m_name.isNotEmpty()) {
							return ret;
						}
					}
					return sl_null;
				}

				String getErrorMessage() override
				{
					String error = PQerrorMessage(m_connection);
					if (error.isEmpty()) {
						return sl_null;
					}
					return error;
				}
				
				sl_bool isDatabaseExisting(const StringParam& name) override
				{
					return getDatabases().contains(name);
				}
				
				List<String> getDatabases() override
				{
					PGresult* res = PQexec(m_connection, "SELECT datname FROM pg_database");
					if (res) {
						if (PQresultStatus(res) == PGRES_TUPLES_OK) {
							List<String> ret;
							int n = PQntuples(res);
							for (int i = 0; i < n; i++) {
								ret.add_NoLock(PQgetvalue(res, i, 0));
							}
							return ret;
						}
						PQclear(res);
					}
					return sl_null;
				}

				sl_bool isTableExisting(const StringParam& name) override
				{
					return getTables().contains(name);
				}
				
				List<String> getTables() override
				{
					PGresult* res = PQexec(m_connection, "SELECT tablename FROM pg_tables WHERE schemaname='public'");
					if (res) {
						if (PQresultStatus(res) == PGRES_TUPLES_OK) {
							List<String> ret;
							int n = PQntuples(res);
							for (int i = 0; i < n; i++) {
								ret.add_NoLock(PQgetvalue(res, i, 0));
							}
							return ret;
						}
						PQclear(res);
					}
					return sl_null;
				}
				
				sl_uint64 getLastInsertRowId() override
				{
					PGresult* res = PQexec(m_connection, "SELECT lastval()");
					if (res) {
						if (PQresultStatus(res) == PGRES_TUPLES_OK) {
							int n = PQntuples(res);
							if (n > 0) {
								sl_uint64 r = 0;
								String::parseUint64(10, &r, PQgetvalue(res, 0, 0));
								return r;
							}
						}
						PQclear(res);
					}
					return 0;
				}
								
			};
		
		}
	}

	using namespace priv::postgresql;

	Ref<PostgreSQL> PostgreSQL::connect(PostgreSqlParam& param)
	{
		return DatabaseImpl::connect(param);
	}

}

#endif
