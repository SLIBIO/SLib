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

#include "slib/core/log.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Database, Object)

	Database::Database()
	{
		m_flagLogSQL = sl_false;
		m_flagLogErrors = sl_true;
	}

	Database::~Database()
	{
	}
	
	sl_int64 Database::_execute(const String& sql)
	{
		return executeBy(sql, sl_null, 0);
	}

	sl_int64 Database::execute(const String& sql)
	{
		sl_int64 ret = _execute(sql);
		if (ret < 0) {
			_logError(sql);
		} else {
			_logSQL(sql);
		}
		return ret;
	}
	
	Ref<DatabaseCursor> Database::_query(const String& sql)
	{
		return queryBy(sql, sl_null, 0);
	}
	
	Ref<DatabaseCursor> Database::query(const String& sql)
	{
		Ref<DatabaseCursor> ret = queryBy(sql, sl_null, 0);
		if (ret.isNull()) {
			_logError(sql);
		} else {
			_logSQL(sql);
		}
		return ret;
	}
	
	sl_int64 Database::_executeBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->executeBy(params, nParams);
		}
		return -1;
	}

	sl_int64 Database::executeBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		sl_int64 ret = _executeBy(sql, params, nParams);
		if (ret < 0) {
			_logError(sql, params, nParams);
		} else {
			_logSQL(sql, params, nParams);
		}
		return ret;
	}
	
	Ref<DatabaseCursor> Database::_queryBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->queryBy(params, nParams);
		}
		return sl_null;
	}

	Ref<DatabaseCursor> Database::queryBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> ret = _queryBy(sql, params, nParams);
		if (ret.isNull()) {
			_logError(sql, params, nParams);
		} else {
			_logSQL(sql, params, nParams);
		}
		return ret;
	}

	List< HashMap<String, Variant> > Database::getListForQueryResult(const String& sql)
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
	
	HashMap<String, Variant> Database::getRecordForQueryResult(const String& sql)
	{
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getRow();
			}
		}
		return sl_null;
	}
	
	Variant Database::getValueForQueryResult(const String& sql)
	{
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getValue(0);
			}
		}
		return sl_null;
	}
	
	List< HashMap<String, Variant> > Database::getListForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
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

	HashMap<String, Variant> Database::getRecordForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(sql, params, nParams);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getRow();
			}
		}
		return sl_null;
	}

	Variant Database::getValueForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(sql, params, nParams);
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
	
	void Database::_logSQL(const String& sql)
	{
		if (m_flagLogSQL) {
			Log((char*)(getObjectType()), "SQL: %s", sql);
		}
	}
	
	void Database::_logSQL(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		if (m_flagLogSQL) {
			Log((char*)(getObjectType()), "SQL: %s Params=%s", sql, Variant(VariantList(params, nParams)).toJsonString());
		}
	}
	
	void Database::_logError(const String& sql)
	{
		if (m_flagLogErrors) {
			LogError((char*)(getObjectType()), "Error: %s SQL: %s", getErrorMessage(), sql);
		}
	}

	void Database::_logError(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		if (m_flagLogErrors) {
			LogError((char*)(getObjectType()), "Error: %s SQL: %s Params=%s", getErrorMessage(), sql, Variant(VariantList(params, nParams)).toJsonString());
		}
	}

}
