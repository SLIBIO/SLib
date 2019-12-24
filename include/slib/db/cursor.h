/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_DB_CURSOR
#define CHECKHEADER_SLIB_DB_CURSOR

#include "definition.h"

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
		virtual sl_int32 getColumnIndex(const StringParam& name) = 0;
	

		virtual HashMap<String, Variant> getRow() = 0;
	

		virtual Variant getValue(sl_uint32 index);

		virtual Variant getValue(const StringParam& name);
	

		virtual String getString(sl_uint32 index) = 0;

		virtual String getString(const StringParam& name);
	

		virtual sl_int64 getInt64(sl_uint32 index, sl_int64 defaultValue = 0);

		virtual sl_int64 getInt64(const StringParam& name, sl_int64 defaultValue = 0);
	

		virtual sl_uint64 getUint64(sl_uint32 index, sl_uint64 defaultValue = 0);

		virtual sl_uint64 getUint64(const StringParam& name, sl_uint64 defaultValue = 0);
	

		virtual sl_int32 getInt32(sl_uint32 index, sl_int32 defaultValue = 0);

		virtual sl_int32 getInt32(const StringParam& name, sl_int32 defaultValue = 0);
	

		virtual sl_uint32 getUint32(sl_uint32 index, sl_uint32 defaultValue = 0);

		virtual sl_uint32 getUint32(const StringParam& name, sl_uint32 defaultValue = 0);
	

		virtual float getFloat(sl_uint32 index, float defaultValue = 0);

		virtual float getFloat(const StringParam& name, float defaultValue = 0);
	

		virtual double getDouble(sl_uint32 index, double defaultValue = 0);

		virtual double getDouble(const StringParam& name, double defaultValue = 0);
	

		virtual sl_bool getBoolean(sl_uint32 index, sl_bool defaultValue = sl_false);

		virtual sl_bool getBoolean(const StringParam& name, sl_bool defaultValue = sl_false);
	
		
		virtual Time getTime(sl_uint32 index, const Time& defaultValue);

		virtual Time getTime(const StringParam& name, const Time& defaultValue);
	
		Time getTime(sl_uint32 index);

		Time getTime(const StringParam& name);
	

		virtual Memory getBlob(sl_uint32 index) = 0;

		virtual Memory getBlob(const StringParam& name);
	

		virtual sl_bool moveNext() = 0;
	
	protected:
		Ref<Database> m_db;

	};

}

#endif
