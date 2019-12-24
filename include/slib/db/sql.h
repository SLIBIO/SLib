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

#ifndef CHECKHEADER_SLIB_DB_DATABASE_SQL
#define CHECKHEADER_SLIB_DB_DATABASE_SQL

#include "definition.h"

#include "constants.h"
#include "expression.h"

#include "../core/string_buffer.h"

namespace slib
{
	
	class SLIB_EXPORT DatabaseColumn
	{
	public:
		String name;
		DatabaseExpression expression;
		
	public:
		DatabaseColumn();
		
		DatabaseColumn(const char* name);
		
		DatabaseColumn(const String& name);
		
		DatabaseColumn(const DatabaseExpression& expression);
		
		DatabaseColumn(const String& name, const DatabaseExpression& expression);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseColumn)
		
	};

	class SLIB_EXPORT DatabaseColumnExpression : public DatabaseColumn
	{
	public:
		DatabaseColumnExpression(const DatabaseExpression& expression);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseColumnExpression)

	};

	class SLIB_EXPORT DatabaseColumnIdentifier : public DatabaseColumn
	{
	public:
		DatabaseColumnIdentifier(const char* name);
		
		DatabaseColumnIdentifier(const String& name);
		
		DatabaseColumnIdentifier(const String& name1, const String& name2);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseColumnIdentifier)

	};

	class SLIB_EXPORT DatabaseColumnTypeDefinition
	{
	public:
		DatabaseColumnType type;
		sl_uint32 length;
		sl_uint32 scale;
		String rawType;
		
	public:
		DatabaseColumnTypeDefinition(const String& rawType);
		
		DatabaseColumnTypeDefinition(DatabaseColumnType type);
		
		DatabaseColumnTypeDefinition(DatabaseColumnType type, sl_uint32 length, sl_uint32 scale = 0);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseColumnTypeDefinition)
		
	};

#define SLIB_DATABASE_COLUMN_TYPE(type, ...) slib::DatabaseColumnTypeDefinition(slib::DatabaseColumnType::type, ##__VA_ARGS__)

	class SLIB_EXPORT DatabaseReferenceDefinition
	{
	public:
		String tableName;
		ListParam<String> columns;
		DatabaseReferenceMatch match;
		DatabaseReferentialAction onDelete;
		DatabaseReferentialAction onUpdate;
		
	public:
		DatabaseReferenceDefinition();
		
		DatabaseReferenceDefinition(const String& tableName, const ListParam<String>& columns, DatabaseReferentialAction onDelete = DatabaseReferentialAction::Default, DatabaseReferentialAction onUpdate = DatabaseReferentialAction::Default, DatabaseReferenceMatch match = DatabaseReferenceMatch::Default);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseReferenceDefinition)
		
	};

	class SLIB_EXPORT DatabaseColumnDefinition
	{
	public:
		String name;
		DatabaseColumnTypeDefinition type;
		DatabaseColumnFlags flags;
		DatabaseExpression defaultValue;
		String collate;
		DatabaseReferenceDefinition reference;
		
	public:
		DatabaseColumnDefinition();
		
		DatabaseColumnDefinition(const String& name, const DatabaseColumnTypeDefinition& type, DatabaseColumnFlags flags = 0, const DatabaseExpression& defaultValue = sl_null, const String& collate = sl_null);
		
		DatabaseColumnDefinition(const String& name, const DatabaseColumnTypeDefinition& type, const DatabaseReferenceDefinition& reference, DatabaseColumnFlags flags = 0, const DatabaseExpression& defaultValue = sl_null, const String& collate = sl_null);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseColumnDefinition)
		
	};

	class SLIB_EXPORT DatabaseTableConstraint
	{
	public:
		String name;
		DatabaseConstaintType type;
		ListParam<String> columns;
		DatabaseReferenceDefinition reference;
		
	public:
		DatabaseTableConstraint();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseTableConstraint)
		
	};

	class SLIB_EXPORT DatabasePrimaryKey : public DatabaseTableConstraint
	{
	public:
		DatabasePrimaryKey(const String& name, const ListParam<String>& columns);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabasePrimaryKey)
		
	};

	class SLIB_EXPORT DatabaseUniqueKey : public DatabaseTableConstraint
	{
	public:
		DatabaseUniqueKey(const String& name, const ListParam<String>& columns);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseUniqueKey)
		
	};

	class SLIB_EXPORT DatabaseForeignKey : public DatabaseTableConstraint
	{
	public:
		DatabaseForeignKey(const String& name, const ListParam<String>& columns, const String& referTableName, const ListParam<String>& referColumns, DatabaseReferentialAction onDelete = DatabaseReferentialAction::Default, DatabaseReferentialAction onUpdate = DatabaseReferentialAction::Default, DatabaseReferenceMatch match = DatabaseReferenceMatch::Default);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseForeignKey)
		
	};

	class SLIB_EXPORT DatabaseIndexColumn
	{
	public:
		DatabaseColumn column;
		DatabaseOrderType order;
		String collate;
		
	public:
		DatabaseIndexColumn();
		
		DatabaseIndexColumn(const DatabaseColumn& column, DatabaseOrderType order = DatabaseOrderType::Default, const String& collate = sl_null);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseIndexColumn)
		
	};

	class SLIB_EXPORT DatabaseIdentifier
	{
	public:
		String schema;
		String name;
		
	public:
		DatabaseIdentifier();
		
		DatabaseIdentifier(const char* name);
		
		DatabaseIdentifier(const String& name);
		
		DatabaseIdentifier(const String& schema, const String& name);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseIdentifier)
		
	public:
		DatabaseIdentifier& operator=(const char* name);

		DatabaseIdentifier& operator=(const String& name);

	};

	class DatabaseQuery;

	class SLIB_EXPORT DatabaseQuerySource
	{
	public:
		String name;
		DatabaseIdentifier table;
		Ptr<DatabaseQuery> query;
		DatabaseExpression expression;
		
	public:
		DatabaseQuerySource();
		
		DatabaseQuerySource(const char* name);
		
		DatabaseQuerySource(const String& name);
		
		DatabaseQuerySource(const DatabaseIdentifier& table);
		
		DatabaseQuerySource(const String& name, const DatabaseIdentifier& table);
		
		DatabaseQuerySource(const String& name, const DatabaseQuery& query);
		
		DatabaseQuerySource(const String& name, const DatabaseExpression& expression);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQuerySource)
		
	};

	class SLIB_EXPORT DatabaseQueryJoin : public DatabaseQuerySource
	{
	public:
		DatabaseJoinType type;
		DatabaseExpression on;
		
	public:
		DatabaseQueryJoin();
		
		DatabaseQueryJoin(const String& name, const DatabaseExpression& on);
		
		DatabaseQueryJoin(const DatabaseIdentifier& table, const DatabaseExpression& on);
		
		DatabaseQueryJoin(const String& name, const DatabaseIdentifier& table, const DatabaseExpression& on);
		
		DatabaseQueryJoin(DatabaseJoinType type, const String& name, const DatabaseExpression& on);
		
		DatabaseQueryJoin(DatabaseJoinType type, const DatabaseIdentifier& table, const DatabaseExpression& on);
		
		DatabaseQueryJoin(DatabaseJoinType type, const String& name, const DatabaseIdentifier& table, const DatabaseExpression& on);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQueryJoin)
		
	};

	class SLIB_EXPORT DatabaseQueryGroupBy
	{
	public:
		ListParam<DatabaseColumn> columns;
		DatabaseExpression having;
		
	public:
		DatabaseQueryGroupBy();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQueryGroupBy)

	};

	class SLIB_EXPORT DatabaseQueryOrder
	{
	public:
		DatabaseColumn column;
		DatabaseOrderType type;
		String collate;
		DatabaseFlags flags; // NullsFirst, NullsLast
		
	public:
		DatabaseQueryOrder();
		
		DatabaseQueryOrder(const char* columnName);
		
		DatabaseQueryOrder(const char* columnName, DatabaseOrderType type);
		
		DatabaseQueryOrder(const String& columnName);
		
		DatabaseQueryOrder(const String& columnName, DatabaseOrderType type);
		
		DatabaseQueryOrder(const DatabaseExpression& expression);
		
		DatabaseQueryOrder(const DatabaseExpression& expression, DatabaseOrderType type);

		DatabaseQueryOrder(const DatabaseColumn& column);

		DatabaseQueryOrder(const DatabaseColumn& column, DatabaseOrderType type);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQueryOrder)
		
	};

	class DatabaseQueryCombine;

	class SLIB_EXPORT DatabaseQuery
	{
	public:
		sl_bool distinct;
		ListParam<DatabaseColumn> columns;
		DatabaseQuerySource source;
		ListParam<DatabaseQueryJoin> joins;
		DatabaseExpression where;
		DatabaseQueryGroupBy groupBy;
		ListParam<DatabaseQueryOrder> orders;
		sl_uint32 limit;
		sl_uint32 offset;
		ListParam<DatabaseQueryCombine> combines;
		ListParam<DatabaseQueryOrder> ordersOfCombined;
		sl_uint32 limitOfCombined;
		sl_uint32 offsetOfCombined;

	public:
		DatabaseQuery();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQuery)
		
	public:
		template <class... ARGS>
		void addColumn(ARGS&&... args)
		{
			columns.add(Forward<ARGS>(args)...);
		}
		
		template <class... ARGS>
		void addJoin(ARGS&&... args)
		{
			joins.add(Forward<ARGS>(args)...);
		}
		
		template <class... ARGS>
		void addCombine(ARGS&&... args)
		{
			combines.add(Forward<ARGS>(args)...);
		}
		
		template <class... ARGS>
		void addOrder(ARGS&&... args)
		{
			orders.add(Forward<ARGS>(args)...);
		}
		
		template <class... ARGS>
		void addOrderOfCombined(ARGS&&... args)
		{
			ordersOfCombined.add(Forward<ARGS>(args)...);
		}
		
	};

	class SLIB_EXPORT DatabaseQueryCombine
	{
	public:
		DatabaseCombineType type;
		DatabaseQuery query;
		
	public:
		DatabaseQueryCombine();
		
		DatabaseQueryCombine(const DatabaseQuery& query);
		
		DatabaseQueryCombine(DatabaseCombineType type, const DatabaseQuery& query);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQueryCombine)
		
	};

	class SLIB_EXPORT DatabaseQueryWith
	{
	public:
		String name;
		ListParam<String> columns;
		DatabaseQuery query;
		
	public:
		DatabaseQueryWith();
		
		DatabaseQueryWith(const String& name, const DatabaseQuery& query);
		
		DatabaseQueryWith(const String& name, const ListParam<String>& columns, const DatabaseQuery& query);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseQueryWith)
		
	};

	class SLIB_EXPORT DatabaseCreateTableParam
	{
	public:
		DatabaseIdentifier table;
		ListParam<DatabaseColumnDefinition> columns;
		DatabaseFlags flags;
		ListParam<DatabaseTableConstraint> constraints;

	public:
		DatabaseCreateTableParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseCreateTableParam)
		
	public:
		template <class... ARGS>
		void addColumn(ARGS&&... args)
		{
			columns.add(Forward<ARGS>(args)...);
		}
		
		template <class... ARGS>
		void addConstraint(ARGS&&... args)
		{
			constraints.add(Forward<ARGS>(args)...);
		}
		
	};

	class SLIB_EXPORT DatabaseCreateIndexParam
	{
	public:
		DatabaseIdentifier index;
		String table;
		ListParam<DatabaseIndexColumn> columns;
		DatabaseFlags flags;

	public:
		DatabaseCreateIndexParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseCreateIndexParam)
		
	public:
		template <class... ARGS>
		void addColumn(ARGS&&... args)
		{
			columns.add(Forward<ARGS>(args)...);
		}
		
	};

	class SLIB_EXPORT DatabaseSelectParam : public DatabaseQuery
	{
	public:
		ListParam<DatabaseQueryWith> withs;
		sl_bool withRecursive;
		
	public:
		DatabaseSelectParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DatabaseSelectParam)

	public:
		template <class... ARGS>
		void addWith(ARGS&&... args)
		{
			withs.add(Forward<ARGS>(args)...);
		}
		
	};

	class SLIB_EXPORT SqlBuilder
	{
	public:
		DatabaseDialect dialect;
		StringBuffer buf;
		List<String> parameters;
		
	public:
		SqlBuilder(DatabaseDialect dialect);
		
		~SqlBuilder();
		
		SLIB_DELETE_CLASS_DEFAULT_MEMBERS(SqlBuilder)
		
	public:
		void append(const String& str);
		
		void appendStatic(const char* sz, sl_size len);
		
		template <sl_size N>
		SLIB_INLINE void appendStatic(const char (&str)[N])
		{
			appendStatic(str, N - 1);
		}
		
		void appendParameter(const String& name);
		
		void appendParameter();
		
		String toString() const;
		
	public:
		void appendIdentifierPrefix();
		
		void appendIdentifierSuffix();
		
		void appendIdentifier(const String& name);
		
		void appendIdentifier(const char* name, sl_size length);
		
		template <sl_size N>
		SLIB_INLINE void appendIdentifier(const char (&name)[N])
		{
			appendIdentifier(name, N - 1);
		}
		
		void appendIdentifier(const DatabaseIdentifier& name);
		
		
		void generateCreateTable(const DatabaseCreateTableParam& param);

		// flags: Temp, IfNotExists, WithoutRowId (SQLite)
		void generateCreateTable(const DatabaseIdentifier& table, const ListParam<DatabaseColumnDefinition>& columns, DatabaseFlags flags = 0);

		// flags: Temp, IfExists, Cascade, Restrict
		void generateDropTable(const DatabaseIdentifier& table, DatabaseFlags flags = 0);

		
		void generateCreateIndex(const DatabaseCreateIndexParam& param);

		// flags: Unique, IfNotExists
		void generateCreateIndex(const DatabaseIdentifier& index, const String& table, const ListParam<DatabaseIndexColumn>& columns, DatabaseFlags flags = 0);

		// flags: IfExists, Cascade, Restrict
		void generateDropIndex(const DatabaseIdentifier& index, const String& table, DatabaseFlags flags = 0);
		
		
		void generateInsert(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& columns);
		
		void generateInsert(const DatabaseIdentifier& table, const ListParam<String>& columns);
		
		void generateUpdate(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& columns, const DatabaseExpression& where);
		
		void generateUpdate(const DatabaseIdentifier& table, const ListParam<String>& columns, const DatabaseExpression& where);
		
		void generateDelete(const DatabaseIdentifier& table, const DatabaseExpression& where);
		
		void generateSelect(const DatabaseSelectParam& param);
		
		void generateSelect(const DatabaseIdentifier& table, const DatabaseExpression& where);
		

		void appendColumnTypeName(const DatabaseColumnTypeDefinition& type, const DatabaseColumnFlags& flags);
		
		void appendReferentialActionString(DatabaseReferentialAction action);
		
		void appendColumn(const DatabaseColumn& column);
		
		void appendColumnNoAs(const DatabaseColumn& column);

		void appendReferenceDefinition(const DatabaseReferenceDefinition& reference);
		
		void appendColumnDefinition(const DatabaseColumnDefinition& column);
		
		void appendTableConstraint(const DatabaseTableConstraint& constraint);
		
		void appendIndexColumn(const DatabaseIndexColumn& column);
		
		void appendSelectWithClause(const DatabaseQueryWith& with);
		
		void appendQuerySource(const DatabaseQuerySource& source);
		
		void appendQueryJoin(const DatabaseQueryJoin& join);
		
		void appendQueryOrder(const DatabaseQueryOrder& order);
		
		void appendQuery(const DatabaseQuery& query);
		
	};

}

#endif
