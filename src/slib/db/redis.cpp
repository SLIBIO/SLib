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

#include "hiredis/hiredis.h"

#include "slib/db/redis.h"

#include "slib/core/log.h"

#define TAG "Redis"

namespace slib
{

	namespace priv
	{
		namespace redis
		{

			class DatabaseImpl : public RedisDatabase
			{
			public:
				redisContext* m_context;

				DatabaseImpl()
				{
					m_context = sl_null;
				}

				~DatabaseImpl()
				{
					redisFree(m_context);
				}

				static Ref<DatabaseImpl> connect(const String& ip, sl_uint16 port)
				{
					redisContext* context = redisConnect(ip.getData(), port);
					if (context) {
						Ref<DatabaseImpl> ret = new DatabaseImpl();
						if (ret.isNotNull()) {
							ret->m_context = context;
							return ret;
						}
						redisFree(context);
					}
					return sl_null;
				}
				
				Variant _parseReply(redisReply* reply)
				{
					clearError();
					if (reply) {
						switch (reply->type) {
							case REDIS_REPLY_INTEGER:
								return reply->integer;
							case REDIS_REPLY_STRING:
							case REDIS_REPLY_STATUS:
								return String(reply->str);
							case REDIS_REPLY_ARRAY:
							{
								VariantList list;
								for (size_t i = 0; i < reply->elements; i++) {
									list.add(_parseReply(reply->element[i]));
								}
								return list;
							}
							case REDIS_REPLY_ERROR:
								processError(reply->str);
								break;
						}
					}
					return sl_null;
				}
				
				sl_bool _processReply(redisReply* reply, Variant* pValue)
				{
					if (reply) {
						if (reply->type == REDIS_REPLY_ERROR) {
							if (pValue) {
								*pValue = String(reply->str);
							}
							return sl_false;
						}
						if (pValue) {
							*pValue = _parseReply(reply);
						}
						freeReplyObject(reply);
						return sl_true;
					} else {
						processError("Cannot connect to the server");
					}
					return sl_false;
				}

				sl_bool _processCheckReply(redisReply* reply, Variant check)
				{
					Variant out;
					if (_processReply(reply, &out)) {
						return out == check;
					}
					return sl_false;
				}
				
				sl_bool _processStringReply(redisReply* reply, String* pValue)
				{
					Variant out;
					if (pValue) {
						if (_processReply(reply, &out)) {
							*pValue = out.getString();
							return sl_true;
						} else {
							*pValue = out.getString();
							return sl_false;
						}
					} else {
						return _processReply(reply, sl_null);
					}
				}
				
				sl_bool _processIntReply(redisReply* reply, sl_int64* pValue)
				{
					Variant out;
					if (pValue) {
						if (_processReply(reply, &out)) {
							*pValue = out.getInt64();
							return sl_true;
						} else {
							return sl_false;
						}
					} else {
						return _processReply(reply, sl_null);
					}
				}
				
				sl_bool _processListReply(redisReply* reply, VariantList* pValue)
				{
					Variant out;
					if (pValue) {
						if (_processReply(reply, &out)) {
							*pValue = out.getVariantList();
							return sl_true;
						} else {
							return sl_false;
						}
					} else {
						return _processReply(reply, sl_null);
					}
				}
				
				sl_bool execute(const String& command, Variant* pValue) override
				{
					ObjectLocker lock(this);
					String s = command.replaceAll("%", "%%");
					redisReply* reply = (redisReply*)(redisCommand(m_context, s.getData()));
					return _processReply(reply, pValue);
				}
				
				sl_bool set(const String& key, const Variant& value) override
				{
					ObjectLocker lock(this);
					String str = value.getString();
					redisReply* reply = (redisReply*)(redisCommand(m_context, "SET %s %s", key.getData(), str.getData()));
					return _processCheckReply(reply, "OK");
				}
				
				sl_bool get(const String& key, String* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "GET %s", key.getData()));
					return _processStringReply(reply, pValue);
				}
				
				sl_bool del(const String& key) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "DEL %s", key.getData()));
					return _processCheckReply(reply, 1);
				}
				
				sl_bool incr(const String& key, sl_int64* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "INCR %s", key.getData()));
					return _processIntReply(reply, pValue);
				}
				
				sl_bool decr(const String& key, sl_int64* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "DECR %s", key.getData()));
					return _processIntReply(reply, pValue);
				}
				
				sl_bool incrby(const String& key, sl_int64 n, sl_int64* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "INCRBY %s %lld", key.getData(), n));
					return _processIntReply(reply, pValue);
				}
				
				sl_bool decrby(const String& key, sl_int64 n, sl_int64* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "DECRBY %s %lld", key.getData(), n));
					return _processIntReply(reply, pValue);
				}
				
				sl_bool llen(const String& key, sl_int64* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LLEN %s", key.getData()));
					return _processIntReply(reply, pValue);
				}
				
				sl_int64 lpush(const String& key, const Variant& value) override
				{
					ObjectLocker lock(this);
					String str = value.getString();
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LPUSH %s %s", key.getData(), str.getData()));
					sl_int64 count = 0;
					_processIntReply(reply, &count);
					return count;
				}

				sl_int64 rpush(const String& key, const Variant& value) override
				{
					ObjectLocker lock(this);
					String str = value.getString();
					redisReply* reply = (redisReply*)(redisCommand(m_context, "RPUSH %s %s", key.getData(), str.getData()));
					sl_int64 count = 0;
					_processIntReply(reply, &count);
					return count;
				}

				sl_bool lindex(const String& key, sl_int64 index, String* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LINDEX %s %lld", key.getData(), index));
					return _processStringReply(reply, pValue);
				}
				
				sl_bool lset(const String& key, sl_int64 index, const Variant& value) override
				{
					ObjectLocker lock(this);
					String str = value.getString();
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LSET %s %lld %s", key.getData(), index, str.getData()));
					return _processCheckReply(reply, "OK");
				}
				
				sl_bool ltrm(const String& key, sl_int64 start, sl_int64 stop) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LSET %s %lld %lld", key.getData(), start, stop));
					return _processCheckReply(reply, "OK");
				}
				
				sl_bool lpop(const String& key, String* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LPOP %s", key.getData()));
					return _processStringReply(reply, pValue);
				}
				
				sl_bool rpop(const String& key, String* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "RPOP %s", key.getData()));
					return _processStringReply(reply, pValue);
				}
				
				sl_bool lrange(const String& key, sl_int64 start, sl_int64 stop, VariantList* pValue) override
				{
					ObjectLocker lock(this);
					redisReply* reply = (redisReply*)(redisCommand(m_context, "LRANGE %s %lld %lld", key.getData(), start, stop));
					return _processListReply(reply, pValue);
				}
				
			};

		}
	}
	
	SLIB_DEFINE_OBJECT(RedisDatabase, Object)

	RedisDatabase::RedisDatabase()
	{
		m_flagLogErrors = sl_false;
	}

	RedisDatabase::~RedisDatabase()
	{
	}
	
	Variant RedisDatabase::execute(const String& key)
	{
		Variant val;
		if (execute(key, &val)) {
			return val;
		}
		return sl_null;
	}
	
	String RedisDatabase::get(const String& key)
	{
		String val;
		if (get(key, &val)) {
			return val;
		}
		return sl_null;
	}
	
	String RedisDatabase::get(const String& key, const String& def)
	{
		String val;
		if (get(key, &val)) {
			return val;
		}
		return def;
	}

	sl_int64 RedisDatabase::incr(const String& key, sl_int64 def)
	{
		sl_int64 val;
		if (incr(key, &val)) {
			return val;
		}
		return def;
	}

	sl_int64 RedisDatabase::decr(const String& key, sl_int64 def)
	{
		sl_int64 val;
		if (decr(key, &val)) {
			return val;
		}
		return def;
	}

	sl_int64 RedisDatabase::incrby(const String& key, sl_int64 n, sl_int64 def)
	{
		sl_int64 val;
		if (incrby(key, n, &val)) {
			return val;
		}
		return def;
	}
	
	sl_int64 RedisDatabase::decrby(const String& key, sl_int64 n, sl_int64 def)
	{
		sl_int64 val;
		if (decrby(key, n, &val)) {
			return val;
		}
		return def;
	}

	sl_int64 RedisDatabase::llen(const String& key)
	{
		sl_int64 val;
		if (llen(key, &val)) {
			return val;
		}
		return 0;
	}
	
	String RedisDatabase::lindex(const String& key, sl_int64 index)
	{
		String val;
		if (lindex(key, index, &val)) {
			return val;
		}
		return sl_null;
	}
	
	String RedisDatabase::lindex(const String& key, sl_int64 index, const String& def)
	{
		String val;
		if (lindex(key, index, &val)) {
			return val;
		}
		return def;
	}
	
	String RedisDatabase::lpop(const String& key)
	{
		String val;
		if (lpop(key, &val)) {
			return val;
		}
		return sl_null;
	}
	
	String RedisDatabase::lpop(const String& key, const String& def)
	{
		String val;
		if (lpop(key, &val)) {
			return val;
		}
		return def;
	}
	
	String RedisDatabase::rpop(const String& key)
	{
		String val;
		if (rpop(key, &val)) {
			return val;
		}
		return sl_null;
	}
	
	String RedisDatabase::rpop(const String& key, const String& def)
	{
		String val;
		if (rpop(key, &val)) {
			return val;
		}
		return def;
	}
	
	VariantList RedisDatabase::lrange(const String& key, sl_int64 start, sl_int64 stop)
	{
		VariantList val;
		if (lrange(key, start, stop, &val)) {
			return val;
		}
		return sl_null;
	}
	
	sl_bool RedisDatabase::isLoggingErrors()
	{
		return m_flagLogErrors;
	}
	
	void RedisDatabase::setLoggingErrors(sl_bool flag)
	{
		m_flagLogErrors = flag;
	}
	
	String RedisDatabase::getLastError()
	{
		return m_lastError;
	}
	
	void RedisDatabase::processError(const String& error)
	{
		m_lastError = error;
		if (m_flagLogErrors) {
			LogError(TAG, "%s", error);
		}
	}
	
	void RedisDatabase::clearError()
	{
		m_lastError.setNull();
	}

	Ref<RedisDatabase> RedisDatabase::connect(const String& ip, sl_uint16 port)
	{
		return priv::redis::DatabaseImpl::connect(ip, port);
	}

}
