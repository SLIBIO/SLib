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

	List<String> DatabaseStatement::getParameterNames()
	{
		return m_names;
	}

	void DatabaseStatement::setParameterNames(const ListParam<String>& names)
	{
		m_names = names.toList();
	}

	List< HashMap<String, Variant> > DatabaseStatement::getRecordsBy(const Variant* params, sl_uint32 nParams)
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

	HashMap<String, Variant> DatabaseStatement::getRecordBy(const Variant* params, sl_uint32 nParams)
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

	Variant DatabaseStatement::getValueBy(const Variant* params, sl_uint32 nParams)
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
