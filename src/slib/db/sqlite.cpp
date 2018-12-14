/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

#include "sqlite/sqlite3.h"

#include "slib/db/sqlite.h"

#include "slib/core/file.h"
#include "slib/core/log.h"

#define TAG "SQLiteDatabase"

namespace slib
{	

	SLIB_DEFINE_OBJECT(SQLiteDatabase, Database)

	SQLiteDatabase::SQLiteDatabase()
	{
	}

	SQLiteDatabase::~SQLiteDatabase()
	{
	}

	class _priv_Sqlite3Database : public SQLiteDatabase
	{
	public:
		sqlite3* m_db;

		_priv_Sqlite3Database()
		{
			m_db = sl_null;
		}

		~_priv_Sqlite3Database()
		{
			::sqlite3_close(m_db);
		}

		static Ref<_priv_Sqlite3Database> connect(const String& filePath, sl_bool flagCreate, sl_bool flagReadonly)
		{
			Ref<_priv_Sqlite3Database> ret;
			sqlite3* db = sl_null;
			int flags;
			if (flagCreate) {
				flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
			} else {
				flags = flagReadonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE;
			}
			sl_int32 iResult = ::sqlite3_open_v2(filePath.getData(), &db, flags, sl_null);
			if (SQLITE_OK == iResult) {
				ret = new _priv_Sqlite3Database();
				if (ret.isNotNull()) {
					ret->m_db = db;
					return ret;
				}
				::sqlite3_close(db);
			}
			return ret;
		}

		sl_int64 _execute(const String& sql) override
		{
			ObjectLocker lock(this);
			if (SQLITE_OK == ::sqlite3_exec(m_db, sql.getData(), 0, 0, sl_null)) {
				return ::sqlite3_changes(m_db);
			}
			return -1;
		}

		static HashMap<String, Variant> _getRow(sqlite3_stmt* stmt, String* columns, sl_uint32 nColumns)
		{
			HashMap<String, Variant> row;
			for (sl_uint32 cIndex = 0; cIndex < nColumns; cIndex++) {
				String cValue = String::fromUtf8(::sqlite3_column_text(stmt, (int)cIndex));
				row.put_NoLock(columns[cIndex], cValue);
			}
			return row;
		}

		class _priv_DatabaseCursor : public DatabaseCursor
		{
		public:
			Ref<DatabaseStatement> m_statementObj;
			sqlite3_stmt* m_statement;

			CList<String> m_listColumnNames;
			sl_uint32 m_nColumnNames;
			String* m_columnNames;
			CHashMap<String, sl_int32> m_mapColumnIndexes;

			_priv_DatabaseCursor(Database* db, DatabaseStatement* statementObj, sqlite3_stmt* statement)
			{
				m_db = db;
				m_statementObj = statementObj;
				m_statement = statement;

				sl_int32 cols = ::sqlite3_column_count(statement);
				for (sl_int32 i = 0; i < cols; i++) {
					const char* buf = ::sqlite3_column_name(statement, (int)i);
					String name(buf);
					m_listColumnNames.add_NoLock(name);
					m_mapColumnIndexes.put_NoLock(name, i);
				}
				m_nColumnNames = (sl_uint32)(m_listColumnNames.getCount());
				m_columnNames = m_listColumnNames.getData();

				db->lock();
			}

			~_priv_DatabaseCursor()
			{
				::sqlite3_reset(m_statement);
				::sqlite3_clear_bindings(m_statement);
				m_db->unlock();
			}

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

			sl_int32 getColumnIndex(const String& name) override
			{
				return m_mapColumnIndexes.getValue_NoLock(name, -1);
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

			String _getString(sl_uint32 index)
			{
				int n = sqlite3_column_bytes(m_statement, index);
				const void* buf = sqlite3_column_text(m_statement, index);
				if (buf && n > 0) {
					return String::fromUtf8(buf, n);
				}
				return String::getEmpty();
			}

			Memory _getBlob(sl_uint32 index)
			{
				int n = sqlite3_column_bytes(m_statement, index);
				const void* buf = sqlite3_column_blob(m_statement, index);
				if (buf && n > 0) {
					return Memory::create(buf, n);
				}
				return sl_null;
			}

			Variant _getValue(sl_uint32 index)
			{
				int type = ::sqlite3_column_type(m_statement, index);
				switch (type) {
				case SQLITE_INTEGER:
					{
						sl_int64 v64 = sqlite3_column_int64(m_statement, index);
						sl_int32 v32 = (sl_int32)v64;
						if (v64 == v32) {
							return v32;
						} else {
							return v64;
						}
					}
					break;
				case SQLITE_FLOAT:
					return sqlite3_column_double(m_statement, index);
				case SQLITE_TEXT:
					return _getString(index);
				case SQLITE_BLOB:
					return _getBlob(index);
				}
				return sl_null;
			}

			Variant getValue(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					return _getValue(index);
				}
				return sl_null;
			}

			String getString(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return String::fromInt64(sqlite3_column_int64(m_statement, index));
					case SQLITE_FLOAT:
						return String::fromDouble(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index);
					}
				}
				return sl_null;
			}

			sl_int64 getInt64(sl_uint32 index, sl_int64 defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return sqlite3_column_int64(m_statement, index);
					case SQLITE_FLOAT:
						return (sl_int64)(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseInt64(10, defaultValue);
					}
				}
				return defaultValue;
			}

			sl_uint64 getUint64(sl_uint32 index, sl_uint64 defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return sqlite3_column_int64(m_statement, index);
					case SQLITE_FLOAT:
						return (sl_uint64)(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseUint64(10, defaultValue);
					}
				}
				return defaultValue;
			}

			sl_int32 getInt32(sl_uint32 index, sl_int32 defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return sqlite3_column_int(m_statement, index);
					case SQLITE_FLOAT:
						return (sl_int32)(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseInt32(10, defaultValue);
					}
				}
				return defaultValue;
			}

			sl_uint32 getUint32(sl_uint32 index, sl_uint32 defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return (sl_uint32)(sqlite3_column_int64(m_statement, index));
					case SQLITE_FLOAT:
						return (sl_uint32)(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseUint32(10, defaultValue);
					}
				}
				return defaultValue;
			}

			float getFloat(sl_uint32 index, float defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return (float)(sqlite3_column_int64(m_statement, index));
					case SQLITE_FLOAT:
						return (float)(sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseFloat(defaultValue);
					}
				}
				return defaultValue;
			}

			double getDouble(sl_uint32 index, double defaultValue) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						return (double)(sqlite3_column_int64(m_statement, index));
					case SQLITE_FLOAT:
						return (sqlite3_column_double(m_statement, index));
					case SQLITE_TEXT:
						return _getString(index).parseDouble(defaultValue);
					}
				}
				return defaultValue;
			}

			Memory getBlob(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					int type = ::sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_TEXT:
					case SQLITE_BLOB:
						return _getBlob(index);
					}
				}
				return sl_null;
			}

			sl_bool moveNext() override
			{
				sl_int32 nRet = ::sqlite3_step(m_statement);
				if (nRet == SQLITE_ROW) {
					return sl_true;
				}
				return sl_false;
			}

		};

		class _priv_DatabaseStatement : public DatabaseStatement
		{
		public:
			sqlite3* m_sqlite;
			sqlite3_stmt* m_statement;
			Array<Variant> m_boundParams;

			_priv_DatabaseStatement(_priv_Sqlite3Database* db, sqlite3_stmt* statement)
			{
				m_db = db;
				m_sqlite = db->m_db;
				m_statement = statement;
			}

			~_priv_DatabaseStatement()
			{
				::sqlite3_finalize(m_statement);
			}
			
			sl_bool isLoggingErrors()
			{
				if (m_db.isNotNull()) {
					return m_db->isLoggingErrors();
				}
				return sl_false;
			}
			
			sl_bool _execute(const Variant* _params, sl_uint32 nParams)
			{
				::sqlite3_reset(m_statement);
				::sqlite3_clear_bindings(m_statement);
				m_boundParams.setNull();
				
				if (nParams == 0) {
					return sl_true;
				}

				Array<Variant> params = Array<Variant>::create(_params, nParams);
				if (params.isNull()) {
					return sl_false;
				}
				sl_uint32 n = (sl_uint32)(::sqlite3_bind_parameter_count(m_statement));
				if (n == nParams) {
					if (n > 0) {
						for (sl_uint32 i = 0; i < n; i++) {
							int iRet = SQLITE_ABORT;
							Variant& var = (params.getData())[i];
							switch (var.getType()) {
							case VariantType::Null:
								iRet = ::sqlite3_bind_null(m_statement, i+1);
								break;
							case VariantType::Boolean:
							case VariantType::Int32:
								iRet = ::sqlite3_bind_int(m_statement, i+1, var.getInt32());
								break;
							case VariantType::Uint32:
							case VariantType::Int64:
							case VariantType::Uint64:
								iRet = ::sqlite3_bind_int64(m_statement, i+1, var.getInt64());
								break;
							case VariantType::Float:
							case VariantType::Double:
								iRet = ::sqlite3_bind_double(m_statement, i+1, var.getDouble());
								break;
							default:
								if (var.isMemory()) {
									Memory mem = var.getMemory();
									sl_size size = mem.getSize();
									if (size > 0x7fffffff) {
										iRet = ::sqlite3_bind_blob64(m_statement, i+1, mem.getData(), size, SQLITE_STATIC);
									} else {
										iRet = ::sqlite3_bind_blob(m_statement, i+1, mem.getData(), (sl_uint32)size, SQLITE_STATIC);
									}
								} else {
									String str = var.getString();
									var = str;
									iRet = ::sqlite3_bind_text(m_statement, i+1, str.getData(), (sl_uint32)(str.getLength()), SQLITE_STATIC);
								}
							}
							if (iRet != SQLITE_OK) {
								return sl_false;
							}
						}
					}
					m_boundParams = params;
					return sl_true;
				} else {
					if (isLoggingErrors()) {
						LogError(TAG, "Bind error: requires %d params but %d params provided", n, nParams);
					}
				}
				return sl_false;
			}

			sl_int64 executeBy(const Variant* params, sl_uint32 nParams) override
			{
				ObjectLocker lock(m_db.get());
				if (_execute(params, nParams)) {
					if (::sqlite3_step(m_statement) == SQLITE_DONE) {
						::sqlite3_reset(m_statement);
						::sqlite3_clear_bindings(m_statement);
						return ::sqlite3_changes(m_sqlite);
					}
				}
				return -1;
			}

			Ref<DatabaseCursor> queryBy(const Variant* params, sl_uint32 nParams) override
			{
				ObjectLocker lock(m_db.get());
				Ref<DatabaseCursor> ret;
				if (_execute(params, nParams)) {
					ret = new _priv_DatabaseCursor(m_db.get(), this, m_statement);
					if (ret.isNotNull()) {
						return ret;
					}
					::sqlite3_reset(m_statement);
					::sqlite3_clear_bindings(m_statement);
				}
				return ret;
			}
		};

		Ref<DatabaseStatement> prepareStatement(const String& sql) override
		{
			ObjectLocker lock(this);
			Ref<DatabaseStatement> ret;
			sqlite3_stmt* statement = sl_null;
			if (SQLITE_OK == ::sqlite3_prepare_v2(m_db, sql.getData(), -1, &statement, sl_null)) {
				ret = new _priv_DatabaseStatement(this, statement);
				if (ret.isNotNull()) {
					return ret;
				}
				::sqlite3_finalize(statement);
			}
			return ret;
		}

		String getErrorMessage() override
		{
			String error = ::sqlite3_errmsg(m_db);
			if (error.isEmpty() || error == "not an error") {
				return sl_null;
			}
			return error;
		}
	};
	
	Ref<SQLiteDatabase> SQLiteDatabase::connect(const String& path, sl_bool flagCreate, sl_bool flagReadonly)
	{
		return _priv_Sqlite3Database::connect(path, flagCreate, flagReadonly);
	}

}
