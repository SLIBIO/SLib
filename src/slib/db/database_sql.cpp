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
		if (dialect == DatabaseDialect::PostgreSQL) {
			append("$" + String::fromUint32((sl_int32)(parameters.getCount() + 1)));
		} else {
			appendStatic("?");
		}
		parameters.add_NoLock(name);
	}

	void SqlBuilder::appendParameter()
	{
		appendParameter(String::null());
	}

	String SqlBuilder::toString() const
	{
		return buf.merge();
	}

	void SqlBuilder::appendIdentifierPrefix()
	{
		switch (dialect) {
			case DatabaseDialect::MySQL:
				appendStatic("`");
				break;
			case DatabaseDialect::MSSQL:
				appendStatic("[");
				break;
			default:
				appendStatic("\"");
				break;
		}
	}

	void SqlBuilder::appendIdentifierSuffix()
	{
		switch (dialect) {
			case DatabaseDialect::MySQL:
				appendStatic("`");
				break;
			case DatabaseDialect::MSSQL:
				appendStatic("]");
				break;
			default:
				appendStatic("\"");
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
			appendStatic(".");
		}
		if (identifier.name.equals("*")) {
			appendStatic("*");
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
		appendStatic("CREATE");
		if (param.flags & DatabaseFlags::Temp) {
			appendStatic(" TEMPORARY");
		}
		appendStatic(" TABLE ");
		if (param.flags & DatabaseFlags::IfNotExists) {
			appendStatic("IF NOT EXISTS ");
		}
		appendIdentifier(param.table);
		appendStatic(" (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendColumnDefinition(columns[i]);
			}
		}
		{
			ListLocker<DatabaseTableConstraint> constraints(param.constraints);
			for (sl_size i = 0; i < constraints.count; i++) {
				appendStatic(", ");
				appendTableConstraint(constraints[i]);
			}
		}
		appendStatic(")");
		if (param.flags & DatabaseFlags::WithoutRowId) {
			if (dialect == DatabaseDialect::SQLite) {
				appendStatic(" WITHOUT ROWID");
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
			appendStatic("DROP TEMPORARY TABLE ");
		} else {
			appendStatic("DROP TABLE ");
		}
		if (flags & DatabaseFlags::IfExists) {
			appendStatic("IF EXISTS ");
		}
		appendIdentifier(table);
		if (dialect != DatabaseDialect::SQLite) {
			if (flags & DatabaseFlags::Cascade) {
				appendStatic(" CASCADE");
			} else if (flags & DatabaseFlags::Restrict) {
				appendStatic(" RESTRICT");
			}
		}
	}

	void SqlBuilder::generateCreateIndex(const DatabaseCreateIndexParam& param)
	{
		ListLocker<DatabaseIndexColumn> columns(param.columns);
		if (param.index.name.isEmpty() || param.table.isEmpty() || !(columns.count)) {
			return;
		}
		appendStatic("CREATE");
		if (param.flags & DatabaseFlags::Unique) {
			appendStatic(" UNIQUE");
		}
		appendStatic(" INDEX ");
		if (param.flags & DatabaseFlags::IfNotExists) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic("IF NOT EXISTS ");
			}
		}
		appendIdentifier(param.index);
		appendStatic(" ON ");
		appendIdentifier(param.table);
		appendStatic(" (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIndexColumn(columns[i]);
			}
		}
		appendStatic(")");
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
		appendStatic("DROP INDEX ");
		if (flags & DatabaseFlags::IfExists) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic("IF EXISTS ");
			}
		}
		appendIdentifier(index);
		if (dialect == DatabaseDialect::MySQL) {
			appendStatic(" ON ");
			appendIdentifier(table);
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseFlags::Cascade) {
				appendStatic(" CASCADE");
			} else if (flags & DatabaseFlags::Restrict) {
				appendStatic(" RESTRICT");
			}
		}
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("INSERT INTO ");
		appendIdentifier(table);
		appendStatic(" (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i].name);
			}
		}
		appendStatic(") VALUES (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					appendParameter();
				}
			}
		}
		appendStatic(")");
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<String>& _columns)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("INSERT INTO ");
		appendIdentifier(table);
		appendStatic(" (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i]);
			}
		}
		appendStatic(") VALUES (");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendParameter();
			}
		}
		appendStatic(")");
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns, const DatabaseExpression& where)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("UPDATE ");
		appendIdentifier(table);
		appendStatic(" SET ");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i].name);
				appendStatic("=");
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					appendParameter();
				}
			}
		}
		if (where.isNotNull()) {
			appendStatic(" WHERE ");
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<String>& _columns, const DatabaseExpression& where)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		appendStatic("UPDATE ");
		appendIdentifier(table);
		appendStatic(" SET ");
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i]);
				appendStatic("=?");
			}
		}
		if (where.isNotNull()) {
			appendStatic(" WHERE ");
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateDelete(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		appendStatic("DELETE FROM ");
		appendIdentifier(table);
		if (where.isNotNull()) {
			appendStatic(" WHERE ");
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateSelect(const DatabaseSelectParam& param)
	{
		ListElements<DatabaseQueryWith> withs(param.withs);
		if (withs.count) {
			appendStatic("WITH ");
			if (param.withRecursive) {
				appendStatic("RECURSIVE ");
			}
			for (sl_size i = 0; i < withs.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendSelectWithClause(withs[i]);
			}
			appendStatic(" ");
		}
		appendQuery(param);
	}
	
	void SqlBuilder::generateSelect(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		appendStatic("SELECT * FROM ");
		appendIdentifier(table);
		if (where.isNotNull()) {
			appendStatic(" WHERE ");
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
		appendStatic(TYPE); \
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
		appendStatic(TYPE); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_RETURN_MYSQL(TYPE) \
	if (flags & (DatabaseColumnFlags::Unsigned | DatabaseColumnFlags::ZeroFill)) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		break; \
	} else { \
		appendStatic(TYPE); \
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
		appendStatic(TYPE); \
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
		appendStatic(TYPE); \
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
					appendStatic("INTEGER");
					return;
				case DatabaseColumnType::BOOLEAN:
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
				case DatabaseColumnType::DATE:
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIME:
				case DatabaseColumnType::TIMESTAMP:
					appendStatic("NUMERIC");
					return;
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					appendStatic("REAL");
					return;
				case DatabaseColumnType::CHAR:
				case DatabaseColumnType::NCHAR:
				case DatabaseColumnType::VARCHAR:
				case DatabaseColumnType::NVARCHAR:
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("TEXT");
					return;
				case DatabaseColumnType::BINARY:
				case DatabaseColumnType::VARBINARY:
				case DatabaseColumnType::TINYBLOB:
				case DatabaseColumnType::BLOB:
				case DatabaseColumnType::MEDIUMBLOB:
				case DatabaseColumnType::LONGBLOB:
					appendStatic("BLOB");
					return;
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("boolean");
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("smallserial");
					} else {
						appendStatic("smallint");
					}
					return;
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::INTEGER:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("serial");
					} else {
						appendStatic("integer");
					}
					return;
				case DatabaseColumnType::BIGINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						appendStatic("bigserial");
					} else {
						appendStatic("bigint");
					}
					return;
				case DatabaseColumnType::SMALLSERIAL:
					appendStatic("smallserial");
					return;
				case DatabaseColumnType::SERIAL:
					appendStatic("serial");
					return;
				case DatabaseColumnType::BIGSERIAL:
					appendStatic("bigserial");
					return;
				case DatabaseColumnType::DECIMAL:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("decimal")
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("numeric")
				case DatabaseColumnType::FLOAT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("float")
				case DatabaseColumnType::REAL:
					appendStatic("real");
					return;
				case DatabaseColumnType::DOUBLE:
					appendStatic("double precision");
					return;
				case DatabaseColumnType::DATE:
					appendStatic("date");
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
						appendStatic("timestamp");
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
						appendStatic("time");
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
					appendStatic("bytea");
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("text");
					return;
			}
		} else if (dialect == DatabaseDialect::MySQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("BOOLEAN");
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
					appendStatic("REAL");
					return;
				case DatabaseColumnType::DOUBLE:
					appendStatic("DOUBLE");
					return;
				case DatabaseColumnType::DATE:
					appendStatic("DATE");
					return;
				case DatabaseColumnType::DATETIME:
					appendStatic("DATETIME");
					return;
				case DatabaseColumnType::TIMESTAMP:
					appendStatic("TIMESTAMP");
					return;
				case DatabaseColumnType::TIME:
					appendStatic("TIME");
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
					appendStatic("TINYBLOB");
					return;
				case DatabaseColumnType::BLOB:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("BLOB")
				case DatabaseColumnType::MEDIUMBLOB:
					appendStatic("MEDIUMBLOB");
					return;
				case DatabaseColumnType::LONGBLOB:
					appendStatic("LONGBLOB");
					return;
				case DatabaseColumnType::TINYTEXT:
					appendStatic("TINYTEXT");
					return;
				case DatabaseColumnType::TEXT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("TEXT")
				case DatabaseColumnType::MEDIUMTEXT:
					appendStatic("MEDIUMTEXT");
					return;
				case DatabaseColumnType::LONGTEXT:
					appendStatic("LONGTEXT");
					return;
			}
		} else {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					appendStatic("BOOLEAN");
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLSERIAL:
				case DatabaseColumnType::SMALLINT:
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::SERIAL:
				case DatabaseColumnType::INTEGER:
					appendStatic("INT");
					return;
				case DatabaseColumnType::BIGSERIAL:
				case DatabaseColumnType::BIGINT:
					appendStatic("BIGINT");
					return;
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("NUMERIC")
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					appendStatic("DOUBLE");
					return;
				case DatabaseColumnType::DATE:
					appendStatic("DATE");
					return;
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIMESTAMP:
				case DatabaseColumnType::TIME:
					appendStatic("DATETIME");
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
					appendStatic("BLOB");
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					appendStatic("TEXT");
					return;
			}
		}
		append(*pStrType);
		if (flagAppendLength) {
			appendStatic("(");
			append(String::fromUint32(type.length));
			if (flagAppendScale && type.scale) {
				appendStatic(",");
				append(String::fromUint32(type.scale));
			}
			appendStatic(")");
		}
		if (dialect == DatabaseDialect::MySQL) {
			if (flags & DatabaseColumnFlags::Unsigned) {
				appendStatic(" UNSIGNED");
			}
			if (flags & DatabaseColumnFlags::ZeroFill) {
				appendStatic(" ZEROFILL");
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseColumnFlags::WithTimeZone) {
				appendStatic(" with time zone");
			}
		}
	}

	void SqlBuilder::appendReferentialActionString(DatabaseReferentialAction action)
	{
		switch (action) {
			case DatabaseReferentialAction::Default:
				return;
			case DatabaseReferentialAction::SetNull:
				appendStatic("SET NULL");
				return;
			case DatabaseReferentialAction::SetDefault:
				appendStatic("SET DEFAULT");
				return;
			case DatabaseReferentialAction::Cascade:
				appendStatic("CASCADE");
				return;
			case DatabaseReferentialAction::Restrict:
				appendStatic("RESTRICT");
				return;
			case DatabaseReferentialAction::NoAction:
				appendStatic("NO ACTION");
				return;
		}
	}

	void SqlBuilder::appendColumn(const DatabaseColumn& column)
	{
		if (column.expression.isNotNull()) {
			column.expression.appendTo(*this, sl_true);
			if (column.name.isNotEmpty()) {
				appendStatic(" ");
				appendIdentifier(column.name);
			}
		} else {
			if (column.name.equals("*")) {
				appendStatic("*");
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
		appendStatic("REFERENCES ");
		appendIdentifier(reference.tableName);
		ListLocker<String> columns(reference.columns);
		if (columns.count > 0) {
			appendStatic(" (");
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i]);
			}
			appendStatic(")");
		}
		switch (reference.match) {
			case DatabaseReferenceMatch::Simple:
				appendStatic(" MATCH SIMPLE");
				break;
			case DatabaseReferenceMatch::Full:
				appendStatic(" MATCH FULL");
				break;
			case DatabaseReferenceMatch::Partial:
				appendStatic(" MATCH PARTIAL");
				break;
			case DatabaseReferenceMatch::Default:
				break;
		}
		if (reference.onDelete != DatabaseReferentialAction::Default) {
			appendStatic(" ");
			appendReferentialActionString(reference.onDelete);
		}
		if (reference.onUpdate != DatabaseReferentialAction::Default) {
			appendStatic(" ");
			appendReferentialActionString(reference.onUpdate);
		}
	}

	void SqlBuilder::appendColumnDefinition(const DatabaseColumnDefinition& column)
	{
		appendIdentifier(column.name);
		appendStatic(" ");
		appendColumnTypeName(column.type, column.flags);
		if (column.collate.isNotEmpty()) {
			appendStatic(" COLLATE ");
			append(column.collate);
		}
		if (column.flags & DatabaseColumnFlags::NotNull) {
			appendStatic(" NOT NULL");
		}
		if (column.defaultValue.isNotNull()) {
			appendStatic(" DEFAULT ");
			column.defaultValue.appendTo(*this, sl_true);
		}
		if (column.flags & DatabaseColumnFlags::AutoIncrement) {
			if (dialect == DatabaseDialect::MySQL) {
				appendStatic(" AUTO_INCREMENT");
			}
		}
		if (column.flags & DatabaseColumnFlags::PrimaryKey) {
			appendStatic(" PRIMARY KEY");
			if (dialect == DatabaseDialect::SQLite) {
				if (column.flags & DatabaseColumnFlags::Asc) {
					appendStatic(" ASC");
				} else if (column.flags & DatabaseColumnFlags::Desc) {
					appendStatic(" DESC");
				}
				if (column.flags & DatabaseColumnFlags::AutoIncrement) {
					appendStatic(" AUTOINCREMENT");
				}
			}
		}
		if (column.flags & DatabaseColumnFlags::Unique) {
			appendStatic(" UNIQUE");
		}
		if (column.reference.tableName.isNotEmpty()) {
			appendStatic(" ");
			appendReferenceDefinition(column.reference);
		}
	}

	void SqlBuilder::appendTableConstraint(const DatabaseTableConstraint& constraint)
	{
		appendStatic("CONSTRAINT ");
		append(constraint.name);
		appendStatic(" ");
		switch (constraint.type) {
			case DatabaseConstaintType::PrimaryKey:
				appendStatic(" PRIMARY KEY");
				break;
			case DatabaseConstaintType::Unique:
				appendStatic(" UNIQUE");
				break;
			case DatabaseConstaintType::ForeignKey:
				appendStatic(" FOREIGN KEY");
				break;
		}
		appendStatic(" (");
		ListLocker<String> columns(constraint.columns);
		for (sl_size i = 0; i < columns.count; i++) {
			if (i) {
				appendStatic(", ");
			}
			appendIdentifier(columns[i]);
		}
		appendStatic(")");
		if (constraint.type == DatabaseConstaintType::ForeignKey) {
			appendStatic(" ");
			appendReferenceDefinition(constraint.reference);
		}
	}

	void SqlBuilder::appendIndexColumn(const DatabaseIndexColumn& column)
	{
		appendColumnNoAs(column.column);
		if (column.collate.isNotEmpty()) {
			if (dialect != DatabaseDialect::MySQL) {
				appendStatic(" COLLATE ");
				append(column.collate);
			}
		}
		if (column.order == DatabaseOrderType::Asc) {
			appendStatic(" ASC");
		} else if (column.order == DatabaseOrderType::Desc) {
			appendStatic(" DESC");
		}
	}

	void SqlBuilder::appendSelectWithClause(const DatabaseQueryWith& with)
	{
		ListElements<String> columns(with.columns);
		appendIdentifier(with.name);
		if (columns.count) {
			appendStatic("(");
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendIdentifier(columns[i]);
			}
			appendStatic(")");
		}
		appendStatic(" AS (");
		appendQuery(with.query);
		appendStatic(")");
	}

	void SqlBuilder::appendQuerySource(const DatabaseQuerySource& source)
	{
		if (source.query.isNotNull()) {
			appendStatic("(");
			appendQuery(*(source.query));
			appendStatic(")");
		} else if (source.expression.isNotNull()) {
			source.expression.appendTo(*this, sl_true);
		} else {
			appendIdentifier(source.table);
		}
		if (source.name.isNotEmpty()) {
			appendStatic(" ");
			appendIdentifier(source.name);
		}
	}

	void SqlBuilder::appendQueryJoin(const DatabaseQueryJoin& join)
	{
		switch (join.type) {
			case DatabaseJoinType::Left:
				appendStatic("LEFT JOIN ");
				break;
			case DatabaseJoinType::Right:
				appendStatic("RIGHT JOIN ");
				break;
			case DatabaseJoinType::Inner:
				appendStatic("INNER JOIN ");
				break;
			case DatabaseJoinType::Full:
				appendStatic("FULL JOIN ");
				break;
			default:
				return;
		}
		appendQuerySource(join);
		if (join.on.isNotNull()) {
			appendStatic(" ");
			join.on.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::appendQueryOrder(const DatabaseQueryOrder& order)
	{
		appendColumnNoAs(order.column);
		if (order.collate.isNotEmpty()) {
			appendStatic(" COLLATE ");
			append(order.collate);
		}
		if (order.type == DatabaseOrderType::Asc) {
			appendStatic(" ASC");
		} else if (order.type == DatabaseOrderType::Desc) {
			appendStatic(" DESC");
		}
		if (order.flags & DatabaseFlags::NullsFirst) {
			appendStatic(" NULLS FIRST");
		} else if (order.flags & DatabaseFlags::NullsLast) {
			appendStatic(" NULLS LAST");
		}
	}

	void SqlBuilder::appendQuery(const DatabaseQuery& query)
	{
		ListElements<DatabaseQueryCombine> combines(query.combines);
		if (combines.count) {
			appendStatic("(");
		}
		appendStatic("SELECT ");
		if (query.distinct) {
			appendStatic("DISTINCT ");
		}
		ListElements<DatabaseColumn> columns(query.columns);
		if (columns.count) {
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendColumn(columns[i]);
			}
		} else {
			appendStatic("*");
		}
		appendStatic(" FROM ");
		appendQuerySource(query.source);
		ListElements<DatabaseQueryJoin> joins(query.joins);
		{
			for (sl_size i = 0; i < joins.count; i++) {
				appendStatic(" ");
				appendQueryJoin(joins[i]);
			}
		}
		if (query.where.isNotNull()) {
			appendStatic(" WHERE ");
			query.where.appendTo(*this, sl_false);
		}
		ListElements<DatabaseColumn> groupByColumns(query.groupBy.columns);
		if (groupByColumns.count) {
			appendStatic(" GROUP BY ");
			for (sl_size i = 0; i < groupByColumns.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendColumnNoAs(groupByColumns[i]);
			}
			if (query.groupBy.having.isNotNull()) {
				appendStatic(" HAVING ");
				query.groupBy.having.appendTo(*this, sl_false);
			}
		}
		ListElements<DatabaseQueryOrder> orders(query.orders);
		if (orders.count) {
			appendStatic(" ORDER BY ");
			for (sl_size i = 0; i < orders.count; i++) {
				if (i) {
					appendStatic(", ");
				}
				appendQueryOrder(orders[i]);
			}
		}
		if (query.limit) {
			appendStatic(" LIMIT ");
			append(String::fromUint32(query.limit));
		}
		if (query.offset) {
			appendStatic(" OFFSET ");
			append(String::fromUint32(query.offset));
		}
		if (combines.count) {
			appendStatic(")");
			for (sl_size i = 0; i < combines.count; i++) {
				switch(combines[i].type) {
					case DatabaseCombineType::Union:
						appendStatic(" UNION ");
						break;
					case DatabaseCombineType::UnionAll:
						appendStatic(" UNION ALL ");
						break;
					case DatabaseCombineType::Intersect:
						appendStatic(" INTERSECT ");
						break;
					case DatabaseCombineType::IntersectAll:
						appendStatic(" INTERSECT ALL ");
						break;
					case DatabaseCombineType::Except:
						appendStatic(" EXCEPT ");
						break;
					case DatabaseCombineType::ExcpetAll:
						appendStatic(" EXCEPT ALL ");
						break;
				}
				appendStatic("(");
				appendQuery(combines[i].query);
				appendStatic(")");
			}
			ListElements<DatabaseQueryOrder> ordersOfCombined(query.ordersOfCombined);
			if (ordersOfCombined.count) {
				appendStatic(" ORDER BY ");
				for (sl_size i = 0; i < ordersOfCombined.count; i++) {
					if (i) {
						appendStatic(", ");
					}
					appendQueryOrder(ordersOfCombined[i]);
				}
			}
			if (query.limitOfCombined) {
				appendStatic(" LIMIT ");
				append(String::fromUint32(query.limitOfCombined));
			}
			if (query.offsetOfCombined) {
				appendStatic(" OFFSET ");
				append(String::fromUint32(query.offsetOfCombined));
			}
		}
	}

}
