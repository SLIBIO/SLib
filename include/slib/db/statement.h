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

#ifndef CHECKHEADER_SLIB_DB_STATEMENT
#define CHECKHEADER_SLIB_DB_STATEMENT

#include "definition.h"

#include "parameter.h"

namespace slib
{

	class Database;
	
	class SLIB_EXPORT DatabaseStatement : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		DatabaseStatement();

		~DatabaseStatement();

	public:
		Ref<Database> getDatabase();
		
		List<String> getParameterNames();
		
		void setParameterNames(const ListParam<String>& names);
		
		virtual sl_int64 executeBy(const Variant* params, sl_uint32 nParams) = 0;

		template <class T>
		SLIB_INLINE sl_int64 executeBy(const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, m_names);
			return executeBy(params.data, params.count);
		}

		SLIB_INLINE sl_int64 execute()
		{
			return executeBy(sl_null, 0);
		}

		template <class... ARGS>
		SLIB_INLINE sl_int64 execute(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return executeBy(params, sizeof...(args));
		}

		virtual Ref<DatabaseCursor> queryBy(const Variant* params, sl_uint32 nParams) = 0;

		template <class T>
		SLIB_INLINE Ref<DatabaseCursor> queryBy(const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, m_names);
			return queryBy(params.data, params.count);
		}

		SLIB_INLINE Ref<DatabaseCursor> query()
		{
			return queryBy(sl_null, 0);
		}

		template <class... ARGS>
		SLIB_INLINE Ref<DatabaseCursor> query(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return queryBy(params, sizeof...(args));
		}
	
		virtual List< HashMap<String, Variant> > getRecordsBy(const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE List< HashMap<String, Variant> > getRecordsBy(const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, m_names);
			return getRecordsBy(params.data, params.count);
		}

		SLIB_INLINE List< HashMap<String, Variant> > getRecords()
		{
			return getRecordsBy(sl_null, 0);
		}

		template <class... ARGS>
		SLIB_INLINE List< HashMap<String, Variant> > getRecords(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordsBy(params, sizeof...(args));
		}

		virtual HashMap<String, Variant> getRecordBy(const Variant* params, sl_uint32 nParams);

		template <class T>
		SLIB_INLINE HashMap<String, Variant> getRecordBy(const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, m_names);
			return getRecordBy(params.data, params.count);
		}

		SLIB_INLINE HashMap<String, Variant> getRecord()
		{
			return getRecordBy(sl_null, 0);
		}

		template <class... ARGS>
		SLIB_INLINE HashMap<String, Variant> getRecord(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getRecordBy(params, sizeof...(args));
		}

		virtual Variant getValueBy(const Variant* params, sl_uint32 nParams);
		
		template <class T>
		SLIB_INLINE Variant getValueBy(const T& _params)
		{
			DatabaseParametersLocker<T> params(_params, m_names);
			return getValueBy(params.data, params.count);
		}
		
		SLIB_INLINE Variant getValue()
		{
			return getValueBy(sl_null, 0);
		}

		template <class... ARGS>
		SLIB_INLINE Variant getValue(ARGS&&... args)
		{
			Variant params[] = {Forward<ARGS>(args)...};
			return getValueBy(params, sizeof...(args));
		}

	protected:
		Ref<Database> m_db;
		List<String> m_names;

	};

}

#endif
