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

#ifndef CHECKHEADER_SLIB_DB_DATABASE
#define CHECKHEADER_SLIB_DB_DATABASE

#include "definition.h"

#include "sql.h"
#include "cursor.h"
#include "statement.h"

namespace slib
{
	
	class SLIB_EXPORT Database : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Database();

		~Database();

	public:
		Ref<DatabaseStatement> prepareStatement(const String& sql);
		
		Ref<DatabaseStatement> prepareStatement(const SqlBuilder& builder);
		
		sl_int64 executeBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE sl_int64 executeBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return executeBy(builder.toString(), params.data, params.count);
		}

		sl_int64 execute(const String& sql);
		
		template <class... ARGS>
		SLIB_INLINE sl_int64 execute(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return executeBy(sql, params, sizeof...(args));
		}
		
		Ref<DatabaseCursor> queryBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE Ref<DatabaseCursor> queryBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return queryBy(builder.toString(), params.data, params.count);
		}

		Ref<DatabaseCursor> query(const String& sql);
		
		template <class... ARGS>
		SLIB_INLINE Ref<DatabaseCursor> query(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return queryBy(sql, params, sizeof...(args));
		}
		
		List< HashMap<String, Variant> > getRecordsBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE List< HashMap<String, Variant> > getRecordsBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getRecordsBy(builder.toString(), params.data, params.count);
		}

		List< HashMap<String, Variant> > getRecords(const String& sql);
		
		template <class... ARGS>
		SLIB_INLINE List< HashMap<String, Variant> > getRecords(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordsBy(sql, params, sizeof...(args));
		}
		
		HashMap<String, Variant> getRecordBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE HashMap<String, Variant> getRecordBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getRecordBy(builder.toString(), params.data, params.count);
		}

		HashMap<String, Variant> getRecord(const String& sql);
		
		template <class... ARGS>
		SLIB_INLINE HashMap<String, Variant> getRecord(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordBy(sql, params, sizeof...(args));
		}
		
		Variant getValueBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE Variant getValueBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getValueBy(builder.toString(), params.data, params.count);
		}
		
		Variant getValue(const String& sql);
		
		template <class... ARGS>
		SLIB_INLINE Variant getValue(const String& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getValueBy(sql, params, sizeof...(args));
		}

		sl_bool isLoggingSQL();
		
		void setLoggingSQL(sl_bool flag);
		
		sl_bool isLoggingErrors();
		
		void setLoggingErrors(sl_bool flag);
		
		
		DatabaseDialect getDialect();
		
		
		virtual String getErrorMessage() = 0;
		
		virtual sl_bool isDatabaseExisting(const String& name) = 0;
		
		virtual List<String> getDatabases() = 0;

		virtual sl_bool isTableExisting(const String& name) = 0;
		
		virtual List<String> getTables() = 0;
		
		virtual sl_uint64 getLastInsertRowId() = 0;
		
		
		sl_bool createTable(const DatabaseCreateTableParam& param);

		sl_bool createTable(const DatabaseIdentifier& table, const ListParam<DatabaseColumnDefinition>& columns, DatabaseFlags flags = 0);

		sl_bool dropTable(const DatabaseIdentifier& table, DatabaseFlags flags = 0);

		sl_bool createIndex(const DatabaseCreateIndexParam& param);

		sl_bool createIndex(const DatabaseIdentifier& index, const String& table, const ListParam<DatabaseIndexColumn>& columns, DatabaseFlags flags = 0);
		
		sl_bool dropIndex(const DatabaseIdentifier& index, const String& table, DatabaseFlags flags = 0);
		
		
		Ref<DatabaseStatement> prepareInsert(const DatabaseIdentifier& table, const ListParam<String>& columns);

		Ref<DatabaseStatement> prepareUpdate(const DatabaseIdentifier& table, const ListParam<String>& columns, const DatabaseExpression& where);
		
		sl_bool startTransaction();
		
		sl_bool commitTransaction();
		
		sl_bool rollbackTransaction();

	protected:
		virtual Ref<DatabaseStatement> _prepareStatement(const String& sql) = 0;
		
		virtual sl_int64 _executeBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		virtual sl_int64 _execute(const String& sql);
		
		virtual Ref<DatabaseCursor> _queryBy(const String& sql, const Variant* params, sl_uint32 nParams);
		
		virtual Ref<DatabaseCursor> _query(const String& sql);
		
		void _logSQL(const String& sql);
		
		void _logSQL(const String& sql, const Variant* params, sl_uint32 nParams);
		
		void _logError(const String& sql);
		
		void _logError(const String& sql, const Variant* params, sl_uint32 nParams);

	protected:
		sl_bool m_flagLogSQL;
		sl_bool m_flagLogErrors;
		
		DatabaseDialect m_dialect;
		
	};

}

#endif
