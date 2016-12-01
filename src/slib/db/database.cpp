#include "../../../inc/slib/db/database.h"

SLIB_DB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Database, Object)

sl_int64 Database::execute(const String& sql, const Variant* params, sl_uint32 nParams)
{
	Ref<DatabaseStatement> statement = prepareStatement(sql);
	if (statement.isNotNull()) {
		return statement->execute(params, nParams);
	}
	return -1;
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

sl_int64 Database::execute(const String& sql)
{
	return execute(sql, sl_null, 0);
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
	return Map<String, Variant>::null();
}

Variant Database::getValueForQueryResult(const String& sql)
{
	Ref<DatabaseCursor> cursor = query(sql);
	if (cursor.isNotNull()) {
		if (cursor->moveNext()) {
			return cursor->getValue(0);
		}
	}
	return Variant::null();
}

#define FORMAT_BEGIN_VARS_LIST(n) SLIB_ALIGN(16) char _vars[sizeof(Variant)*n]; Variant* vars=(Variant*)((void*)_vars); char* pvar = _vars;
#define FORMAT_ADD_VAR_TO_LIST(s) { char* sv = (char*)((void*)&s); for (int iv = 0; iv < sizeof(Variant); iv++) { *(pvar++) = *(sv++); } }
#define FORMAT_PARAMS_LIST_1 FORMAT_ADD_VAR_TO_LIST(param1)
#define FORMAT_PARAMS_LIST_2 FORMAT_ADD_VAR_TO_LIST(param1) FORMAT_ADD_VAR_TO_LIST(param2)
#define FORMAT_PARAMS_LIST_3 FORMAT_PARAMS_LIST_2 FORMAT_ADD_VAR_TO_LIST(param3)
#define FORMAT_PARAMS_LIST_4 FORMAT_PARAMS_LIST_3 FORMAT_ADD_VAR_TO_LIST(param4)
#define FORMAT_PARAMS_LIST_5 FORMAT_PARAMS_LIST_4 FORMAT_ADD_VAR_TO_LIST(param5)
#define FORMAT_PARAMS_LIST_6 FORMAT_PARAMS_LIST_5 FORMAT_ADD_VAR_TO_LIST(param6)
#define FORMAT_PARAMS_LIST_7 FORMAT_PARAMS_LIST_6 FORMAT_ADD_VAR_TO_LIST(param7)
#define FORMAT_PARAMS_LIST_8 FORMAT_PARAMS_LIST_7 FORMAT_ADD_VAR_TO_LIST(param8)
#define FORMAT_PARAMS_LIST_9 FORMAT_PARAMS_LIST_8 FORMAT_ADD_VAR_TO_LIST(param9)
#define FORMAT_PARAMS_LIST_10 FORMAT_PARAMS_LIST_9 FORMAT_ADD_VAR_TO_LIST(param10)
#define FORMAT_PARAMS_LIST_11 FORMAT_PARAMS_LIST_10 FORMAT_ADD_VAR_TO_LIST(param11)
#define FORMAT_PARAMS_LIST_12 FORMAT_PARAMS_LIST_11 FORMAT_ADD_VAR_TO_LIST(param12)
#define FORMAT_PARAMS_LIST_13 FORMAT_PARAMS_LIST_12 FORMAT_ADD_VAR_TO_LIST(param13)
#define FORMAT_PARAMS_LIST_14 FORMAT_PARAMS_LIST_13 FORMAT_ADD_VAR_TO_LIST(param14)
#define FORMAT_PARAMS_LIST_15 FORMAT_PARAMS_LIST_14 FORMAT_ADD_VAR_TO_LIST(param15)
#define FORMAT_PARAMS_LIST_16 FORMAT_PARAMS_LIST_15 FORMAT_ADD_VAR_TO_LIST(param16)
#define FORMAT_PARAMS_LIST_17 FORMAT_PARAMS_LIST_16 FORMAT_ADD_VAR_TO_LIST(param17)
#define FORMAT_PARAMS_LIST_18 FORMAT_PARAMS_LIST_17 FORMAT_ADD_VAR_TO_LIST(param18)
#define FORMAT_PARAMS_LIST_19 FORMAT_PARAMS_LIST_18 FORMAT_ADD_VAR_TO_LIST(param19)
#define FORMAT_PARAMS_LIST_20 FORMAT_PARAMS_LIST_19 FORMAT_ADD_VAR_TO_LIST(param20)
#define FORMAT_PARAMS_LIST_21 FORMAT_PARAMS_LIST_20 FORMAT_ADD_VAR_TO_LIST(param21)
#define FORMAT_PARAMS_LIST_22 FORMAT_PARAMS_LIST_21 FORMAT_ADD_VAR_TO_LIST(param22)
#define FORMAT_PARAMS_LIST_23 FORMAT_PARAMS_LIST_22 FORMAT_ADD_VAR_TO_LIST(param23)
#define FORMAT_PARAMS_LIST_24 FORMAT_PARAMS_LIST_23 FORMAT_ADD_VAR_TO_LIST(param24)
#define FORMAT_PARAMS_LIST_25 FORMAT_PARAMS_LIST_24 FORMAT_ADD_VAR_TO_LIST(param25)
#define FORMAT_PARAMS_LIST_26 FORMAT_PARAMS_LIST_25 FORMAT_ADD_VAR_TO_LIST(param26)
#define FORMAT_PARAMS_LIST_27 FORMAT_PARAMS_LIST_26 FORMAT_ADD_VAR_TO_LIST(param27)
#define FORMAT_PARAMS_LIST_28 FORMAT_PARAMS_LIST_27 FORMAT_ADD_VAR_TO_LIST(param28)
#define FORMAT_PARAMS_LIST_29 FORMAT_PARAMS_LIST_28 FORMAT_ADD_VAR_TO_LIST(param29)
#define FORMAT_PARAMS_LIST_30 FORMAT_PARAMS_LIST_29 FORMAT_ADD_VAR_TO_LIST(param30)

#define DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, nParams) \
RET CLASS::NAME##By(DEFINE_RUN_BY_PARAMS_PREFIX _SLIB_STRING_DECLARE_FORMAT_FUNCTIONS_PARAMS##nParams) { \
	FORMAT_BEGIN_VARS_LIST(nParams) \
	FORMAT_PARAMS_LIST_##nParams \
	return NAME(DEFINE_RUN_BY_CALL_PREFIX vars, nParams); \
}

#define DEFINE_RUN_BY_PARAMS(RET, CLASS, NAME) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 1) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 2) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 3) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 4) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 5) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 6) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 7) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 8) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 9) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 10) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 11) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 12) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 13) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 14) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 15) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 16) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 17) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 18) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 19) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 20) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 21) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 22) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 23) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 24) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 25) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 26) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 27) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 28) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 29) \
DEFINE_RUN_BY_PARAMS_SECTION(RET, CLASS, NAME, 30)

#define DEFINE_RUN_BY_PARAMS_PREFIX
#define DEFINE_RUN_BY_CALL_PREFIX
DEFINE_RUN_BY_PARAMS(sl_int64, DatabaseStatement, execute)
DEFINE_RUN_BY_PARAMS(Ref<DatabaseCursor>, DatabaseStatement, query)
DEFINE_RUN_BY_PARAMS(VariantMapList, DatabaseStatement, getListForQueryResult)
DEFINE_RUN_BY_PARAMS(VariantMap, DatabaseStatement, getRecordForQueryResult)
DEFINE_RUN_BY_PARAMS(Variant, DatabaseStatement, getValueForQueryResult)
#undef DEFINE_RUN_BY_PARAMS_PREFIX
#undef DEFINE_RUN_BY_CALL_PREFIX


#define DEFINE_RUN_BY_PARAMS_PREFIX const String& sql,
#define DEFINE_RUN_BY_CALL_PREFIX sql,
DEFINE_RUN_BY_PARAMS(sl_int64, Database, execute)
DEFINE_RUN_BY_PARAMS(Ref<DatabaseCursor>, Database, query)
DEFINE_RUN_BY_PARAMS(VariantMapList, Database, getListForQueryResult)
DEFINE_RUN_BY_PARAMS(VariantMap, Database, getRecordForQueryResult)
DEFINE_RUN_BY_PARAMS(Variant, Database, getValueForQueryResult)
#undef DEFINE_RUN_BY_PARAMS_PREFIX
#undef DEFINE_RUN_BY_CALL_PREFIX


SLIB_DB_NAMESPACE_END
