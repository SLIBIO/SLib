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

#ifndef CHECKHEADER_SLIB_DB_REDIS
#define CHECKHEADER_SLIB_DB_REDIS

#include "definition.h"

#include "../core/object.h"
#include "../core/variant.h"

namespace slib
{

	class SLIB_EXPORT RedisDatabase : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		RedisDatabase();

		~RedisDatabase();

	public:
		static Ref<RedisDatabase> connect(const String& ip, sl_uint16 port);
		
	public:
		virtual sl_bool execute(const String& command, Variant* pValue) = 0;
		
		Variant execute(const String& key);

		virtual sl_bool set(const String& key, const Variant& value) = 0;
		
		virtual sl_bool get(const String& key, String* pValue) = 0;
		
		String get(const String& key);
		
		String get(const String& key, const String& def);

		virtual sl_bool del(const String& key) = 0;
		
		virtual sl_bool incr(const String& key, sl_int64* pValue) = 0;
		
		sl_int64 incr(const String& key, sl_int64 def = 0);

		virtual sl_bool decr(const String& key, sl_int64* pValue) = 0;
		
		sl_int64 decr(const String& key, sl_int64 def = 0);

		virtual sl_bool incrby(const String& key, sl_int64 n, sl_int64* pValue) = 0;
		
		sl_int64 incrby(const String& key, sl_int64 n, sl_int64 def = 0);
		
		virtual sl_bool decrby(const String& key, sl_int64 n, sl_int64* pValue) = 0;
		
		sl_int64 decrby(const String& key, sl_int64 n, sl_int64 def = 0);
		
		virtual sl_bool llen(const String& key, sl_int64* pValue) = 0;
		
		sl_int64 llen(const String& key);
		
		virtual sl_int64 lpush(const String& key, const Variant& value) = 0;

		virtual sl_int64 rpush(const String& key, const Variant& value) = 0;

		virtual sl_bool lindex(const String& key, sl_int64 index, String* pValue) = 0;
		
		String lindex(const String& key, sl_int64 index);
		
		String lindex(const String& key, sl_int64 index, const String& def);

		virtual sl_bool lset(const String& key, sl_int64 index, const Variant& value) = 0;

		virtual sl_bool ltrm(const String& key, sl_int64 start, sl_int64 stop) = 0;

		virtual sl_bool lpop(const String& key, String* pValue) = 0;
		
		String lpop(const String& key);
		
		String lpop(const String& key, const String& def);

		virtual sl_bool rpop(const String& key, String* pValue) = 0;
		
		String rpop(const String& key);
		
		String rpop(const String& key, const String& def);
		
		virtual sl_bool lrange(const String& key, sl_int64 start, sl_int64 stop, VariantList* pValue) = 0;
		
		VariantList lrange(const String& key, sl_int64 start = 0, sl_int64 stop = -1);
		
	public:
		sl_bool isLoggingErrors();
		
		void setLoggingErrors(sl_bool flag);
		
		String getLastError();
		
	protected:
		void processError(const String& error);
		
		void clearError();
		
	protected:
		sl_bool m_flagLogErrors;
		AtomicString m_lastError;
		
	};

}

#endif
