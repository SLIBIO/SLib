#include "../../../inc/slib/db/database.h"

SLIB_DB_NAMESPACE_BEGIN
Database::Database()
{
}

sl_bool Database::execute(const String& sql, const Variant* params, sl_uint32 nParams, sl_uint64* pOutAffectedRowsCount)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		if (statement->execute(params, nParams, pOutAffectedRowsCount)) {
			return sl_true;
		}
	}
	return sl_false;
}

Ref<DatabaseCursor> Database::query(const String& sql, const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		return statement->query(params, nParams);
	}
	return Ref<DatabaseCursor>::null();
}

List< Map<String, Variant> > Database::getListForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		return statement->getListForQueryResult(params, nParams);
	}
	return List< Map<String, Variant> >::null();
}

Map<String, Variant> Database::getRecordForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		return statement->getRecordForQueryResult(params, nParams);
	}
	return Map<String, Variant>::null();
}

Variant Database::getValueForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		return statement->getValueForQueryResult(params, nParams);
	}
	return Variant::null();
}

sl_bool Database::execute(const String& sql, sl_uint64* pOutAffectedRowsCount)
{
	return execute(sql, sl_null, 0, pOutAffectedRowsCount);
}

Ref<DatabaseCursor> Database::query(const String& sql)
{
	return query(sql, sl_null, 0);
}

List< Map<String, Variant> > Database::getListForQueryResult(const String& sql)
{
	List< Map<String, Variant> > ret;
	Ref<DatabaseCursor> cursor = query(sql);
	if (cursor.isNotNull()) {
		while (cursor->moveNext()) {
			ret.add(cursor->getRow());
		}
	}
	return ret;
}

Map<String, Variant> Database::getRecordForQueryResult(const String& sql)
{
	Map<String, Variant> ret;
	Ref<DatabaseCursor> cursor = query(sql);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			ret = cursor->getRow();
		}
	}
	return ret;
}

Variant Database::getValueForQueryResult(const String& sql)
{
	Variant ret;
	Ref<DatabaseCursor> cursor = query(sql);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			ret = cursor->getValue(0);
		}
	}
	return ret;
}
SLIB_DB_NAMESPACE_END
