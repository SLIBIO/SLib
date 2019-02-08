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

#ifndef CHECKHEADER_SLIB_DB_MYSQL
#define CHECKHEADER_SLIB_DB_MYSQL

#include "database.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)
#define SLIB_DATABASE_SUPPORT_MYSQL
#endif

#if defined(SLIB_DATABASE_SUPPORT_MYSQL)

namespace slib
{
	
	class SLIB_EXPORT MySQL_Param
	{
	public:
		String host;
		sl_uint32 port;
		String user;
		String password;
		String db;

		sl_bool flagAutoReconnect;
		sl_bool flagMultipleStatements;

	public:
		MySQL_Param();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(MySQL_Param)

	};

	class SLIB_EXPORT MySQL_Database : public Database
	{
		SLIB_DECLARE_OBJECT

	protected:
		MySQL_Database();

		~MySQL_Database();
	
	public:
		static Ref<MySQL_Database> connect(const MySQL_Param& param);

		static Ref<MySQL_Database> connect(const MySQL_Param& param, String& outErrorMessage);
	
	public:
		virtual sl_bool ping() = 0;
	
	public:
		static void initThread();

	};

}

#endif

#endif
