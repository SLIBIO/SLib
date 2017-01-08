#include "../../../inc/slib/db/database.h"

SLIB_DB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(DatabaseStatement, Object)

Ref<Database> DatabaseStatement::getDatabase()
{
	return m_db;
}

List< Map<String, Variant> > DatabaseStatement::getListForQueryResultBy(const Variant* params, sl_uint32 nParams)
{
	List< Map<String, Variant> > ret;
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		while (cursor->moveNext()) {
			ret.add_NoLock(cursor->getRow());
		}
	}
	return ret;
}

Map<String, Variant> DatabaseStatement::getRecordForQueryResultBy(const Variant* params, sl_uint32 nParams)
{
	Map<String, Variant> ret;
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			return cursor->getRow();
		}
	}
	return sl_null;
}

Variant DatabaseStatement::getValueForQueryResultBy(const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			return cursor->getValue(0);
		}
	}
	return sl_null;
}

SLIB_DB_NAMESPACE_END
