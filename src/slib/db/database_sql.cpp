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

#include "slib/db/database.h"

#include "slib/core/string_buffer.h"
#include "slib/core/log.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumn)

	DatabaseColumn::DatabaseColumn()
	{
	}
	
	DatabaseColumn::DatabaseColumn(const char* _name): name(_name)
	{
	}

	DatabaseColumn::DatabaseColumn(const String& _name): name(_name)
	{
	}

	DatabaseColumn::DatabaseColumn(const DatabaseExpression& _expression): expression(_expression)
	{
	}

	DatabaseColumn::DatabaseColumn(const String& _name, const DatabaseExpression& _expression): name(_name), expression(_expression)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnExpression)

	DatabaseColumnExpression::DatabaseColumnExpression(const DatabaseExpression& expression): DatabaseColumn(expression)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnIdentifier)

	DatabaseColumnIdentifier::DatabaseColumnIdentifier(const char* name): DatabaseColumn(name)
	{
	}

	DatabaseColumnIdentifier::DatabaseColumnIdentifier(const String& name): DatabaseColumn(name)
	{
	}

	DatabaseColumnIdentifier::DatabaseColumnIdentifier(const String& name1, const String& name2): DatabaseColumn(sl_null, DatabaseExpression::column(name1, name2))
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnTypeDefinition)

	DatabaseColumnTypeDefinition::DatabaseColumnTypeDefinition(const String& _rawType): rawType(_rawType)
	{
	}

	DatabaseColumnTypeDefinition::DatabaseColumnTypeDefinition(DatabaseColumnType _type): type(_type), length(0), scale(0)
	{
	}

	DatabaseColumnTypeDefinition::DatabaseColumnTypeDefinition(DatabaseColumnType _type, sl_uint32 _length, sl_uint32 _scale): type(_type), length(_length), scale(_scale)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseReferenceDefinition)

	DatabaseReferenceDefinition::DatabaseReferenceDefinition(): onDelete(DatabaseReferentialAction::Cascade), onUpdate(DatabaseReferentialAction::Cascade)
	{
	}

	DatabaseReferenceDefinition::DatabaseReferenceDefinition(const String& _tableName, const ListParam<String>& _columns, DatabaseReferentialAction _onDelete, DatabaseReferentialAction _onUpdate, DatabaseReferenceMatch _match):
		tableName(_tableName),
		columns(_columns),
		onDelete(_onDelete),
		onUpdate(_onUpdate),
		match(_match)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnDefinition)

	DatabaseColumnDefinition::DatabaseColumnDefinition(): type(DatabaseColumnType::TEXT)
	{
	}

	DatabaseColumnDefinition::DatabaseColumnDefinition(const String& _name, const DatabaseColumnTypeDefinition& _type, DatabaseColumnFlags _flags, const DatabaseExpression& _defaultValue, const String& _collate) :
		name(_name),
		type(_type),
		flags(_flags),
		defaultValue(_defaultValue),
		collate(_collate)
	{
	}

	DatabaseColumnDefinition::DatabaseColumnDefinition(const String& _name, const DatabaseColumnTypeDefinition& _type, const DatabaseReferenceDefinition& _reference, DatabaseColumnFlags _flags, const DatabaseExpression& _defaultValue, const String& _collate) :
		name(_name),
		type(_type),
		reference(_reference),
		flags(_flags),
		defaultValue(_defaultValue),
		collate(_collate)
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseTableConstraint)

	DatabaseTableConstraint::DatabaseTableConstraint(): type(DatabaseConstaintType::Unique)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabasePrimaryKey)

	DatabasePrimaryKey::DatabasePrimaryKey(const String& _name, const ListParam<String>& _columns)
	{
		type = DatabaseConstaintType::PrimaryKey;
		name = _name;
		columns = _columns;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseUniqueKey)

	DatabaseUniqueKey::DatabaseUniqueKey(const String& _name, const ListParam<String>& _columns)
	{
		type = DatabaseConstaintType::Unique;
		name = _name;
		columns = _columns;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseForeignKey)

	DatabaseForeignKey::DatabaseForeignKey(const String& _name, const ListParam<String>& _columns, const String& referTableName, const ListParam<String>& referColumns, DatabaseReferentialAction onDelete, DatabaseReferentialAction onUpdate, DatabaseReferenceMatch match)
	{
		type = DatabaseConstaintType::ForeignKey;
		name = _name;
		columns = _columns;
		reference.tableName = referTableName;
		reference.columns = referColumns;
		reference.onDelete = onDelete;
		reference.onUpdate = onUpdate;
		reference.match = match;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseIndexColumn)

	DatabaseIndexColumn::DatabaseIndexColumn(): column(sl_null)
	{
	}

	DatabaseIndexColumn::DatabaseIndexColumn(const DatabaseColumn& _column, DatabaseOrderType _order, const String& _collate): column(_column), order(_order), collate(_collate)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseIdentifier)

	DatabaseIdentifier::DatabaseIdentifier()
	{
	}

	DatabaseIdentifier::DatabaseIdentifier(const char* _name): name(_name)
	{
	}

	DatabaseIdentifier::DatabaseIdentifier(const String& _name): name(_name)
	{
	}

	DatabaseIdentifier::DatabaseIdentifier(const String& _schema, const String& _name): schema(_schema), name(_name)
	{
	}

	DatabaseIdentifier& DatabaseIdentifier::operator=(const char* _name)
	{
		schema.setNull();
		name = _name;
		return *this;
	}

	DatabaseIdentifier& DatabaseIdentifier::operator=(const String& _name)
	{
		schema.setNull();
		name = _name;
		return *this;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQuerySource)

	DatabaseQuerySource::DatabaseQuerySource()
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const char* _name): name(_name)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const String& _name): name(_name)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const DatabaseIdentifier& _table): table(_table)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const String& _name, const DatabaseIdentifier& _table): name(_name), table(_table)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const String& _name, const DatabaseQuery& _query): name(_name), query(MakeShared<DatabaseQuery>(_query))
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const String& _name, const DatabaseExpression& _expression): name(_name), expression(_expression)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQueryJoin)

	DatabaseQueryJoin::DatabaseQueryJoin(): type(DatabaseJoinType::Inner)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(const String& name, const DatabaseExpression& _on): DatabaseQuerySource(name), type(DatabaseJoinType::Inner), on(_on)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(const DatabaseIdentifier& table, const DatabaseExpression& _on): DatabaseQuerySource(table), type(DatabaseJoinType::Inner), on(_on)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(const String& name, const DatabaseIdentifier& table, const DatabaseExpression& _on): DatabaseQuerySource(name, table), type(DatabaseJoinType::Inner), on(_on)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(DatabaseJoinType _type, const String& name, const DatabaseExpression& _on): DatabaseQuerySource(name), type(_type), on(_on)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(DatabaseJoinType _type, const DatabaseIdentifier& table, const DatabaseExpression& _on): DatabaseQuerySource(table), type(_type), on(_on)
	{
	}

	DatabaseQueryJoin::DatabaseQueryJoin(DatabaseJoinType _type, const String& name, const DatabaseIdentifier& table, const DatabaseExpression& _on): DatabaseQuerySource(name, table), type(_type), on(_on)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQueryGroupBy)

	DatabaseQueryGroupBy::DatabaseQueryGroupBy()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQueryOrder)

	DatabaseQueryOrder::DatabaseQueryOrder(): type(DatabaseOrderType::Default)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const char* columnName): column(columnName), type(DatabaseOrderType::Default)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const char* columnName, DatabaseOrderType _type): column(columnName), type(_type)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const String& columnName): column(columnName), type(DatabaseOrderType::Default)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const String& columnName, DatabaseOrderType _type): column(columnName), type(_type)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const DatabaseExpression& expression): column(expression), type(DatabaseOrderType::Default)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const DatabaseExpression& expression, DatabaseOrderType _type): column(expression), type(_type)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const DatabaseColumn& _column): column(_column), type(DatabaseOrderType::Default)
	{
	}

	DatabaseQueryOrder::DatabaseQueryOrder(const DatabaseColumn& _column, DatabaseOrderType _type): column(_column), type(_type)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQuery)

	DatabaseQuery::DatabaseQuery()
	{
		distinct = sl_false;
		limit = 0;
		offset = 0;
		limitOfCombined = 0;
		offsetOfCombined = 0;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQueryWith)

	DatabaseQueryWith::DatabaseQueryWith()
	{
	}

	DatabaseQueryWith::DatabaseQueryWith(const String& _name, const DatabaseQuery& _query): name(_name), query(_query)
	{
	}

	DatabaseQueryWith::DatabaseQueryWith(const String& _name, const ListParam<String>& _columns, const DatabaseQuery& _query): name(_name), columns(_columns), query(_query)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseQueryCombine)

	DatabaseQueryCombine::DatabaseQueryCombine(): type(DatabaseCombineType::UnionAll)
	{
	}

	DatabaseQueryCombine::DatabaseQueryCombine(const DatabaseQuery& _query): type(DatabaseCombineType::UnionAll), query(_query)
	{
		
	}

	DatabaseQueryCombine::DatabaseQueryCombine(DatabaseCombineType _type, const DatabaseQuery& _query): type(_type), query(_query)
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseCreateTableParam)

	DatabaseCreateTableParam::DatabaseCreateTableParam()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseCreateIndexParam)

	DatabaseCreateIndexParam::DatabaseCreateIndexParam()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseSelectParam)

	DatabaseSelectParam::DatabaseSelectParam()
	{
		withRecursive = sl_false;
	}


	SqlBuilder::SqlBuilder(DatabaseDialect _dialect): dialect(_dialect)
	{
	}

	SqlBuilder::~SqlBuilder()
	{
	}

	void SqlBuilder::append(const String& str)
	{
		buf.add(str);
	}

	void SqlBuilder::appendStatic(const char* sz, sl_size len)
	{
		buf.addStatic(sz, len);
	}

	void SqlBuilder::appendParameter(const String& name)
	{
		appendStatic("?", 1);
		parameters.add_NoLock(name);
	}

	String SqlBuilder::toString() const
	{
		return buf.merge();
	}

	void SqlBuilder::appendIdentifierPrefix()
	{
		switch (dialect) {
			case DatabaseDialect::MySQL:
				appendStatic("`", 1);
				break;
			case DatabaseDialect::MSSQL:
				appendStatic("[", 1);
				break;
			default:
				appendStatic("\"", 1);
				break;
		}
	}

	void SqlBuilder::appendIdentifierSuffix()
	{
		switch (dialect) {
			case DatabaseDialect::MySQL:
				appendStatic("`", 1);
				break;
			case DatabaseDialect::MSSQL:
				appendStatic("]", 1);
				break;
			default:
				appendStatic("\"", 1);
				break;
		}
	}
	
	void SqlBuilder::appendIdentifier(const String& name)
	{
		appendIdentifierPrefix();
		append(name);
		appendIdentifierSuffix();
	}

	void SqlBuilder::appendIdentifier(const char* name, sl_size len)
	{
		appendIdentifierPrefix();
		appendStatic(name, len);
		appendIdentifierSuffix();
	}

	void SqlBuilder::appendIdentifier(const DatabaseIdentifier& identifier)
	{
		if (identifier.schema.isNotEmpty()) {
			appendIdentifierPrefix();
			append(identifier.schema);
			appendIdentifierSuffix();
			appendStatic(".", 1);
		}
		if (identifier.name.equals("*")) {
			appendStatic("*", 1);
		} else {
			appendIdentifierPrefix();
			append(identifier.name);
			appendIdentifierSuffix();
		}
	}

	void SqlBuilder::generateCreateTable(const DatabaseCreateTableParam& param)
	{
		ListLocker<DatabaseColumnDefinition> columns(param.columns);
		if (param.table.name.isEmpty() || !(columns.count)) {
			return;
		}
		appendStatic("CREATE", 6);
		if (param.flags & DatabaseFlags::Temp) {
			appendStatic(" TEMPORARY", 10);
		}
		appendStatic(" TABLE ", 7);
		if (param.flags & DatabaseFlags::IfNotExists) {
			appendStatic("IF NOT EXISTS ", 14);
		}
		appendIdentifier(param.table);
		appendStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendColumnDefinition(columns[i]);
			}
		}
		{
			ListLocker<DatabaseTableConstraint> constraints(param.constraints);
			for (sl_size i = 0; i < constraints.count; i++) {
				appendStatic(", ", 2);
				appendTableConstraint(constraints[i]);
			}
		}
		appendStatic(")", 1);
		if (param.flags & DatabaseFlags::WithoutRowId) {
			if (dialect == DatabaseDialect::SQLite) {
				appendStatic(" WITHOUT ROWID", 14);
			}
		}
	}

	void SqlBuilder::generateCreateTable(const DatabaseIdentifier& table, const ListParam<DatabaseColumnDefinition>& columns, DatabaseFlags flags)
	{
		DatabaseCreateTableParam param;
		param.table = table;
		param.columns = columns;
		param.flags = flags;
		generateCreateTable(param);
	}

	void SqlBuilder::generateDropTable(const DatabaseIdentifier& table, DatabaseFlags flags)
	{
		if ((flags & DatabaseFlags::Temp) && dialect == DatabaseDialect::MySQL) {
			appendStatic("DROP TEMPORARY TABLE ", 21);
		} else {
			appendStatic("DROP TABLE ", 11);
		}
		if (flags & DatabaseFlags::IfExists) {
			appendStatic("IF EXISTS ", 10);
		}
		appendIdentifier(table);
		if (dialect != DatabaseDialect::SQLite) {
			if (flags & DatabaseFlags::Cascade) {
				appendStatic(" CASCADE", 8);
			} else if (flags & DatabaseFlags::Restrict) {
				appendStatic(" RESTRICT", 9);
			}
		}
	}

	void SqlBuilder::generateCreateIndex(const DatabaseCreateIndexParam& param)
	{
		ListLocker<DatabaseIndexColumn> columns(param.columns);
		if (param.index.name.isEmpty() || param.table.isEmpty() || !(columns.count)) {
			return;
		}
		appendStatic("CREATE", 6);
		if (param.flags & DatabaseFlags::Unique) {
			appendStatic(" UNIQUE", 7);
		}
		appendStatic(" INDEX ", 7);
		if (param.flags & DatabaseFlags::IfNotExists) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic("IF NOT EXISTS ", 14);
			}
		}
		appendIdentifier(param.index);
		appendStatic(" ON ", 4);
		appendIdentifier(param.table);
		appendStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIndexColumn(columns[i]);
			}
		}
		appendStatic(")", 1);
	}

	void SqlBuilder::generateCreateIndex(const DatabaseIdentifier& index, const String& table, const ListParam<DatabaseIndexColumn>& columns, DatabaseFlags flags)
	{
		DatabaseCreateIndexParam param;
		param.index = index;
		param.table = table;
		param.columns = columns;
		param.flags = flags;
		generateCreateIndex(param);
	}

	void SqlBuilder::generateDropIndex(const DatabaseIdentifier& index, const String& table, DatabaseFlags flags)
	{
		appendStatic("DROP INDEX ", 11);
		if (flags & DatabaseFlags::IfExists) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic("IF EXISTS ", 10);
			}
		}
		appendIdentifier(index);
		if (dialect == DatabaseDialect::MySQL) {
			appendStatic(" ON ", 4);
			appendIdentifier(table);
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseFlags::Cascade) {
				appendStatic(" CASCADE", 8);
			} else if (flags & DatabaseFlags::Restrict) {
				appendStatic(" RESTRICT", 9);
			}
		}
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("INSERT INTO ", 12);
		appendIdentifier(table);
		appendStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i].name);
			}
		}
		appendStatic(") VALUES (", 10);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					appendStatic("?", 1);
				}
			}
		}
		appendStatic(")", 1);
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<String>& _columns)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("INSERT INTO ", 12);
		appendIdentifier(table);
		appendStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
			}
		}
		appendStatic(") VALUES (", 10);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendStatic("?", 1);
			}
		}
		appendStatic(")", 1);
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns, const DatabaseExpression& where)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("UPDATE ", 7);
		appendIdentifier(table);
		appendStatic(" SET ", 5);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i].name);
				appendStatic("=", 1);
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					appendStatic("?", 1);
				}
			}
		}
		if (where.isNotNull()) {
			appendStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<String>& _columns, const DatabaseExpression& where)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("UPDATE ", 7);
		appendIdentifier(table);
		appendStatic(" SET ", 5);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
				appendStatic("=?", 2);
			}
		}
		if (where.isNotNull()) {
			appendStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateDelete(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		appendStatic("DELETE FROM ", 12);
		appendIdentifier(table);
		if (where.isNotNull()) {
			appendStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateSelect(const DatabaseSelectParam& param)
	{
		ListElements<DatabaseQueryWith> withs(param.withs);
		if (withs.count) {
			appendStatic("WITH ", 5);
			if (param.withRecursive) {
				appendStatic("RECURSIVE ", 10);
			}
			for (sl_size i = 0; i < withs.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendSelectWithClause(withs[i]);
			}
			appendStatic(" ", 1);
		}
		appendQuery(param);
	}
	
	void SqlBuilder::generateSelect(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		appendStatic("SELECT * FROM ", 14);
		appendIdentifier(table);
		if (where.isNotNull()) {
			appendStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

#define DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH(TYPE) \
	if (type.length) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		flagAppendLength = sl_true; \
		break; \
	} else { \
		static char s[] = TYPE; \
		appendStatic(s, sizeof(s)-1); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE(TYPE) \
	if (type.length) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		flagAppendLength = sl_true; \
		flagAppendScale = sl_true; \
		break; \
	} else { \
		static char s[] = TYPE; \
		appendStatic(s, sizeof(s)-1); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_RETURN_MYSQL(TYPE) \
	if (flags & (DatabaseColumnFlags::Unsigned | DatabaseColumnFlags::ZeroFill)) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		break; \
	} else { \
		static char s[] = TYPE; \
		appendStatic(s, sizeof(s)-1); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_MYSQL(TYPE) \
	if (type.length || (flags & (DatabaseColumnFlags::Unsigned | DatabaseColumnFlags::ZeroFill))) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		if (type.length) { \
			flagAppendLength = sl_true; \
		} \
		break; \
	} else { \
		static char s[] = TYPE; \
		appendStatic(s, sizeof(s)-1); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE_MYSQL(TYPE) \
	if (type.length || (flags & (DatabaseColumnFlags::Unsigned | DatabaseColumnFlags::ZeroFill))) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		if (type.length) { \
			flagAppendLength = sl_true; \
			flagAppendScale = sl_true; \
		} \
		break; \
	} else { \
		static char s[] = TYPE; \
		appendStatic(s, sizeof(s)-1); \
		return; \
	}

	void SqlBuilder::appendColumnTypeName(const DatabaseColumnTypeDefinition& type, const DatabaseColumnFlags& flags)
	{
		if (type.rawType.isNotEmpty()) {
			append(type.rawType);
			return;
		}
		const String* pStrType = sl_null;
		sl_bool flagAppendLength = sl_false;
		sl_bool flagAppendScale = sl_false;
		if (dialect == DatabaseDialect::SQLite) {
			switch (type.type) {
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLINT:
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::INTEGER:
				case DatabaseColumnType::BIGINT:
				case DatabaseColumnType::SMALLSERIAL:
				case DatabaseColumnType::SERIAL:
				case DatabaseColumnType::BIGSERIAL:
					appendStatic("INTEGER", 7);
					return;
				case DatabaseColumnType::BOOLEAN:
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
				case DatabaseColumnType::DATE:
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIME:
				case DatabaseColumnType::TIMESTAMP:
					appendStatic("NUMERIC", 7);
					return;
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					appendStatic("REAL", 4);
					return;
				case DatabaseColumnType::CHAR:
				case DatabaseColumnType::NCHAR:
				case DatabaseColumnType::VARCHAR:
				case DatabaseColumnType::NVARCHAR:
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("TEXT", 4);
					return;
				case DatabaseColumnType::BINARY:
				case DatabaseColumnType::VARBINARY:
				case DatabaseColumnType::TINYBLOB:
				case DatabaseColumnType::BLOB:
				case DatabaseColumnType::MEDIUMBLOB:
				case DatabaseColumnType::LONGBLOB:
					appendStatic("BLOB", 4);
					return;
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("boolean", 7);
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("smallserial", 11);
					} else {
						appendStatic("smallint", 8);
					}
					return;
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::INTEGER:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("serial", 6);
					} else {
						appendStatic("integer", 7);
					}
					return;
				case DatabaseColumnType::BIGINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("bigserial", 9);
					} else {
						appendStatic("bigint", 6);
					}
					return;
				case DatabaseColumnType::SMALLSERIAL:
					appendStatic("smallserial", 11);
					return;
				case DatabaseColumnType::SERIAL:
					appendStatic("serial", 6);
					return;
				case DatabaseColumnType::BIGSERIAL:
					appendStatic("bigserial", 9);
					return;
				case DatabaseColumnType::DECIMAL:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("decimal")
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("numeric")
				case DatabaseColumnType::FLOAT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("float")
				case DatabaseColumnType::REAL:
					appendStatic("real", 4);
					return;
				case DatabaseColumnType::DOUBLE:
					appendStatic("double precision", 16);
					return;
				case DatabaseColumnType::DATE:
					appendStatic("date", 4);
					return;
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIMESTAMP:
					if (type.length || (flags & DatabaseColumnFlags::WithTimeZone)) {
						SLIB_STATIC_STRING(s, "timestamp")
						pStrType = &s;
						if (type.length) {
							flagAppendLength = sl_true;
						}
						break;
					} else {
						appendStatic("timestamp", 9);
						return;
					}
				case DatabaseColumnType::TIME:
					if (type.length || (flags & DatabaseColumnFlags::WithTimeZone)) {
						SLIB_STATIC_STRING(s, "time")
						pStrType = &s;
						if (type.length) {
							flagAppendLength = sl_true;
						}
						break;
					} else {
						appendStatic("time", 4);
						return;
					}
				case DatabaseColumnType::CHAR:
				case DatabaseColumnType::NCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("character")
				case DatabaseColumnType::VARCHAR:
				case DatabaseColumnType::NVARCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("varchar")
				case DatabaseColumnType::BINARY:
				case DatabaseColumnType::VARBINARY:
				case DatabaseColumnType::TINYBLOB:
				case DatabaseColumnType::BLOB:
				case DatabaseColumnType::MEDIUMBLOB:
				case DatabaseColumnType::LONGBLOB:
					appendStatic("bytea", 5);
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("text", 4);
					return;
			}
		} else if (dialect == DatabaseDialect::MySQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("BOOLEAN", 7);
					return;
				case DatabaseColumnType::TINYINT:
					DEFINE_COLUMN_TYPE_RETURN_MYSQL("TINYINT")
				case DatabaseColumnType::SMALLSERIAL:
				case DatabaseColumnType::SMALLINT:
					DEFINE_COLUMN_TYPE_RETURN_MYSQL("SMALLINT")
				case DatabaseColumnType::MEDIUMINT:
					DEFINE_COLUMN_TYPE_RETURN_MYSQL("MEDIUMINT")
				case DatabaseColumnType::SERIAL:
					DEFINE_COLUMN_TYPE_RETURN_MYSQL("INT")
				case DatabaseColumnType::INTEGER:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_MYSQL("INT")
				case DatabaseColumnType::BIGSERIAL:
				case DatabaseColumnType::BIGINT:
					DEFINE_COLUMN_TYPE_RETURN_MYSQL("BIGINT")
				case DatabaseColumnType::DECIMAL:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE_MYSQL("DECIMAL")
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE_MYSQL("NUMERIC")
				case DatabaseColumnType::FLOAT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE_MYSQL("FLOAT")
				case DatabaseColumnType::REAL:
					appendStatic("REAL", 4);
					return;
				case DatabaseColumnType::DOUBLE:
					appendStatic("DOUBLE", 6);
					return;
				case DatabaseColumnType::DATE:
					appendStatic("DATE", 4);
					return;
				case DatabaseColumnType::DATETIME:
					appendStatic("DATETIME", 8);
					return;
				case DatabaseColumnType::TIMESTAMP:
					appendStatic("TIMESTAMP", 9);
					return;
				case DatabaseColumnType::TIME:
					appendStatic("TIME", 4);
					return;
				case DatabaseColumnType::CHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("CHARACTER")
				case DatabaseColumnType::NCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("NCHAR")
				case DatabaseColumnType::VARCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("VARCHAR")
				case DatabaseColumnType::NVARCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("NVARCHAR")
				case DatabaseColumnType::BINARY:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("BINARY")
				case DatabaseColumnType::VARBINARY:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("VARBINARY")
				case DatabaseColumnType::TINYBLOB:
					appendStatic("TINYBLOB", 8);
					return;
				case DatabaseColumnType::BLOB:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("BLOB")
				case DatabaseColumnType::MEDIUMBLOB:
					appendStatic("MEDIUMBLOB", 10);
					return;
				case DatabaseColumnType::LONGBLOB:
					appendStatic("LONGBLOB", 8);
					return;
				case DatabaseColumnType::TINYTEXT:
					appendStatic("TINYTEXT", 8);
					return;
				case DatabaseColumnType::TEXT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("TEXT")
				case DatabaseColumnType::MEDIUMTEXT:
					appendStatic("MEDIUMTEXT", 10);
					return;
				case DatabaseColumnType::LONGTEXT:
					appendStatic("LONGTEXT", 8);
					return;
			}
		} else {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("BOOLEAN", 7);
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLSERIAL:
				case DatabaseColumnType::SMALLINT:
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::SERIAL:
				case DatabaseColumnType::INTEGER:
					appendStatic("INT", 3);
					return;
				case DatabaseColumnType::BIGSERIAL:
				case DatabaseColumnType::BIGINT:
					appendStatic("BIGINT", 6);
					return;
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("NUMERIC")
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					appendStatic("DOUBLE", 6);
					return;
				case DatabaseColumnType::DATE:
					appendStatic("DATE", 4);
					return;
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIMESTAMP:
				case DatabaseColumnType::TIME:
					appendStatic("DATETIME", 8);
					return;
				case DatabaseColumnType::CHAR:
				case DatabaseColumnType::NCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("CHARACTER")
				case DatabaseColumnType::VARCHAR:
				case DatabaseColumnType::NVARCHAR:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("VARCHAR")
				case DatabaseColumnType::BINARY:
				case DatabaseColumnType::VARBINARY:
				case DatabaseColumnType::TINYBLOB:
				case DatabaseColumnType::BLOB:
				case DatabaseColumnType::MEDIUMBLOB:
				case DatabaseColumnType::LONGBLOB:
					appendStatic("BLOB", 4);
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("TEXT", 4);
					return;
			}
		}
		append(*pStrType);
		if (flagAppendLength) {
			appendStatic("(", 1);
			append(String::fromUint32(type.length));
			if (flagAppendScale && type.scale) {
				appendStatic(",", 1);
				append(String::fromUint32(type.scale));
			}
			appendStatic(")", 1);
		}
		if (dialect == DatabaseDialect::MySQL) {
			if (flags & DatabaseColumnFlags::Unsigned) {
				appendStatic(" UNSIGNED", 9);
			}
			if (flags & DatabaseColumnFlags::ZeroFill) {
				appendStatic(" ZEROFILL", 9);
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseColumnFlags::WithTimeZone) {
				appendStatic(" with time zone", 15);
			}
		}
	}

	void SqlBuilder::appendReferentialActionString(DatabaseReferentialAction action)
	{
		switch (action) {
			case DatabaseReferentialAction::Default:
				return;
			case DatabaseReferentialAction::SetNull:
				appendStatic("SET NULL", 8);
				return;
			case DatabaseReferentialAction::SetDefault:
				appendStatic("SET DEFAULT", 11);
				return;
			case DatabaseReferentialAction::Cascade:
				appendStatic("CASCADE", 7);
				return;
			case DatabaseReferentialAction::Restrict:
				appendStatic("RESTRICT", 8);
				return;
			case DatabaseReferentialAction::NoAction:
				appendStatic("NO ACTION", 9);
				return;
		}
	}

	void SqlBuilder::appendColumn(const DatabaseColumn& column)
	{
		if (column.expression.isNotNull()) {
			column.expression.appendTo(*this, sl_true);
			if (column.name.isNotEmpty()) {
				appendStatic(" ", 1);
				appendIdentifier(column.name);
			}
		} else {
			if (column.name.equals("*")) {
				appendStatic("*", 1);
			} else {
				appendIdentifier(column.name);
			}
		}
	}

	void SqlBuilder::appendColumnNoAs(const DatabaseColumn& column)
	{
		if (column.expression.isNotNull()) {
			column.expression.appendTo(*this, sl_true);
		} else {
			appendIdentifier(column.name);
		}
	}

	void SqlBuilder::appendReferenceDefinition(const DatabaseReferenceDefinition& reference)
	{
		appendStatic("REFERENCES ", 11);
		appendIdentifier(reference.tableName);
		ListLocker<String> columns(reference.columns);
		if (columns.count > 0) {
			appendStatic(" (", 2);
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
			}
			appendStatic(")", 1);
		}
		switch (reference.match) {
			case DatabaseReferenceMatch::Simple:
				appendStatic(" MATCH SIMPLE", 13);
				break;
			case DatabaseReferenceMatch::Full:
				appendStatic(" MATCH FULL", 11);
				break;
			case DatabaseReferenceMatch::Partial:
				appendStatic(" MATCH PARTIAL", 14);
				break;
			case DatabaseReferenceMatch::Default:
				break;
		}
		if (reference.onDelete != DatabaseReferentialAction::Default) {
			appendStatic(" ", 1);
			appendReferentialActionString(reference.onDelete);
		}
		if (reference.onUpdate != DatabaseReferentialAction::Default) {
			appendStatic(" ", 1);
			appendReferentialActionString(reference.onUpdate);
		}
	}

	void SqlBuilder::appendColumnDefinition(const DatabaseColumnDefinition& column)
	{
		appendIdentifier(column.name);
		appendStatic(" ", 1);
		appendColumnTypeName(column.type, column.flags);
		if (column.collate.isNotEmpty()) {
			appendStatic(" COLLATE ", 9);
			append(column.collate);
		}
		if (column.flags & DatabaseColumnFlags::NotNull) {
			appendStatic(" NOT NULL", 9);
		}
		if (column.defaultValue.isNotNull()) {
			appendStatic(" DEFAULT ", 9);
			column.defaultValue.appendTo(*this, sl_true);
		}
		if (column.flags & DatabaseColumnFlags::AutoIncrement) {
			if (dialect == DatabaseDialect::MySQL) {
				appendStatic(" AUTO_INCREMENT", 15);
			}
		}
		if (column.flags & DatabaseColumnFlags::PrimaryKey) {
			appendStatic(" PRIMARY KEY", 12);
			if (dialect == DatabaseDialect::SQLite) {
				if (column.flags & DatabaseColumnFlags::Asc) {
					appendStatic(" ASC", 4);
				} else if (column.flags & DatabaseColumnFlags::Desc) {
					appendStatic(" DESC", 5);
				}
				if (column.flags & DatabaseColumnFlags::AutoIncrement) {
					appendStatic(" AUTOINCREMENT", 14);
				}
			}
		}
		if (column.flags & DatabaseColumnFlags::Unique) {
			appendStatic(" UNIQUE", 7);
		}
		if (column.reference.tableName.isNotEmpty()) {
			appendStatic(" ", 1);
			appendReferenceDefinition(column.reference);
		}
	}

	void SqlBuilder::appendTableConstraint(const DatabaseTableConstraint& constraint)
	{
		appendStatic("CONSTRAINT ", 11);
		append(constraint.name);
		appendStatic(" ", 1);
		switch (constraint.type) {
			case DatabaseConstaintType::PrimaryKey:
				appendStatic(" PRIMARY KEY", 12);
				break;
			case DatabaseConstaintType::Unique:
				appendStatic(" UNIQUE", 7);
				break;
			case DatabaseConstaintType::ForeignKey:
				appendStatic(" FOREIGN KEY", 12);
				break;
		}
		appendStatic(" (", 2);
		ListLocker<String> columns(constraint.columns);
		for (sl_size i = 0; i < columns.count; i++) {
			if (i) {
				appendStatic(", ", 2);
			}
			appendIdentifier(columns[i]);
		}
		appendStatic(")", 1);
		if (constraint.type == DatabaseConstaintType::ForeignKey) {
			appendStatic(" ", 1);
			appendReferenceDefinition(constraint.reference);
		}
	}

	void SqlBuilder::appendIndexColumn(const DatabaseIndexColumn& column)
	{
		appendColumnNoAs(column.column);
		if (column.collate.isNotEmpty()) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic(" COLLATE ", 9);
				append(column.collate);
			}
		}
		if (column.order == DatabaseOrderType::Asc) {
			appendStatic(" ASC", 4);
		} else if (column.order == DatabaseOrderType::Desc) {
			appendStatic(" DESC", 5);
		}
	}

	void SqlBuilder::appendSelectWithClause(const DatabaseQueryWith& with)
	{
		ListElements<String> columns(with.columns);
		appendIdentifier(with.name);
		if (columns.count) {
			appendStatic("(", 1);
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
			}
			appendStatic(")", 1);
		}
		appendStatic(" AS (", 5);
		appendQuery(with.query);
		appendStatic(")", 1);
	}

	void SqlBuilder::appendQuerySource(const DatabaseQuerySource& source)
	{
		if (source.query.isNotNull()) {
			appendStatic("(", 1);
			appendQuery(*(source.query));
			appendStatic(")", 1);
		} else if (source.expression.isNotNull()) {
			source.expression.appendTo(*this, sl_true);
		} else {
			appendIdentifier(source.table);
		}
		if (source.name.isNotEmpty()) {
			appendStatic(" ", 1);
			appendIdentifier(source.name);
		}
	}

	void SqlBuilder::appendQueryJoin(const DatabaseQueryJoin& join)
	{
		switch (join.type) {
			case DatabaseJoinType::Left:
				appendStatic("LEFT JOIN ", 10);
				break;
			case DatabaseJoinType::Right:
				appendStatic("RIGHT JOIN ", 11);
				break;
			case DatabaseJoinType::Inner:
				appendStatic("INNER JOIN ", 11);
				break;
			case DatabaseJoinType::Full:
				appendStatic("FULL JOIN ", 10);
				break;
			default:
				return;
		}
		appendQuerySource(join);
		if (join.on.isNotNull()) {
			appendStatic(" ", 1);
			join.on.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::appendQueryOrder(const DatabaseQueryOrder& order)
	{
		appendColumnNoAs(order.column);
		if (order.collate.isNotEmpty()) {
			appendStatic(" COLLATE ", 9);
			append(order.collate);
		}
		if (order.type == DatabaseOrderType::Asc) {
			appendStatic(" ASC", 4);
		} else if (order.type == DatabaseOrderType::Desc) {
			appendStatic(" DESC", 5);
		}
		if (order.flags & DatabaseFlags::NullsFirst) {
			appendStatic(" NULLS FIRST", 12);
		} else if (order.flags & DatabaseFlags::NullsLast) {
			appendStatic(" NULLS LAST", 11);
		}
	}

	void SqlBuilder::appendQuery(const DatabaseQuery& query)
	{
		ListElements<DatabaseQueryCombine> combines(query.combines);
		if (combines.count) {
			appendStatic("(", 1);
		}
		appendStatic("SELECT ", 7);
		if (query.distinct) {
			appendStatic("DISTINCT ", 9);
		}
		ListElements<DatabaseColumn> columns(query.columns);
		if (columns.count) {
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendColumn(columns[i]);
			}
		} else {
			appendStatic("*", 1);
		}
		appendStatic(" FROM ", 6);
		appendQuerySource(query.source);
		ListElements<DatabaseQueryJoin> joins(query.joins);
		{
			for (sl_size i = 0; i < joins.count; i++) {
				appendStatic(" ", 1);
				appendQueryJoin(joins[i]);
			}
		}
		if (query.where.isNotNull()) {
			appendStatic(" WHERE ", 7);
			query.where.appendTo(*this, sl_false);
		}
		ListElements<DatabaseColumn> groupByColumns(query.groupBy.columns);
		if (groupByColumns.count) {
			appendStatic(" GROUP BY ", 10);
			for (sl_size i = 0; i < groupByColumns.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendColumnNoAs(groupByColumns[i]);
			}
			if (query.groupBy.having.isNotNull()) {
				appendStatic(" HAVING ", 8);
				query.groupBy.having.appendTo(*this, sl_false);
			}
		}
		ListElements<DatabaseQueryOrder> orders(query.orders);
		if (orders.count) {
			appendStatic(" ORDER BY ", 10);
			for (sl_size i = 0; i < orders.count; i++) {
				if (i) {
					appendStatic(", ", 2);
				}
				appendQueryOrder(orders[i]);
			}
		}
		if (query.limit) {
			appendStatic(" LIMIT ", 7);
			append(String::fromUint32(query.limit));
		}
		if (query.offset) {
			appendStatic(" OFFSET ", 8);
			append(String::fromUint32(query.offset));
		}
		if (combines.count) {
			appendStatic(")", 1);
			for (sl_size i = 0; i < combines.count; i++) {
				switch(combines[i].type) {
					case DatabaseCombineType::Union:
						appendStatic(" UNION ", 7);
						break;
					case DatabaseCombineType::UnionAll:
						appendStatic(" UNION ALL ", 11);
						break;
					case DatabaseCombineType::Intersect:
						appendStatic(" INTERSECT ", 11);
						break;
					case DatabaseCombineType::IntersectAll:
						appendStatic(" INTERSECT ALL ", 15);
						break;
					case DatabaseCombineType::Except:
						appendStatic(" EXCEPT ", 8);
						break;
					case DatabaseCombineType::ExcpetAll:
						appendStatic(" EXCEPT ALL ", 12);
						break;
				}
				appendStatic("(", 1);
				appendQuery(combines[i].query);
				appendStatic(")", 1);
			}
			ListElements<DatabaseQueryOrder> ordersOfCombined(query.ordersOfCombined);
			if (ordersOfCombined.count) {
				appendStatic(" ORDER BY ", 10);
				for (sl_size i = 0; i < ordersOfCombined.count; i++) {
					if (i) {
						appendStatic(", ", 2);
					}
					appendQueryOrder(ordersOfCombined[i]);
				}
			}
			if (query.limitOfCombined) {
				appendStatic(" LIMIT ", 7);
				append(String::fromUint32(query.limitOfCombined));
			}
			if (query.offsetOfCombined) {
				appendStatic(" OFFSET ", 8);
				append(String::fromUint32(query.offsetOfCombined));
			}
		}
	}

}
