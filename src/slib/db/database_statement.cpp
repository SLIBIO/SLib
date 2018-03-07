/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/db/database.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(DatabaseStatement, Object)

	DatabaseStatement::DatabaseStatement()
	{
	}

	DatabaseStatement::~DatabaseStatement()
	{
	}

	Ref<Database> DatabaseStatement::getDatabase()
	{
		return m_db;
	}

	List< HashMap<String, Variant> > DatabaseStatement::getListForQueryResultBy(const Variant* params, sl_uint32 nParams)
	{
		List< HashMap<String, Variant> > ret;
		Ref<DatabaseCursor> cursor = queryBy(params, nParams);
		if (cursor.isNotNull()) {
			while (cursor->moveNext()) {
				ret.add_NoLock(cursor->getRow());
			}
		}
		return ret;
	}

	HashMap<String, Variant> DatabaseStatement::getRecordForQueryResultBy(const Variant* params, sl_uint32 nParams)
	{
		HashMap<String, Variant> ret;
		Ref<DatabaseCursor> cursor = queryBy(params, nParams);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getRow();
			}
		}
		return sl_null;
	}

	Variant DatabaseStatement::getValueForQueryResultBy(const Variant* params, sl_uint32 nParams)
	{
		Ref<DatabaseCursor> cursor = queryBy(params, nParams);
		if (cursor.isNotNull()) {
			if (cursor->moveNext()) {
				return cursor->getValue(0);
			}
		}
		return sl_null;
	}

}
