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

#include "slib/db/database.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(DatabaseCursor, Object)

	DatabaseCursor::DatabaseCursor()
	{
	}

	DatabaseCursor::~DatabaseCursor()
	{
	}

	Ref<Database> DatabaseCursor::getDatabase()
	{
		return m_db;
	}

	Variant DatabaseCursor::getValue(sl_uint32 index)
	{
		return getString(index);
	}

	Variant DatabaseCursor::getValue(const StringParam& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getValue(index);
		}
		return sl_null;
	}

	String DatabaseCursor::getString(sl_uint32 index)
	{
		return getValue(index).getString();
	}

	String DatabaseCursor::getString(const StringParam& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getString(index);
		}
		return sl_null;
	}

	sl_int64 DatabaseCursor::getInt64(sl_uint32 index, sl_int64 defaultValue)
	{
		return getValue(index).getInt64(defaultValue);
	}

	sl_int64 DatabaseCursor::getInt64(const StringParam& name, sl_int64 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getInt64(index, defaultValue);
		}
		return defaultValue;
	}

	sl_uint64 DatabaseCursor::getUint64(sl_uint32 index, sl_uint64 defaultValue)
	{
		return getValue(index).getUint64(defaultValue);
	}

	sl_uint64 DatabaseCursor::getUint64(const StringParam& name, sl_uint64 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getUint64(index, defaultValue);
		}
		return defaultValue;
	}

	sl_int32 DatabaseCursor::getInt32(sl_uint32 index, sl_int32 defaultValue)
	{
		return getValue(index).getInt32(defaultValue);
	}

	sl_int32 DatabaseCursor::getInt32(const StringParam& name, sl_int32 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getInt32(index, defaultValue);
		}
		return defaultValue;
	}

	sl_uint32 DatabaseCursor::getUint32(sl_uint32 index, sl_uint32 defaultValue)
	{
		return getValue(index).getUint32(defaultValue);
	}

	sl_uint32 DatabaseCursor::getUint32(const StringParam& name, sl_uint32 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getUint32(index, defaultValue);
		}
		return defaultValue;
	}

	float DatabaseCursor::getFloat(sl_uint32 index, float defaultValue)
	{
		return getValue(index).getFloat(defaultValue);
	}

	float DatabaseCursor::getFloat(const StringParam& name, float defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getFloat(index, defaultValue);
		}
		return defaultValue;
	}

	double DatabaseCursor::getDouble(sl_uint32 index, double defaultValue)
	{
		return getValue(index).getDouble(defaultValue);
	}

	double DatabaseCursor::getDouble(const StringParam& name, double defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getDouble(index, defaultValue);
		}
		return defaultValue;
	}

	sl_bool DatabaseCursor::getBoolean(sl_uint32 index, sl_bool defaultValue)
	{
		return getValue(index).getBoolean(defaultValue);
	}

	sl_bool DatabaseCursor::getBoolean(const StringParam& name, sl_bool defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getInt64(index, defaultValue ? 1 : 0) != 0;
		}
		return defaultValue;
	}
	
	Time DatabaseCursor::getTime(sl_uint32 index, const Time& defaultValue)
	{
		return getValue(index).getTime(defaultValue);
	}

	Time DatabaseCursor::getTime(const StringParam& name, const Time& defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getTime(index, defaultValue);
		}
		return defaultValue;
	}

	Time DatabaseCursor::getTime(sl_uint32 index)
	{
		return getTime(index, Time::zero());
	}

	Time DatabaseCursor::getTime(const StringParam& name)
	{
		return getTime(name, Time::zero());
	}

	Memory DatabaseCursor::getBlob(sl_uint32 index)
	{
		return getValue(index).getMemory();
	}

	Memory DatabaseCursor::getBlob(const StringParam& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getBlob(index);
		}
		return sl_null;
	}

}
