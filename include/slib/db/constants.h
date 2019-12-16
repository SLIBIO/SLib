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

#ifndef CHECKHEADER_SLIB_DB_CONSTANTS
#define CHECKHEADER_SLIB_DB_CONSTANTS

#include "definition.h"

#include "../core/macro.h"

namespace slib
{
	
	enum class DatabaseDialect
	{
		Generic = 0,
		SQLite = 1,
		PostgreSQL = 2,
		MySQL = 3,
		MSSQL = 4,
		Oracle = 5,
	};

	enum class DatabaseColumnType
	{
		BOOLEAN,
		TINYINT,
		SMALLINT,
		MEDIUMINT,
		INTEGER,
		BIGINT,
		SMALLSERIAL,
		SERIAL,
		BIGSERIAL,
		DECIMAL,
		NUMERIC,
		FLOAT,
		REAL,
		DOUBLE,
		DATE,
		DATETIME,
		TIMESTAMP,
		TIME,
		CHAR,
		NCHAR,
		VARCHAR,
		NVARCHAR,
		BINARY,
		VARBINARY,
		TINYBLOB,
		BLOB,
		MEDIUMBLOB,
		LONGBLOB,
		TINYTEXT,
		TEXT,
		MEDIUMTEXT,
		LONGTEXT
	};

	enum class DatabaseReferentialAction
	{
		Default = 0,
		SetNull = 1,
		SetDefault = 2,
		Cascade = 3,
		Restrict = 4,
		NoAction = 5
	};

	enum class DatabaseReferenceMatch
	{
		Default = 0,
		Simple = 1,
		Full = 2,
		Partial = 3
	};

	enum class DatabaseConstaintType
	{
		PrimaryKey = 0,
		Unique = 1,
		ForeignKey = 2
	};

	enum class DatabaseJoinType
	{
		Inner = 0, // INNER, CROSS
		Left = 1, // LEFT, LEFT OUTER
		Right = 2, // RIGHT, RIGHT OUTER
		Full = 3 // FULL, FULL OUTER
	};

	enum class DatabaseOrderType
	{
		Default = 0,
		Asc = 1,
		Desc = 2
	};

	enum class DatabaseCombineType
	{
		Union = 0,
		UnionAll = 1,
		Intersect = 2,
		IntersectAll = 3,
		Except = 4,
		ExcpetAll = 5
	};

	class SLIB_EXPORT DatabaseColumnFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(DatabaseColumnFlags, value)
		
		enum
		{
			PrimaryKey = 1,
			Asc = (1 << 1),
			Desc = (1 << 2),
			AutoIncrement = (1 << 3),
			NotNull = (1 << 4),
			Unique = (1 << 5),
			Unsigned = (1 << 6),
			ZeroFill = (1 << 7),
			WithTimeZone = (1 << 8)
		};
	};

	class SLIB_EXPORT DatabaseFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(DatabaseFlags, value)
		
		enum
		{
			Temp = 1,
			IfNotExists = (1 << 1),
			IfExists = (1 << 2),
			WithoutRowId = (1 << 3),
			Cascade = (1 << 4),
			Restrict = (1 << 5),
			Unique = (1 << 6),
			NullsFirst = (1 << 7),
			NullsLast = (1  << 8)
		};
	};

}

#endif
