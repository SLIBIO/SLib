#ifndef CHECKHEADER_SLIB_DATABASE_DB
#define CHECKHEADER_SLIB_DATABASE_DB

#include "definition.h"
#include "../core/object.h"
#include "../core/list.h"
#include "../core/map.h"
#include "../core/variant.h"

SLIB_DATABASE_NAMESPACE_BEGIN

class SLIB_EXPORT DatabaseCursor : public Object
{
public:
	SLIB_DECLARE_OBJECT(DatabaseCursor, Object)
public:
	SLIB_INLINE DatabaseCursor() {}

	virtual sl_uint32 getColumnCount() = 0;
	virtual String getColumnName(sl_uint32 index) = 0;
	// returns -1 when the column name not found
	virtual sl_int32 getColumnIndex(const String& name) = 0;

	virtual Map<String, Variant> getRow() = 0;
	virtual Variant getColumnValue(sl_uint32 index) = 0;
	virtual Variant getColumnValue(const String& name) = 0;
	virtual sl_bool moveNext() = 0;
};

class SLIB_EXPORT Database : public Object
{
	SLIB_DECLARE_OBJECT(Database, Object)
public:
	SLIB_INLINE Database() {}

	virtual void close() = 0;
	virtual sl_bool isOpened() = 0;

	virtual sl_bool executeUpdate(const String& sql) = 0;
	virtual Ref<DatabaseCursor> executeQuery(const String& sql) = 0;
	
	virtual List< Map<String, Variant> > getListForQueryResult(const String& sql) = 0;
	virtual Map<String, Variant> getRecordForQueryResult(const String& sql) = 0;
	virtual Variant getValueForQueryResult(const String& sql) = 0;
	
};

SLIB_DATABASE_NAMESPACE_END

#endif
