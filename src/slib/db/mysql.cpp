/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/db/mysql.h"

#if defined(SLIB_DATABASE_SUPPORT_MYSQL)

#include "libmariadb/mysql.h"
#include "libmariadb/errmsg.h"

#include "slib/core/thread.h"
#include "slib/core/scoped.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#define TAG "MySQL"

namespace slib
{

	MySQL_Param::MySQL_Param()
	{
		port = 0;
		flagAutoReconnect = sl_true;
		flagMultipleStatements = sl_true;
	}

	MySQL_Param::~MySQL_Param()
	{
	}


	SLIB_DEFINE_OBJECT(MySQL_Database, Database)

	MySQL_Database::MySQL_Database()
	{
	}

	MySQL_Database::~MySQL_Database()
	{
	}

	class _priv_MySQL_Database_Lib
	{
	public:
		SLIB_INLINE _priv_MySQL_Database_Lib()
		{
			::mysql_library_init(0, sl_null, sl_null);
		}
	};

	class _priv_MySQL_Database_ThreadHandler : public Referable
	{
	public:
		SLIB_INLINE _priv_MySQL_Database_ThreadHandler()
		{
		}

		~_priv_MySQL_Database_ThreadHandler()
		{
			::mysql_thread_end();
		}
	};

	void MySQL_Database::initThread()
	{
		SLIB_SAFE_STATIC(_priv_MySQL_Database_Lib, lib)
		if (SLIB_SAFE_STATIC_CHECK_FREED(lib)) {
			return;
		}
#if !defined(SLIB_PLATFORM_IS_APPLE)
		static SLIB_THREAD int check = 0;
		if (check) {
			return;
		}
		check = 1;
#endif
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNotNull()) {
			Ref<Referable> ref = thread->getAttachedObject("MYSQL");
			if (ref.isNull()) {
				::mysql_thread_init();
				ref = new _priv_MySQL_Database_ThreadHandler;
				if (ref.isNotNull()) {
					thread->attachObject("MYSQL", ref.get());
				}
			}
		} else {
			::mysql_thread_init();
		}
	}

	class _priv_MySQL_Database : public MySQL_Database
	{
	public:
		MYSQL* m_mysql;

	public:
		_priv_MySQL_Database()
		{
			m_mysql = sl_null;
		}

		~_priv_MySQL_Database()
		{
			::mysql_close(m_mysql);
		}

	public:
		static Ref<_priv_MySQL_Database> connect(const MySQL_Param& param, String& outErrorMessage)
		{
			Ref<_priv_MySQL_Database> ret;

			initThread();

			MYSQL* mysql = ::mysql_init(sl_null);

			if (mysql) {

				String host = param.host;
				String user = param.user;
				String password = param.password;
				String db = param.db;

				unsigned long flags = 0;
				if (param.flagMultipleStatements) {
					flags |= CLIENT_MULTI_STATEMENTS;
				}

				if (::mysql_real_connect(mysql, host.getData(), user.getData(), password.getData(), db.getData(), param.port, sl_null, flags)) {

					::mysql_set_character_set(mysql, "utf8");
					::mysql_autocommit(mysql, 1);

					my_bool flagReconnect = param.flagAutoReconnect ? 1 : 0;
					::mysql_options(mysql, MYSQL_OPT_RECONNECT, &flagReconnect);
					my_bool flagReportTruncation = 1;
					::mysql_options(mysql, MYSQL_REPORT_DATA_TRUNCATION, &flagReportTruncation);

					ret = new _priv_MySQL_Database;
					if (ret.isNotNull()) {
						ret->m_mysql = mysql;
						return ret;
					}

				} else {
					outErrorMessage = ::mysql_error(mysql);
					LogError(TAG, outErrorMessage);
				}
				::mysql_close(mysql);
			}
			return ret;
		}

		sl_bool ping() override
		{
			initThread();
			ObjectLocker lock(this);
			if (0 == ::mysql_ping(m_mysql)) {			
				return sl_true;
			}
			return sl_false;
		}

		sl_int64 execute(const String& sql) override
		{
			initThread();
			ObjectLocker lock(this);
			if (0 == ::mysql_real_query(m_mysql, sql.getData(), (sl_uint32)(sql.getLength()))) {
				return ::mysql_affected_rows(m_mysql);
			}
			LogError(TAG, "Execute Error: %s, SQL:%s", ::mysql_error(m_mysql), sql);
			return -1;
		}

		class _priv_DatabaseCursor : public DatabaseCursor
		{
		public:
			MYSQL_RES* m_result;

			MYSQL_FIELD* m_fields;
			MYSQL_ROW m_row;
			unsigned long* m_lengths;

			CList<String> m_listColumnNames;
			sl_uint32 m_nColumnNames;
			String* m_columnNames;
			CHashMap<String, sl_int32> m_mapColumnIndexes;

			_priv_DatabaseCursor(MySQL_Database* db, MYSQL_RES* result)
			{
				m_db = db;
				m_result = result;

				sl_uint32 cols = (sl_uint32)(::mysql_num_fields(result));
				m_fields = ::mysql_fetch_fields(result);
				for (sl_uint32 i = 0; i < cols; i++) {
					m_listColumnNames.add_NoLock(m_fields[i].name);
					m_mapColumnIndexes.put_NoLock(m_fields[i].name, i);
				}
				m_nColumnNames = (sl_uint32)(m_listColumnNames.getCount());
				m_columnNames = m_listColumnNames.getData();

				m_row = sl_null;
				m_lengths = sl_null;

				db->lock();
			}

			~_priv_DatabaseCursor()
			{
				::mysql_free_result(m_result);
				m_db->unlock();
			}

			sl_uint32 getColumnsCount() override
			{
				return m_nColumnNames;
			}

			String getColumnName(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					return m_columnNames[index];
				}
				return sl_null;
			}

			sl_int32 getColumnIndex(const String& name) override
			{
				return m_mapColumnIndexes.getValue_NoLock(name, -1);
			}

			HashMap<String, Variant> getRow() override
			{
				HashMap<String, Variant> ret;
				if (m_row && m_nColumnNames > 0) {
					for (sl_uint32 index = 0; index < m_nColumnNames; index++) {
						ret.put_NoLock(m_columnNames[index], _getValue(index));
					}
				}
				return ret;
			}

			Variant _getValue(sl_uint32 index)
			{
				if (m_fields[index].type == MYSQL_TYPE_BLOB) {
					return Memory::create(m_row[index], (sl_uint32)(m_lengths[index]));
				} else {
					return String::fromUtf8(m_row[index], (sl_uint32)(m_lengths[index]));
				}
			}

			Variant getValue(sl_uint32 index) override
			{
				if (m_row) {
					if (index < m_nColumnNames) {
						return _getValue(index);
					}
				}
				return sl_null;
			}

			String getString(sl_uint32 index) override
			{
				if (m_row) {
					if (index < m_nColumnNames) {
						return String::fromUtf8(m_row[index], (sl_uint32)(m_lengths[index]));
					}
				}
				return sl_null;
			}

			Memory getBlob(sl_uint32 index) override
			{
				if (m_row) {
					if (index < m_nColumnNames) {
						if (m_lengths[index] > 0) {
							return Memory::create(m_row[index], (sl_uint32)(m_lengths[index]));
						}
					}
				}
				return sl_null;
			}

			sl_bool moveNext() override
			{
				m_row = ::mysql_fetch_row(m_result);
				m_lengths = ::mysql_fetch_lengths(m_result);
				if (m_row) {
					return sl_true;
				}
				return sl_false;
			}
		};

		Ref<DatabaseCursor> query(const String& sql) override
		{
			initThread();
			ObjectLocker lock(this);
			Ref<DatabaseCursor> ret;
			if (0 == mysql_real_query(m_mysql, sql.getData(), (sl_uint32)(sql.getLength()))) {
				MYSQL_RES* res = ::mysql_use_result(m_mysql);
				if (res) {
					ret = new _priv_DatabaseCursor(this, res);
					if (ret.isNotNull()) {
						return ret;
					}
					::mysql_free_result(res);
				}
			} else {
				LogError(TAG, "Query Error: %s, SQL:", ::mysql_error(m_mysql), sql);
			}
			return ret;
		}

#define PRIV_FIELD_DESC_BUFFER_SIZE 64
		struct _priv_FieldDesc
		{
			my_bool isNull;
			my_bool isError;
			unsigned long length;
			union {
				sl_int64 num64;
				sl_int64 unum64;
				sl_int32 num32;
				sl_uint32 unum32;
				float flt;
				double dbl;
				MYSQL_TIME time;
				char buf[PRIV_FIELD_DESC_BUFFER_SIZE];
			};
		};

		static void toMySQLTime(Time time, MYSQL_TIME& mt)
		{
			int y = time.getYear();
			if (y > 0) {
				mt.year = y;
				mt.neg = 0;
			} else {
				mt.year = -y;
				mt.neg = 1;
			}
			mt.month = time.getMonth();
			mt.day = time.getDay();
			mt.hour = time.getHour();
			mt.minute = time.getMinute();
			mt.second = time.getSecond();
			mt.second_part = 0;
			mt.time_type = MYSQL_TIMESTAMP_DATETIME;
		}

		static Time fromMySQLTime(MYSQL_TIME& mt)
		{
			int y = mt.year;
			if (mt.neg) {
				y = -y;
			}
			return Time(y, mt.month, mt.day, mt.hour, mt.minute, mt.second);
		}

		class _priv_DatabaseStatementCursor : public DatabaseCursor
		{
		public:
			Ref<DatabaseStatement> m_statementObj;
			MYSQL_STMT* m_statement;

			MYSQL_RES* m_resultMetadata;
			MYSQL_FIELD* m_fields;
			MYSQL_BIND* m_bind;
			_priv_FieldDesc* m_fds;

			CList<String> m_listColumnNames;
			sl_uint32 m_nColumnNames;
			String* m_columnNames;
			CHashMap<String, sl_int32> m_mapColumnIndexes;

			_priv_DatabaseStatementCursor(Database* db, DatabaseStatement* statementObj, MYSQL_STMT* statement, MYSQL_RES* resultMetadata, MYSQL_BIND* bind, _priv_FieldDesc* fds)
			{
				m_db = db;
				m_statementObj = statementObj;
				m_statement = statement;

				m_resultMetadata = resultMetadata;
				sl_uint32 cols = (sl_uint32)(::mysql_num_fields(resultMetadata));
				m_fields = ::mysql_fetch_fields(resultMetadata);
				m_bind = bind;
				m_fds = fds;

				for (sl_uint32 i = 0; i < cols; i++) {
					m_listColumnNames.add_NoLock(m_fields[i].name);
					m_mapColumnIndexes.put_NoLock(m_fields[i].name, i);
				}
				m_nColumnNames = (sl_uint32)(m_listColumnNames.getCount());
				m_columnNames = m_listColumnNames.getData();

				db->lock();
			}

			~_priv_DatabaseStatementCursor()
			{
				::mysql_free_result(m_resultMetadata);
				::mysql_stmt_free_result(m_statement);
				Base::freeMemory(m_fds);
				Base::freeMemory(m_bind);
				m_db->unlock();
			}

			sl_uint32 getColumnsCount() override
			{
				return m_nColumnNames;
			}

			String getColumnName(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					return m_columnNames[index];
				}
				return sl_null;
			}

			sl_int32 getColumnIndex(const String& name) override
			{
				return m_mapColumnIndexes.getValue_NoLock(name, -1);
			}

			HashMap<String, Variant> getRow() override
			{
				HashMap<String, Variant> ret;
				if (m_nColumnNames > 0) {
					for (sl_uint32 index = 0; index < m_nColumnNames; index++) {
						ret.put_NoLock(m_columnNames[index], _getValue(index));
					}
				}
				return ret;
			}

			String _getStringEx(sl_uint32 index)
			{
				String s = String::allocate((sl_uint32)(m_fds[index].length));
				if (s.isNotEmpty()) {
					MYSQL_BIND bind = m_bind[index];
					bind.buffer = s.getData();
					bind.buffer_length = (sl_uint32)(s.getLength());
					if (0 == mysql_stmt_fetch_column(m_statement, &bind, index, 0)) {
						return s;
					}
				}
				return sl_null;
			}

			Memory _getBlobEx(sl_uint32 index)
			{
				Memory mem = Memory::create(m_fds[index].length);
				if (mem.isNotNull()) {
					MYSQL_BIND bind = m_bind[index];
					bind.buffer = mem.getData();
					bind.buffer_length = (sl_uint32)(mem.getSize());
					if (0 == mysql_stmt_fetch_column(m_statement, &bind, index, 0)) {
						return mem;
					}
				}
				return sl_null;
			}

			Variant _getValue(sl_uint32 index)
			{
				enum_field_types type = m_bind[index].buffer_type;
				if (!(m_fds[index].isNull)) {
					if (m_fds[index].isError) {
						if (type == MYSQL_TYPE_STRING) {
							return _getStringEx(index);
						} else if (type == MYSQL_TYPE_BLOB) {
							return _getBlobEx(index);
						}
					} else {
						switch (type) {
						case MYSQL_TYPE_LONG:
							if (m_bind[index].is_unsigned) {
								return m_fds[index].unum32;
							} else {
								return m_fds[index].num32;
							}
							break;
						case MYSQL_TYPE_LONGLONG:
							if (m_bind[index].is_unsigned) {
								return m_fds[index].unum64;
							} else {
								return m_fds[index].num64;
							}
							break;
						case MYSQL_TYPE_FLOAT:
							return m_fds[index].flt;
						case MYSQL_TYPE_DOUBLE:
							return m_fds[index].dbl;
						case MYSQL_TYPE_DATETIME:
							return fromMySQLTime(m_fds[index].time);
						case MYSQL_TYPE_STRING:
							return String::fromUtf8(m_fds[index].buf, m_fds[index].length);
						case MYSQL_TYPE_BLOB:
							return Memory::create(m_fds[index].buf, m_fds[index].length);
						default:
							break;
						}
					}
				}
				return sl_null;
			}

			Variant getValue(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					return _getValue(index);
				}
				return sl_null;
			}

			String getString(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return String::fromUint32(m_fds[index].unum32);
								} else {
									return String::fromInt32(m_fds[index].num32);
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return String::fromUint64(m_fds[index].unum64);
								} else {
									return String::fromInt64(m_fds[index].num64);
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return String::fromFloat(m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return String::fromDouble(m_fds[index].dbl);
							case MYSQL_TYPE_DATETIME:
								return fromMySQLTime(m_fds[index].time).toString();
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length);
							default:
								break;
							}
						}
					}
				}
				return sl_null;
			}

			sl_int64 getInt64(sl_uint32 index, sl_int64 defaultValue) override
			{
				enum_field_types type = m_bind[index].buffer_type;
				if (!(m_fds[index].isNull)) {
					if (m_fds[index].isError) {
						if (type == MYSQL_TYPE_STRING) {
							return _getStringEx(index).parseInt64(10, defaultValue);
						}
					} else {
						switch (type) {
						case MYSQL_TYPE_LONG:
							if (m_bind[index].is_unsigned) {
								return m_fds[index].unum32;
							} else {
								return m_fds[index].num32;
							}
							break;
						case MYSQL_TYPE_LONGLONG:
							if (m_bind[index].is_unsigned) {
								return m_fds[index].unum64;
							} else {
								return m_fds[index].num64;
							}
							break;
						case MYSQL_TYPE_FLOAT:
							return (sl_int64)(m_fds[index].flt);
						case MYSQL_TYPE_DOUBLE:
							return (sl_int64)(m_fds[index].dbl);
						case MYSQL_TYPE_DATETIME:
							return fromMySQLTime(m_fds[index].time).toInt();
						case MYSQL_TYPE_STRING:
							return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseInt64(10, defaultValue);
						default:
							break;
						}
					}
				}
				return defaultValue;
			}

			sl_uint64 getUint64(sl_uint32 index, sl_uint64 defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index).parseUint64(10, defaultValue);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return m_fds[index].unum32;
								} else {
									return m_fds[index].num32;
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return m_fds[index].unum64;
								} else {
									return m_fds[index].num64;
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return (sl_uint64)(m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return (sl_uint64)(m_fds[index].dbl);
							case MYSQL_TYPE_DATETIME:
								return fromMySQLTime(m_fds[index].time).toInt();
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseUint64(10, defaultValue);
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			sl_int32 getInt32(sl_uint32 index, sl_int32 defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index).parseInt32(10, defaultValue);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return m_fds[index].unum32;
								} else {
									return m_fds[index].num32;
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return (sl_uint32)(m_fds[index].unum64);
								} else {
									return (sl_int32)(m_fds[index].num64);
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return (sl_int32)(m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return (sl_int32)(m_fds[index].dbl);
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseInt32(10, defaultValue);
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			sl_uint32 getUint32(sl_uint32 index, sl_uint32 defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index).parseUint32(10, defaultValue);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return m_fds[index].unum32;
								} else {
									return m_fds[index].num32;
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return (sl_uint32)(m_fds[index].unum64);
								} else {
									return (sl_int32)(m_fds[index].num64);
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return (sl_uint32)(m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return (sl_uint32)(m_fds[index].dbl);
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseUint32(10, defaultValue);
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			float getFloat(sl_uint32 index, float defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index).parseFloat(defaultValue);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return (float)(m_fds[index].unum32);
								} else {
									return (float)(m_fds[index].num32);
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return (float)(m_fds[index].unum64);
								} else {
									return (float)(m_fds[index].num64);
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return (m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return (float)(m_fds[index].dbl);
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseFloat(defaultValue);
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			double getDouble(sl_uint32 index, double defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return _getStringEx(index).parseDouble(defaultValue);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_LONG:
								if (m_bind[index].is_unsigned) {
									return (double)(m_fds[index].unum32);
								} else {
									return (double)(m_fds[index].num32);
								}
								break;
							case MYSQL_TYPE_LONGLONG:
								if (m_bind[index].is_unsigned) {
									return (double)(m_fds[index].unum64);
								} else {
									return (double)(m_fds[index].num64);
								}
								break;
							case MYSQL_TYPE_FLOAT:
								return (m_fds[index].flt);
							case MYSQL_TYPE_DOUBLE:
								return (m_fds[index].dbl);
							case MYSQL_TYPE_STRING:
								return String::fromUtf8(m_fds[index].buf, m_fds[index].length).parseDouble(defaultValue);
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			Time getTime(sl_uint32 index, const Time& defaultValue) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING) {
								return Time(_getStringEx(index));
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_DATETIME:
								return fromMySQLTime(m_fds[index].time);
							case MYSQL_TYPE_STRING:
								return Time(String::fromUtf8(m_fds[index].buf, m_fds[index].length));
							default:
								break;
							}
						}
					}
				}
				return defaultValue;
			}

			Memory getBlob(sl_uint32 index) override
			{
				if (index < m_nColumnNames) {
					enum_field_types type = m_bind[index].buffer_type;
					if (!(m_fds[index].isNull)) {
						if (m_fds[index].isError) {
							if (type == MYSQL_TYPE_STRING || type == MYSQL_TYPE_BLOB) {
								return _getBlobEx(index);
							}
						} else {
							switch (type) {
							case MYSQL_TYPE_STRING:
							case MYSQL_TYPE_BLOB:
								return Memory::create(m_fds[index].buf, m_fds[index].length);
							default:
								break;
							}
						}
					}
				}
				return sl_null;
			}

			sl_bool moveNext() override
			{
				int iRet = ::mysql_stmt_fetch(m_statement);
				if (iRet == 0 || iRet == MYSQL_DATA_TRUNCATED) {
					return sl_true;
				}
				return sl_false;
			}
			
		};

		class _priv_DatabaseStatement : public DatabaseStatement
		{
		public:
			String m_sql;
			MYSQL* m_mysql;
			MYSQL_STMT* m_statement;

		public:
			_priv_DatabaseStatement(_priv_MySQL_Database* db, const String& sql)
			{
				m_db = db;
				m_sql = sql;
				m_mysql = db->m_mysql;
				m_statement = sl_null;
			}

			~_priv_DatabaseStatement()
			{
				close();
			}

			sl_bool prepare()
			{
				ObjectLocker lock(m_db.get());
				close();
				MYSQL_STMT* statement = ::mysql_stmt_init(m_mysql);
				if (statement) {
					if (0 == ::mysql_stmt_prepare(statement, m_sql.getData(), (sl_uint32)(m_sql.getLength()))) {
						m_statement = statement;
						return sl_true;
					}
					LogError(TAG, "Prepare Error: %s, SQL:%s", ::mysql_stmt_error(statement), m_sql);
					::mysql_stmt_close(statement);
				}
				return sl_false;
			}

			void close()
			{
				ObjectLocker lock(m_db.get());
				if (m_statement) {
					::mysql_stmt_close(m_statement);
					m_statement = sl_null;
				}
			}

			sl_bool _bind(const Variant* params, sl_uint32 nParams)
			{
				sl_uint32 n = (sl_uint32)(::mysql_stmt_param_count(m_statement));
				if (n == nParams) {
					if (n > 0) {
						SLIB_SCOPED_BUFFER(MYSQL_BIND, 32, bind, n);
						SLIB_SCOPED_BUFFER(_priv_FieldDesc, 32, fds, n);
						SLIB_SCOPED_BUFFER(String, 32, strings, n);
						SLIB_SCOPED_BUFFER(Memory, 32, blobs, n);
						if (bind && fds && strings && blobs) {
							for (sl_uint32 i = 0; i < n; i++) {
								Base::zeroMemory(bind + i, sizeof(MYSQL_BIND));
								const Variant& var = params[i];
								switch (var.getType()) {
								case VariantType::Null:
									bind[i].buffer_type = MYSQL_TYPE_NULL;
									break;
								case VariantType::Boolean:
								case VariantType::Uint32:
									bind[i].buffer_type = MYSQL_TYPE_LONG;
									bind[i].buffer_length = 4;
									bind[i].is_unsigned = 1;
									bind[i].buffer = &(fds[i].unum32);
									fds[i].unum32 = var.getUint32();
									break;
								case VariantType::Int32:
									bind[i].buffer_type = MYSQL_TYPE_LONG;
									bind[i].buffer_length = 4;
									bind[i].buffer = &(fds[i].num32);
									fds[i].num32 = var.getInt32();
									break;
								case VariantType::Uint64:
									bind[i].buffer_type = MYSQL_TYPE_LONGLONG;
									bind[i].buffer_length = 8;
									bind[i].is_unsigned = 1;
									bind[i].buffer = &(fds[i].unum64);
									fds[i].unum64 = var.getUint64();
									break;
								case VariantType::Int64:
									bind[i].buffer_type = MYSQL_TYPE_LONGLONG;
									bind[i].buffer_length = 8;
									bind[i].buffer = &(fds[i].num64);
									fds[i].num64 = var.getInt64();
									break;
								case VariantType::Float:
									bind[i].buffer_type = MYSQL_TYPE_FLOAT;
									bind[i].buffer_length = 4;
									bind[i].buffer = &(fds[i].flt);
									fds[i].flt = var.getFloat();
									break;
								case VariantType::Double:
									bind[i].buffer_type = MYSQL_TYPE_DOUBLE;
									bind[i].buffer_length = 8;
									bind[i].buffer = &(fds[i].dbl);
									fds[i].dbl = var.getDouble();
									break;
								case VariantType::Time:
									bind[i].buffer_type = MYSQL_TYPE_DATETIME;
									bind[i].buffer_length = sizeof(MYSQL_TIME);
									bind[i].buffer = &(fds[i].time);
									toMySQLTime(var.getTime(), fds[i].time);
									break;
								default:
									if (var.isMemory()) {
										blobs[i] = var.getMemory();
										bind[i].buffer_type = MYSQL_TYPE_BLOB;
										bind[i].buffer_length = (sl_uint32)(blobs[i].getSize());
										bind[i].buffer = blobs[i].getData();
										bind[i].length = &(fds[i].length);
										fds[i].length = bind[i].buffer_length;
									} else {
										strings[i] = var.getString();
										bind[i].buffer_type = MYSQL_TYPE_STRING;
										bind[i].buffer_length = (sl_uint32)(strings[i].getLength());
										bind[i].buffer = strings[i].getData();
										bind[i].length = &(fds[i].length);
										fds[i].length = bind[i].buffer_length;
									}
								}
							}
							if (0 == ::mysql_stmt_bind_param(m_statement, bind)) {
								return sl_true;
							} else {
								LogError(TAG, "Bind Error: %s, SQL:%s", ::mysql_stmt_error(m_statement), m_sql);
							}
						} else {
							LogError(TAG, "Bind error: Can't create memory for parameter binding, SQL:%s", m_sql);
						}
					} else {
						return sl_true;
					}
				} else {
					LogError(TAG, "Bind error: requires %d params but %d params provided, SQL:%s", n, nParams, m_sql);
				}
				return sl_false;
			}

			sl_bool _execute(const Variant* params, sl_uint32 nParams)
			{
				if (!m_statement) {
					if (!(prepare())) {
						return sl_false;
					}
				}
				if (_bind(params, nParams)) {
					if (0 == ::mysql_stmt_execute(m_statement)) {
						return sl_true;
					} else {
						int err = ::mysql_stmt_errno(m_statement);
						if (err == CR_SERVER_LOST || err == CR_SERVER_GONE_ERROR) {
							if (prepare()) {
								if (_bind(params, nParams)) {
									if (0 == ::mysql_stmt_execute(m_statement)) {
										return sl_true;
									} else {
										LogError(TAG, "Execute Statement Error: %s, SQL:%s", ::mysql_stmt_error(m_statement), m_sql);
									}
								}
							}
						} else {
							LogError(TAG, "Execute Statement Error: %s, SQL:%s", ::mysql_stmt_error(m_statement), m_sql);
						}
					}
				}
				return sl_false;
			}

			sl_int64 executeBy(const Variant* params, sl_uint32 nParams) override
			{
				initThread();
				ObjectLocker lock(m_db.get());
				if (_execute(params, nParams)) {
					return ::mysql_stmt_affected_rows(m_statement);
				}
				return -1;
			}

			Ref<DatabaseCursor> queryBy(const Variant* params, sl_uint32 nParams) override
			{
				initThread();
				ObjectLocker lock(m_db.get());
				Ref<DatabaseCursor> ret;
				if (_execute(params, nParams)) {
					MYSQL_RES* resultMetadata = ::mysql_stmt_result_metadata(m_statement);
					if (resultMetadata) {
						sl_uint32 nFields = (sl_uint32)(::mysql_num_fields(resultMetadata));
						MYSQL_FIELD* fields = ::mysql_fetch_fields(resultMetadata);
						MYSQL_BIND* bind = (MYSQL_BIND*)(Base::createMemory(sizeof(MYSQL_BIND)*nFields));
						if (bind) {
							_priv_FieldDesc* fds = (_priv_FieldDesc*)(Base::createMemory(sizeof(_priv_FieldDesc)*nFields));
							if (fds) {
								Base::zeroMemory(bind, sizeof(MYSQL_BIND)*nFields);
								Base::zeroMemory(fds, sizeof(_priv_FieldDesc)*nFields);
								for (sl_uint32 i = 0; i < nFields; i++) {
									bind[i].length = &(fds[i].length);
									bind[i].is_null = &(fds[i].isNull);
									bind[i].error = &(fds[i].isError);
									switch (fields[i].type) {
									case MYSQL_TYPE_TINY:
									case MYSQL_TYPE_SHORT:
									case MYSQL_TYPE_INT24:
									case MYSQL_TYPE_YEAR:
									case MYSQL_TYPE_LONG:
										bind[i].buffer_type = MYSQL_TYPE_LONG;
										bind[i].buffer = &(fds[i].num32);
										bind[i].buffer_length = 4;
										if (fields[i].flags & UNSIGNED_FLAG) {
											bind[i].is_unsigned = 1;
										}
										break;
									case MYSQL_TYPE_LONGLONG:
										bind[i].buffer_type = MYSQL_TYPE_LONGLONG;
										bind[i].buffer = &(fds[i].num64);
										bind[i].buffer_length = 8;
										if (fields[i].flags & UNSIGNED_FLAG) {
											bind[i].is_unsigned = 1;
										}
										break;
									case MYSQL_TYPE_FLOAT:
										bind[i].buffer_type = MYSQL_TYPE_FLOAT;
										bind[i].buffer = &(fds[i].flt);
										bind[i].buffer_length = 4;
										break;
									case MYSQL_TYPE_DOUBLE:
										bind[i].buffer_type = MYSQL_TYPE_DOUBLE;
										bind[i].buffer = &(fds[i].dbl);
										bind[i].buffer_length = 8;
										break;
									case MYSQL_TYPE_DATE:
									case MYSQL_TYPE_TIME:
									case MYSQL_TYPE_DATETIME:
									case MYSQL_TYPE_TIMESTAMP:
									case MYSQL_TYPE_NEWDATE:
										bind[i].buffer_type = MYSQL_TYPE_DATETIME;
										bind[i].buffer = &(fds[i].time);
										bind[i].buffer_length = sizeof(MYSQL_TIME);
										break;
									case MYSQL_TYPE_TINY_BLOB:
									case MYSQL_TYPE_MEDIUM_BLOB:
									case MYSQL_TYPE_LONG_BLOB:
									case MYSQL_TYPE_BLOB:
										bind[i].buffer_type = MYSQL_TYPE_BLOB;
										bind[i].buffer = &(fds[i].buf);
										bind[i].buffer_length = PRIV_FIELD_DESC_BUFFER_SIZE;
										break;
									case MYSQL_TYPE_VARCHAR:
									case MYSQL_TYPE_VAR_STRING:
									case MYSQL_TYPE_STRING:
										bind[i].buffer_type = MYSQL_TYPE_STRING;
										bind[i].buffer = &(fds[i].buf);
										bind[i].buffer_length = PRIV_FIELD_DESC_BUFFER_SIZE;
										break;
									default:
										bind[i].buffer_type = MYSQL_TYPE_STRING;
										bind[i].buffer = &(fds[i].buf);
										bind[i].buffer_length = PRIV_FIELD_DESC_BUFFER_SIZE;
										break;
									}
								}
								if (0 == ::mysql_stmt_bind_result(m_statement, bind)) {
									ret = new _priv_DatabaseStatementCursor(m_db.get(), this, m_statement, resultMetadata, bind, fds);
									if (ret.isNotNull()) {
										return ret;
									}
								}
								Base::freeMemory(fds);
							}
							Base::freeMemory(bind);
						}
						::mysql_free_result(resultMetadata);
					}
				}
				return ret;
			}

		};

		Ref<DatabaseStatement> prepareStatement(const String& sql) override
		{
			initThread();
			ObjectLocker lock(this);
			Ref<_priv_DatabaseStatement> ret = new _priv_DatabaseStatement(this, sql);
			if (ret.isNotNull()) {
				if (ret->prepare()) {
					return ret;
				}
			}
			return sl_null;
		}

		String getErrorMessage() override
		{
			return ::mysql_error(m_mysql);
		}
	};

	Ref<MySQL_Database> MySQL_Database::connect(const MySQL_Param& param, String& outErrorMessage)
	{
		return _priv_MySQL_Database::connect(param, outErrorMessage);
	}

	Ref<MySQL_Database> MySQL_Database::connect(const MySQL_Param& param)
	{
		String err;
		return connect(param, err);
	}

}

#endif
