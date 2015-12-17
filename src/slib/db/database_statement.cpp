#include "../../../inc/slib/db/database.h"

SLIB_DB_NAMESPACE_BEGIN
DatabaseStatement::DatabaseStatement()
{
}

Ref<Database> DatabaseStatement::getDatabase()
{
	return m_db;
}

List< Map<String, Variant> > DatabaseStatement::getListForQueryResult(const Variant* params, sl_uint32 nParams)
{
	List< Map<String, Variant> > ret;
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		while (cursor->moveNext()) {
			ret.add(cursor->getRow());
		}
	}
	return ret;
}

Map<String, Variant> DatabaseStatement::getRecordForQueryResult(const Variant* params, sl_uint32 nParams)
{
	Map<String, Variant> ret;
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			ret = cursor->getRow();
		}
	}
	return ret;
}

Variant DatabaseStatement::getValueForQueryResult(const Variant* params, sl_uint32 nParams)
{
	Variant ret;
	Ref<DatabaseCursor> cursor = query(params, nParams);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			ret = cursor->getValue(0);
		}
	}
	return ret;
}
SLIB_DB_NAMESPACE_END
