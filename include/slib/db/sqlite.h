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

#ifndef CHECKHEADER_SLIB_DB_SQLITE
#define CHECKHEADER_SLIB_DB_SQLITE

#include "database.h"

namespace slib
{

	class SLIB_EXPORT SQLiteParam
	{
	public:
		String path;
		sl_bool flagCreate;
		sl_bool flagReadonly;
		String encryptionKey;
		
	public:
		SQLiteParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SQLiteParam)
		
	};

	class SLIB_EXPORT SQLiteDatabase : public Database
	{
		SLIB_DECLARE_OBJECT

	protected:
		SQLiteDatabase();

		~SQLiteDatabase();

	public:
		static Ref<SQLiteDatabase> connect(const SQLiteParam& param);

		static Ref<SQLiteDatabase> connect(const String& filePath);

	public:
		sl_bool isTableExisting(const String& name);
		
	};

}

#endif
