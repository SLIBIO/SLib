#ifndef CHECKHEADER_SLIB_DB_DATABASE
#define CHECKHEADER_SLIB_DB_DATABASE

#include "definition.h"

#include "../core/object.h"
#include "../core/list.h"
#include "../core/map.h"
#include "../core/variant.h"

namespace slib
{
	
	class Database;
	
	class SLIB_EXPORT DatabaseCursor : public Object
	{
		SLIB_DECLARE_OBJECT
	
	public:
		DatabaseCursor();
	
		~DatabaseCursor();
	
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
	
	class SLIB_EXPORT DatabaseStatement : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		DatabaseStatement();

		~DatabaseStatement();

	public:
		Ref<Database> getDatabase();

		virtual sl_int64 executeBy(const Variant* params = sl_null, sl_uint32 nParams = 0) = 0;

		SLIB_INLINE sl_int64 execute()
		{
			return executeBy();
		}

		template <class... ARGS>
		SLIB_INLINE sl_int64 execute(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return executeBy(params, sizeof...(args));
		}

		virtual Ref<DatabaseCursor> queryBy(const Variant* params = sl_null, sl_uint32 nParams = 0) = 0;
	
		SLIB_INLINE Ref<DatabaseCursor> query()
		{
			return queryBy();
		}

		template <class... ARGS>
		SLIB_INLINE Ref<DatabaseCursor> query(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return queryBy(params, sizeof...(args));
		}
	
		virtual List< Map<String, Variant> > getListForQueryResultBy(const Variant* params = sl_null, sl_uint32 nParams = 0);

		SLIB_INLINE List< Map<String, Variant> > getListForQueryResult()
		{
			return getListForQueryResultBy();
		}

		template <class... ARGS>
		SLIB_INLINE List< Map<String, Variant> > getListForQueryResult(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getListForQueryResultBy(params, sizeof...(args));
		}

		virtual Map<String, Variant> getRecordForQueryResultBy(const Variant* params = sl_null, sl_uint32 nParams = 0);

		SLIB_INLINE Map<String, Variant> getRecordForQueryResult()
		{
			return getRecordForQueryResultBy();
		}

		template <class... ARGS>
		SLIB_INLINE Map<String, Variant> getRecordForQueryResult(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordForQueryResultBy(params, sizeof...(args));
		}

		virtual Variant getValueForQueryResultBy(const Variant* params = sl_null, sl_uint32 nParams = 0);

		SLIB_INLINE Variant getValueForQueryResult()
		{
			return getValueForQueryResultBy();
		}

		template <class... ARGS>
		SLIB_INLINE Variant getValueForQueryResult(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getValueForQueryResultBy(params, sizeof...(args));
		}

	protected:
		Ref<Database> m_db;

	};
	
	class SLIB_EXPORT Database : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Database();

		~Database();

	public:
		virtual Ref<DatabaseStatement> prepareStatement(const String& sql) = 0;
	
		virtual sl_int64 execute(const String& sql);

		virtual Ref<DatabaseCursor> query(const String& sql);
	

		virtual List< Map<String, Variant> > getListForQueryResult(const String& sql);

		virtual Map<String, Variant> getRecordForQueryResult(const String& sql);

		virtual Variant getValueForQueryResult(const String& sql);


		virtual sl_int64 executeBy(const String& sql, const Variant* params, sl_uint32 nParams);

		template <class... ARGS>
		SLIB_INLINE sl_int64 execute(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return executeBy(sql, params, sizeof...(args));
		}

		virtual Ref<DatabaseCursor> queryBy(const String& sql, const Variant* params, sl_uint32 nParams);
	
		template <class... ARGS>
		SLIB_INLINE Ref<DatabaseCursor> query(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return queryBy(sql, params, sizeof...(args));
		}
	
		virtual List< Map<String, Variant> > getListForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams);

		template <class... ARGS>
		SLIB_INLINE List< Map<String, Variant> > getListForQueryResult(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getListForQueryResultBy(sql, params, sizeof...(args));
		}
	
		virtual Map<String, Variant> getRecordForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams);

		template <class... ARGS>
		SLIB_INLINE Map<String, Variant> getRecordForQueryResult(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordForQueryResultBy(sql, params, sizeof...(args));
		}

		virtual Variant getValueForQueryResultBy(const String& sql, const Variant* params, sl_uint32 nParams);
	
		template <class... ARGS>
		SLIB_INLINE Variant getValueForQueryResult(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getValueForQueryResultBy(sql, params, sizeof...(args));
		}
	
		virtual String getErrorMessage() = 0;

	
	};

}

#endif
