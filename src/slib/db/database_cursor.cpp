/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	Variant DatabaseCursor::getValue(const String& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getValue(index);
		}
		return sl_null;
	}

	String DatabaseCursor::getString(const String& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getString(index);
		}
		return sl_null;
	}

	sl_int64 DatabaseCursor::getInt64(sl_uint32 index, sl_int64 defaultValue)
	{
		return getString(index).parseInt64(10, defaultValue);
	}

	sl_int64 DatabaseCursor::getInt64(const String& name, sl_int64 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getInt64(index, defaultValue);
		}
		return defaultValue;
	}

	sl_uint64 DatabaseCursor::getUint64(sl_uint32 index, sl_uint64 defaultValue)
	{
		return getInt64(index, defaultValue);
	}

	sl_uint64 DatabaseCursor::getUint64(const String& name, sl_uint64 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getUint64(index, defaultValue);
		}
		return defaultValue;
	}

	sl_int32 DatabaseCursor::getInt32(sl_uint32 index, sl_int32 defaultValue)
	{
		return (sl_int32)(getInt64(index, defaultValue));
	}

	sl_int32 DatabaseCursor::getInt32(const String& name, sl_int32 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getInt32(index, defaultValue);
		}
		return defaultValue;
	}

	sl_uint32 DatabaseCursor::getUint32(sl_uint32 index, sl_uint32 defaultValue)
	{
		return (sl_int32)(getInt64(index, defaultValue));
	}

	sl_uint32 DatabaseCursor::getUint32(const String& name, sl_uint32 defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getUint32(index, defaultValue);
		}
		return defaultValue;
	}

	float DatabaseCursor::getFloat(sl_uint32 index, float defaultValue)
	{
		return (float)(getDouble(index, defaultValue));
	}

	float DatabaseCursor::getFloat(const String& name, float defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getFloat(index, defaultValue);
		}
		return defaultValue;
	}

	double DatabaseCursor::getDouble(sl_uint32 index, double defaultValue)
	{
		return getString(index).parseDouble(defaultValue);
	}

	double DatabaseCursor::getDouble(const String& name, double defaultValue)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getDouble(index, defaultValue);
		}
		return defaultValue;
	}

	Time DatabaseCursor::getTime(sl_uint32 index, const Time& defaultValue)
	{
		String s = getString(index);
		if (s.isNotEmpty()) {
			return Time(s);
		}
		return defaultValue;
	}

	Time DatabaseCursor::getTime(const String& name, const Time& defaultValue)
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

	Time DatabaseCursor::getTime(const String& name)
	{
		return getTime(name, Time::zero());
	}

	Memory DatabaseCursor::getBlob(const String& name)
	{
		sl_int32 index = getColumnIndex(name);
		if (index >= 0) {
			return getBlob(index);
		}
		return sl_null;
	}

}
