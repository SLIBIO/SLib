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
		Ref<DatabaseStatement> prepareStatement(const StringParam& sql);
		
		Ref<DatabaseStatement> prepareStatement(const SqlBuilder& builder);
		
		sl_int64 executeBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE sl_int64 executeBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return executeBy(builder.toString(), params.data, params.count);
		}

		sl_int64 execute(const StringParam& sql);
		
		template <class... ARGS>
		SLIB_INLINE sl_int64 execute(const StringParam& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return executeBy(sql, params, sizeof...(args));
		}
		
		Ref<DatabaseCursor> queryBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE Ref<DatabaseCursor> queryBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return queryBy(builder.toString(), params.data, params.count);
		}

		Ref<DatabaseCursor> query(const StringParam& sql);
		
		template <class... ARGS>
		SLIB_INLINE Ref<DatabaseCursor> query(const StringParam& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return queryBy(sql, params, sizeof...(args));
		}
		
		List< HashMap<String, Variant> > getRecordsBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE List< HashMap<String, Variant> > getRecordsBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getRecordsBy(builder.toString(), params.data, params.count);
		}

		List< HashMap<String, Variant> > getRecords(const StringParam& sql);
		
		template <class... ARGS>
		SLIB_INLINE List< HashMap<String, Variant> > getRecords(const StringParam& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordsBy(sql, params, sizeof...(args));
		}
		
		HashMap<String, Variant> getRecordBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE HashMap<String, Variant> getRecordBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getRecordBy(builder.toString(), params.data, params.count);
		}

		HashMap<String, Variant> getRecord(const StringParam& sql);
		
		template <class... ARGS>
		SLIB_INLINE HashMap<String, Variant> getRecord(const StringParam& sql, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordBy(sql, params, sizeof...(args));
		}
		
		Variant getValueBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE Variant getValueBy(const SqlBuilder& builder, const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, builder.parameters);
			return getValueBy(builder.toString(), params.data, params.count);
		}
		
		Variant getValue(const StringParam& sql);
		
		template <class... ARGS>
		SLIB_INLINE Variant getValue(const StringParam& sql, ARGS&&... args)
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
		
		virtual sl_bool isDatabaseExisting(const StringParam& name) = 0;
		
		virtual List<String> getDatabases() = 0;

		virtual sl_bool isTableExisting(const StringParam& name) = 0;
		
		virtual List<String> getTables() = 0;
		
		virtual sl_uint64 getLastInsertRowId() = 0;
		
		
		sl_bool createTable(const DatabaseCreateTableParam& param);

		sl_bool createTable(const DatabaseIdentifier& table, const ListParam<DatabaseColumnDefinition>& columns, DatabaseFlags flags = 0);

		sl_bool dropTable(const DatabaseIdentifier& table, DatabaseFlags flags = 0);

		sl_bool createIndex(const DatabaseCreateIndexParam& param);

		sl_bool createIndex(const DatabaseIdentifier& index, const String& table, const ListParam<DatabaseIndexColumn>& columns, DatabaseFlags flags = 0);
		
		sl_bool dropIndex(const DatabaseIdentifier& index, const String& table, DatabaseFlags flags = 0);
		
		
		Ref<DatabaseStatement> prepareInsert(const DatabaseIdentifier& table, const ListParam<String>& columns);
		
		template <class MAP>
		sl_int64 insert(const DatabaseIdentifier& table, const MAP& map)
		{
			List<String> names;
			List<Variant> values;
			for (auto& pair : map) {
				names.add_NoLock(pair.key);
				values.add_NoLock(pair.value);
			}
			Ref<DatabaseStatement> stmt = prepareInsert(table, names);
			if (stmt.isNotNull()) {
				return stmt->executeBy(values.getData(), (sl_uint32)(values.getCount()));
			}
			return -1;
		}

		Ref<DatabaseStatement> prepareUpdate(const DatabaseIdentifier& table, const ListParam<String>& columns, const DatabaseExpression& where);
		
		template <class MAP, class... ARGS>
		sl_int64 update(const DatabaseIdentifier& table, const MAP& map, const DatabaseExpression& where, ARGS&&... args)
		{
			List<String> names;
			List<Variant> values;
			for (auto& pair : map) {
				names.add_NoLock(pair.key);
				values.add_NoLock(pair.value);
			}
			Variant params[] = {Forward<ARGS>(args)...};
			sl_size nParams = (sl_size)(sizeof...(args));
			for (sl_size i = 0; i < nParams; i++) {
				values.add_NoLock(params[i]);
			}
			Ref<DatabaseStatement> stmt = prepareUpdate(table, names, where);
			if (stmt.isNotNull()) {
				return stmt->executeBy(values.getData(), (sl_uint32)(values.getCount()));
			}
			return -1;
		}
		
		Ref<DatabaseStatement> prepareDelete(const DatabaseIdentifier& table, const DatabaseExpression& where);
		
		template <class... ARGS>
		sl_int64 deleteRecords(const DatabaseIdentifier& table, const DatabaseExpression& where, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			Ref<DatabaseStatement> stmt = prepareDelete(table, where);
			if (stmt.isNotNull()) {
				return stmt->executeBy(params, sizeof...(args));
			}
			return -1;
		}
		
		Ref<DatabaseStatement> prepareQuery(const DatabaseIdentifier& table, const DatabaseExpression& where);
		
		template <class... ARGS>
		List< HashMap<String, Variant> > findRecords(const DatabaseIdentifier& table, const DatabaseExpression& where, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getRecordsBy(params, sizeof...(args));
			}
			return sl_null;
		}

		template <class T>
		List< HashMap<String, Variant> > findRecordsBy(const DatabaseIdentifier& table, const DatabaseExpression& where, const T& params)
		{
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getRecordsBy(params);
			}
			return sl_null;
		}
		
		template <class... ARGS>
		HashMap<String, Variant> findRecord(const DatabaseIdentifier& table, const DatabaseExpression& where, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getRecordBy(params, sizeof...(args));
			}
			return sl_null;
		}
		
		template <class T>
		HashMap<String, Variant> findRecordBy(const DatabaseIdentifier& table, const DatabaseExpression& where, const T& params)
		{
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getRecordBy(params);
			}
			return sl_null;
		}
		
		template <class... ARGS>
		Variant findValue(const DatabaseIdentifier& table, const DatabaseExpression& where, ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getValueBy(params, sizeof...(args));
			}
			return sl_null;
		}
		
		template <class T>
		Variant findValueBy(const DatabaseIdentifier& table, const DatabaseExpression& where, const T& params)
		{
			Ref<DatabaseStatement> stmt = prepareQuery(table, where);
			if (stmt.isNotNull()) {
				return stmt->getValueBy(params);
			}
			return sl_null;
		}
		
		sl_bool startTransaction();
		
		sl_bool commitTransaction();
		
		sl_bool rollbackTransaction();

	protected:
		virtual Ref<DatabaseStatement> _prepareStatement(const StringParam& sql) = 0;
		
		virtual sl_int64 _executeBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		virtual sl_int64 _execute(const StringParam& sql);
		
		virtual Ref<DatabaseCursor> _queryBy(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		virtual Ref<DatabaseCursor> _query(const StringParam& sql);
		
		void _logSQL(const StringParam& sql);
		
		void _logSQL(const StringParam& sql, const Variant* params, sl_uint32 nParams);
		
		void _logError(const StringParam& sql);
		
		void _logError(const StringParam& sql, const Variant* params, sl_uint32 nParams);

	protected:
		sl_bool m_flagLogSQL;
		sl_bool m_flagLogErrors;
		
		DatabaseDialect m_dialect;
		
	};

}

#endif
