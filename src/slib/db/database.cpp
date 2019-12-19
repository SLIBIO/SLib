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

#include "slib/db/database.h"

#include "slib/core/string_buffer.h"
#include "slib/core/log.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Database, Object)

	Database::Database()
	{
		m_flagLogSQL = sl_false;
		m_flagLogErrors = sl_true;
		m_dialect = DatabaseDialect::Generic;
	}

	Database::~Database()
	{
	}
	
	sl_int64 Database::_executeBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->executeBy(params, nParams);
		}
		return -1;
	}

	sl_int64 Database::_execute(const StringParam& sql)
	{
		return _executeBy(sql, sl_null, 0);
	}

	Ref<DatabaseCursor> Database::_queryBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->queryBy(params, nParams);
		}
		return sl_null;
	}

	Ref<DatabaseCursor> Database::_query(const StringParam& sql)
	{
		return _queryBy(sql, sl_null, 0);
	}

	Ref<DatabaseStatement> Database::prepareStatement(const StringParam& sql)
	{
		Ref<DatabaseStatement> ret = _prepareStatement(sql);
		if (ret.isNotNull()) {
			_logSQL(sql);
		} else {
			_logError(sql);
		}
		return ret;
	}

	Ref<DatabaseStatement> Database::prepareStatement(const SqlBuilder& builder)
	{
		Ref<DatabaseStatement> ret = prepareStatement(builder.toString());
		if (ret.isNotNull()) {
			ret->setParameterNames(builder.parameters);
			return ret;
		}
		return sl_null;
	}

	sl_int64 Database::executeBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		sl_int64 ret = _executeBy(sql, params, nParams);
		if (ret < 0) {
			_logError(sql, params, nParams);
		} else {
			_logSQL(sql, params, nParams);
		}
		return ret;
	}

	sl_int64 Database::execute(const StringParam& sql)
	{
		sl_int64 ret = _execute(sql);
		if (ret < 0) {
			_logError(sql);
		} else {
			_logSQL(sql);
		}
		return ret;
	}
	
	Ref<DatabaseCursor> Database::queryBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> ret = _queryBy(sql, params, nParams);
		if (ret.isNull()) {
			_logError(sql, params, nParams);
		} else {
			_logSQL(sql, params, nParams);
		}
		return ret;
	}

	Ref<DatabaseCursor> Database::query(const StringParam& sql)
	{
		Ref<DatabaseCursor> ret = _query(sql);
		if (ret.isNull()) {
			_logError(sql);
		} else {
			_logSQL(sql);
		}
		return ret;
	}
	
	List< HashMap<String, Variant> > Database::getRecordsBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(sql, params, nParams);
		if (cursor.isNotNull()) {
			List< HashMap<String, Variant> > ret;
			while (cursor->moveNext()) {
				ret.add_NoLock(cursor->getRow());
			}
			return ret;
		}
		return sl_null;
	}

	List< HashMap<String, Variant> > Database::getRecords(const StringParam& sql)
	{
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			List< HashMap<String, Variant> > ret;
			while (cursor->moveNext()) {
				ret.add_NoLock(cursor->getRow());
			}
			return ret;
		}
		return sl_null;
	}
	
	HashMap<String, Variant> Database::getRecordBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(sql, params, nParams);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getRow();
			}
		}
		return sl_null;
	}

	HashMap<String, Variant> Database::getRecord(const StringParam& sql)
	{
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getRow();
			}
		}
		return sl_null;
	}
	
	Variant Database::getValueBy(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(sql, params, nParams);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getValue(0);
			}
		}
		return sl_null;
	}

	Variant Database::getValue(const StringParam& sql)
	{
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getValue(0);
			}
		}
		return sl_null;
	}
	
	sl_bool Database::isLoggingSQL()
	{
		return m_flagLogSQL;
	}
	
	void Database::setLoggingSQL(sl_bool flag)
	{
		m_flagLogSQL = flag;
	}
	
	sl_bool Database::isLoggingErrors()
	{
		return m_flagLogErrors;
	}
	
	void Database::setLoggingErrors(sl_bool flag)
	{
		m_flagLogErrors = flag;
	}

	DatabaseDialect Database::getDialect()
	{
		return m_dialect;
	}

	sl_bool Database::createTable(const DatabaseCreateTableParam& param)
	{
		SqlBuilder builder(m_dialect);
		builder.generateCreateTable(param);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_false;
		}
		return execute(sql) >= 0;
	}

	sl_bool Database::createTable(const DatabaseIdentifier& table, const ListParam<DatabaseColumnDefinition>& columns, DatabaseFlags flags)
	{
		DatabaseCreateTableParam param;
		param.table = table;
		param.columns = columns;
		param.flags = flags;
		return createTable(param);
	}

	sl_bool Database::dropTable(const DatabaseIdentifier& table, DatabaseFlags flags)
	{
		SqlBuilder builder(m_dialect);
		builder.generateDropTable(table, flags);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_false;
		}
		return execute(sql) >= 0;
	}

	sl_bool Database::createIndex(const DatabaseCreateIndexParam& param)
	{
		SqlBuilder builder(m_dialect);
		builder.generateCreateIndex(param);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_false;
		}
		return execute(sql) >= 0;
	}

	sl_bool Database::createIndex(const DatabaseIdentifier& index, const String& table, const ListParam<DatabaseIndexColumn>& columns, DatabaseFlags flags)
	{
		DatabaseCreateIndexParam param;
		param.index = index;
		param.table = table;
		param.columns = columns;
		param.flags = flags;
		return createIndex(param);
	}

	sl_bool Database::dropIndex(const DatabaseIdentifier& index, const String& table, DatabaseFlags flags)
	{
		SqlBuilder builder(m_dialect);
		builder.generateDropIndex(index, table, flags);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_false;
		}
		return execute(sql) >= 0;
	}

	Ref<DatabaseStatement> Database::prepareInsert(const DatabaseIdentifier& table, const ListParam<String>& columns)
	{
		SqlBuilder builder(m_dialect);
		builder.generateInsert(table, columns);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_null;
		}
		return prepareStatement(sql);
	}

	Ref<DatabaseStatement> Database::prepareUpdate(const DatabaseIdentifier& table, const ListParam<String>& columns, const DatabaseExpression& where)
	{
		SqlBuilder builder(m_dialect);
		builder.generateUpdate(table, columns, where);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_null;
		}
		return prepareStatement(sql);
	}
	
	Ref<DatabaseStatement> Database::prepareDelete(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		SqlBuilder builder(m_dialect);
		builder.generateDelete(table, where);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_null;
		}
		return prepareStatement(sql);
	}
	
	Ref<DatabaseStatement> Database::prepareQuery(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		SqlBuilder builder(m_dialect);
		builder.generateSelect(table, where);
		String sql = builder.toString();
		if (sql.isEmpty()) {
			return sl_null;
		}
		return prepareStatement(sql);
	}

	sl_bool Database::startTransaction()
	{
		SLIB_STATIC_STRING(s, "BEGIN")
		return execute(s) >= 0;
	}

	sl_bool Database::commitTransaction()
	{
		SLIB_STATIC_STRING(s, "COMMIT")
		return execute(s) >= 0;
	}

	sl_bool Database::rollbackTransaction()
	{
		SLIB_STATIC_STRING(s, "ROLLBACK")
		return execute(s) >= 0;
	}

	void Database::_logSQL(const StringParam& sql)
	{
		if (m_flagLogSQL) {
			Log((char*)(getObjectType()), "SQL: %s", sql);
		}
	}
	
	void Database::_logSQL(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		if (m_flagLogSQL) {
			Log((char*)(getObjectType()), "SQL: %s Params=%s", sql, Variant(VariantList(params, nParams)).toJsonString());
		}
	}
	
	void Database::_logError(const StringParam& sql)
	{
		if (m_flagLogErrors) {
			LogError((char*)(getObjectType()), "Error: %s SQL: %s", getErrorMessage(), sql);
		}
	}

	void Database::_logError(const StringParam& sql, const Variant* params, sl_uint32 nParams)
	{
		if (m_flagLogErrors) {
			LogError((char*)(getObjectType()), "Error: %s SQL: %s Params=%s", getErrorMessage(), sql, Variant(VariantList(params, nParams)).toJsonString());
		}
	}

}
