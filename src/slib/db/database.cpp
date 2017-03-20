/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/db/database.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Database, Object)

	Database::Database()
	{
	}

	Database::~Database()
	{
	}

	sl_int64 Database::executeBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->executeBy(params, nParams);
		}
		return -1;
	}

	Ref<DatabaseCursor> Database::queryBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->queryBy(params, nParams);
		}
		return sl_null;
	}

	List< Map<String, Variant> > Database::getListForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->getListForQueryResultBy(params, nParams);
		}
		return sl_null;
	}

	Map<String, Variant> Database::getRecordForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->getRecordForQueryResultBy(params, nParams);
		}
		return sl_null;
	}

	Variant Database::getValueForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseStatement> statement = prepareStatement(sql);
		if (statement.isNotNull()) {
			return statement->getValueForQueryResultBy(params, nParams);
		}
		return sl_null;
	}

	sl_int64 Database::execute(const String& sql)
	{
		return executeBy(sql, sl_null, 0);
	}

	Ref<DatabaseCursor> Database::query(const String& sql)
	{
		return queryBy(sql, sl_null, 0);
	}

	List< Map<String, Variant> > Database::getListForQueryResult(const String& sql)
	{
		List< Map<String, Variant> > ret;
		Ref<DatabaseCursor> cursor = query(sql);
		if (cursor.isNotNull()) {
			while (cursor->moveNext()) {
				ret.add_NoLock(cursor->getRow());
			}
		}
		return ret;
	}

	Map<String, Variant> Database::getRecordForQueryResult(const String& sql)
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

}
