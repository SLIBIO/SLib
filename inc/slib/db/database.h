#ifndef CHECKHEADER_SLIB_DB_DATABASE
#define CHECKHEADER_SLIB_DB_DATABASE

#include "definition.h"

#include "../core/object.h"
#include "../core/list.h"
#include "../core/map.h"
#include "../core/variant.h"

SLIB_DB_NAMESPACE_BEGIN

class Database;

class SLIB_EXPORT DatabaseCursor : public Object
{
	SLIB_DECLARE_OBJECT

public:
	Ref<Database> getDatabase();

	virtual sl_uint32 getColumnsCount() = 0;

	virtual String getColumnName(sl_uint32 index) = 0;
	
	// returns -1 when the column name not found
	virtual sl_int32 getColumnIndex(const String& name) = 0;

	
	virtual Map<String, Variant> getRow() = 0;

	
	virtual Variant getValue(sl_uint32 index);
	
	virtual Variant getValue(const String& name);

	
	virtual String getString(sl_uint32 index) = 0;
	
	virtual String getString(const String& name);

	
	virtual sl_int64 getInt64(sl_uint32 index, sl_int64 defaultValue = 0);
	
	virtual sl_int64 getInt64(const String& name, sl_int64 defaultValue = 0);

	
	virtual sl_uint64 getUint64(sl_uint32 index, sl_uint64 defaultValue = 0);
	
	virtual sl_uint64 getUint64(const String& name, sl_uint64 defaultValue = 0);

	
	virtual sl_int32 getInt32(sl_uint32 index, sl_int32 defaultValue = 0);
	
	virtual sl_int32 getInt32(const String& name, sl_int32 defaultValue = 0);

	
	virtual sl_uint32 getUint32(sl_uint32 index, sl_uint32 defaultValue = 0);
	
	virtual sl_uint32 getUint32(const String& name, sl_uint32 defaultValue = 0);

	
	virtual float getFloat(sl_uint32 index, float defaultValue = 0);
	
	virtual float getFloat(const String& name, float defaultValue = 0);

	
	virtual double getDouble(sl_uint32 index, double defaultValue = 0);
	
	virtual double getDouble(const String& name, double defaultValue = 0);

	
	virtual Time getTime(sl_uint32 index, const Time& defaultValue);
	
	virtual Time getTime(const String& name, const Time& defaultValue);

	Time getTime(sl_uint32 index);
	
	Time getTime(const String& name);

	
	virtual Memory getBlob(sl_uint32 index) = 0;
	
	virtual Memory getBlob(const String& name);

	
	virtual sl_bool moveNext() = 0;

protected:
	Ref<Database> m_db;
	
};

#define _SLIB_DB_DECLARE_RUN_BY_PARAMS(RET, NAME) \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&); \
	RET NAME##By(_SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&, const Variant&);

class SLIB_EXPORT DatabaseStatement : public Object
{
	SLIB_DECLARE_OBJECT

public:
	Ref<Database> getDatabase();
	
	
	virtual sl_int64 execute(const Variant* params = sl_null, sl_uint32 nParams = 0) = 0;
	
	virtual Ref<DatabaseCursor> query(const Variant* params = sl_null, sl_uint32 nParams = 0) = 0;

	
	virtual List< Map<String, Variant> > getListForQueryResult(const Variant* params = sl_null, sl_uint32 nParams = 0);
	
	virtual Map<String, Variant> getRecordForQueryResult(const Variant* params = sl_null, sl_uint32 nParams = 0);
	
	virtual Variant getValueForQueryResult(const Variant* params = sl_null, sl_uint32 nParams = 0);

	
#define _SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(sl_int64, execute)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(Ref<DatabaseCursor>, query)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(VariantMapList, getListForQueryResult)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(VariantMap, getRecordForQueryResult)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(Variant, getValueForQueryResult)
#undef _SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX

protected:
	Ref<Database> m_db;
	
};

class SLIB_EXPORT Database : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	virtual Ref<DatabaseStatement> prepareStatement(const String& sql) = 0;

	virtual sl_int64 execute(const String& sql);
	
	virtual Ref<DatabaseCursor> query(const String& sql);	

	
	virtual List< Map<String, Variant> > getListForQueryResult(const String& sql);
	
	virtual Map<String, Variant> getRecordForQueryResult(const String& sql);
	
	virtual Variant getValueForQueryResult(const String& sql);
	
	
	virtual sl_int64 execute(const String& sql, const Variant* params, sl_uint32 nParams);
	
	virtual Ref<DatabaseCursor> query(const String& sql, const Variant* params, sl_uint32 nParams);

	
	virtual List< Map<String, Variant> > getListForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams);
	
	virtual Map<String, Variant> getRecordForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams);
	
	virtual Variant getValueForQueryResult(const String& sql, const Variant* params, sl_uint32 nParams);

	virtual String getErrorMessage() = 0;
	
	
#define _SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX const String& sql,
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(sl_int64, execute)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(Ref<DatabaseCursor>, query)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(VariantMapList, getListForQueryResult)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(VariantMap, getRecordForQueryResult)
	_SLIB_DB_DECLARE_RUN_BY_PARAMS(Variant, getValueForQueryResult)
#undef _SLIB_DB_DECLARE_RUN_BY_PARAMS_PREFIX

};

SLIB_DB_NAMESPACE_END

#endif
