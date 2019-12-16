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

	DatabaseColumn::DatabaseColumn(const String& _name): name(_name)
	{
	}

	DatabaseColumn::DatabaseColumn(const String& _name, const DatabaseExpression& _expression): name(_name), expression(_expression)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnExpression)

	DatabaseColumnExpression::DatabaseColumnExpression(const DatabaseExpression& expression): DatabaseColumn(sl_null, expression)
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DatabaseColumnIdentifier)

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

	DatabaseQuerySource::DatabaseQuerySource(const String& _name): name(_name)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const DatabaseIdentifier& _table): table(_table)
	{
	}

	DatabaseQuerySource::DatabaseQuerySource(const String& _name, const DatabaseIdentifier& _table): name(_name), table(_table)
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

	void SqlBuilder::appendStatic(const char* sz, sl_uint32 len)
	{
		buf.addStatic(sz, len);
	}

	void SqlBuilder::appendParameter(const String& name)
	{
		buf.addStatic("?", 1);
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
				buf.addStatic("`", 1);
			case DatabaseDialect::MSSQL:
				buf.addStatic("[", 1);
			default:
				buf.addStatic("\"", 1);
		}
	}

	void SqlBuilder::appendIdentifierSuffix()
	{
		switch (dialect) {
			case DatabaseDialect::MySQL:
				buf.addStatic("`", 1);
			case DatabaseDialect::MSSQL:
				buf.addStatic("]", 1);
			default:
				buf.addStatic("\"", 1);
		}
	}
	
	void SqlBuilder::appendIdentifier(const String& name)
	{
		appendIdentifierPrefix();
		buf.add(name);
		appendIdentifierSuffix();
	}

	void SqlBuilder::appendIdentifier(const DatabaseIdentifier& identifier)
	{
		if (identifier.schema.isNotEmpty()) {
			appendIdentifierPrefix();
			buf.add(identifier.schema);
			appendIdentifierSuffix();
			buf.addStatic(".", 1);
		}
		appendIdentifierPrefix();
		buf.add(identifier.name);
		appendIdentifierSuffix();
	}

	void SqlBuilder::generateCreateTable(const DatabaseCreateTableParam& param)
	{
		ListLocker<DatabaseColumnDefinition> columns(param.columns);
		if (param.table.name.isEmpty() || !(columns.count)) {
			return;
		}
		buf.addStatic("CREATE", 6);
		if (param.flags & DatabaseFlags::Temp) {
			buf.addStatic(" TEMPORARY", 10);
		}
		buf.addStatic(" TABLE ", 7);
		if (param.flags & DatabaseFlags::IfNotExists) {
			buf.addStatic("IF NOT EXISTS ", 14);
		}
		appendIdentifier(param.table);
		buf.addStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendColumnDefinition(columns[i]);
			}
		}
		{
			ListLocker<DatabaseTableConstraint> constraints(param.constraints);
			for (sl_size i = 0; i < constraints.count; i++) {
				buf.addStatic(", ", 2);
				appendTableConstraint(constraints[i]);
			}
		}
		buf.addStatic(")", 1);
		if (param.flags & DatabaseFlags::WithoutRowId) {
			if (dialect == DatabaseDialect::SQLite) {
				buf.addStatic(" WITHOUT ROWID", 14);
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
			buf.addStatic("DROP TEMPORARY TABLE ", 21);
		} else {
			buf.addStatic("DROP TABLE ", 11);
		}
		if (flags & DatabaseFlags::IfExists) {
			buf.addStatic("IF EXISTS ", 10);
		}
		appendIdentifier(table);
		if (dialect != DatabaseDialect::SQLite) {
			if (flags & DatabaseFlags::Cascade) {
				buf.addStatic(" CASCADE", 8);
			} else if (flags & DatabaseFlags::Restrict) {
				buf.addStatic(" RESTRICT", 9);
			}
		}
	}

	void SqlBuilder::generateCreateIndex(const DatabaseCreateIndexParam& param)
	{
		ListLocker<DatabaseIndexColumn> columns(param.columns);
		if (param.index.name.isEmpty() || param.table.isEmpty() || !(columns.count)) {
			return;
		}
		buf.addStatic("CREATE", 6);
		if (param.flags & DatabaseFlags::Unique) {
			buf.addStatic(" UNIQUE", 7);
		}
		buf.addStatic(" INDEX ", 7);
		if (param.flags & DatabaseFlags::IfNotExists) {
			if (dialect != DatabaseDialect::MySQL) {
				buf.addStatic("IF NOT EXISTS ", 14);
			}
		}
		appendIdentifier(param.index);
		buf.addStatic(" ON ", 4);
		appendIdentifier(param.table);
		buf.addStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIndexColumn(columns[i]);
			}
		}
		buf.addStatic(")", 1);
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
		buf.addStatic("DROP INDEX ", 11);
		if (flags & DatabaseFlags::IfExists) {
			if (dialect != DatabaseDialect::MySQL) {
				buf.addStatic("IF EXISTS ", 10);
			}
		}
		appendIdentifier(index);
		if (dialect == DatabaseDialect::MySQL) {
			buf.addStatic(" ON ", 4);
			appendIdentifier(table);
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseFlags::Cascade) {
				buf.addStatic(" CASCADE", 8);
			} else if (flags & DatabaseFlags::Restrict) {
				buf.addStatic(" RESTRICT", 9);
			}
		}
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		buf.addStatic("INSERT INTO ", 12);
		appendIdentifier(table);
		buf.addStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIdentifier(columns[i].name);
			}
		}
		buf.addStatic(") VALUES (", 10);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					buf.addStatic("?", 1);
				}
			}
		}
		buf.addStatic(")", 1);
	}

	void SqlBuilder::generateInsert(const DatabaseIdentifier& table, const ListParam<String>& _columns)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		buf.addStatic("INSERT INTO ", 12);
		appendIdentifier(table);
		buf.addStatic(" (", 2);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
			}
		}
		buf.addStatic(") VALUES (", 10);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				buf.addStatic("?", 1);
			}
		}
		buf.addStatic(")", 1);
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<DatabaseColumn>& _columns, const DatabaseExpression& where)
	{
		ListLocker<DatabaseColumn> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		buf.addStatic("UPDATE ", 7);
		appendIdentifier(table);
		buf.addStatic(" SET ", 5);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIdentifier(columns[i].name);
				buf.addStatic("=", 1);
				if (columns[i].expression.isNotNull()) {
					columns[i].expression.appendTo(*this, sl_false);
				} else {
					buf.addStatic("?", 1);
				}
			}
		}
		if (where.isNotNull()) {
			buf.addStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateUpdate(const DatabaseIdentifier& table, const ListParam<String>& _columns, const DatabaseExpression& where)
	{
		ListLocker<String> columns(_columns);
		if (columns.count < 1) {
			return;
		}
		buf.addStatic("UPDATE ", 7);
		appendIdentifier(table);
		buf.addStatic(" SET ", 5);
		{
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
				buf.addStatic("=?", 2);
			}
		}
		if (where.isNotNull()) {
			buf.addStatic(" WHERE ", 7);
			where.appendTo(*this, sl_false);
		}
	}

	void SqlBuilder::generateDelete(const DatabaseIdentifier& table, const DatabaseExpression& where)
	{
		buf.addStatic("DELETE FROM ", 12);
		appendIdentifier(table);
		if (where.isNotNull()) {
			buf.addStatic(" WHERE ", 7);
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
		buf.addStatic(s, sizeof(s)-1); \
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
		buf.addStatic(s, sizeof(s)-1); \
		return; \
	}

#define DEFINE_COLUMN_TYPE_RETURN_MYSQL(TYPE) \
	if (flags & (DatabaseColumnFlags::Unsigned | DatabaseColumnFlags::ZeroFill)) { \
		SLIB_STATIC_STRING(s, TYPE) \
		pStrType = &s; \
		break; \
	} else { \
		static char s[] = TYPE; \
		buf.addStatic(s, sizeof(s)-1); \
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
		buf.addStatic(s, sizeof(s)-1); \
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
		buf.addStatic(s, sizeof(s)-1); \
		return; \
	}

	void SqlBuilder::appendColumnTypeName(const DatabaseColumnTypeDefinition& type, const DatabaseColumnFlags& flags)
	{
		if (type.rawType.isNotEmpty()) {
			buf.add(type.rawType);
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
					buf.addStatic("INTEGER", 7);
					return;
				case DatabaseColumnType::BOOLEAN:
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
				case DatabaseColumnType::DATE:
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIME:
				case DatabaseColumnType::TIMESTAMP:
					buf.addStatic("NUMERIC", 7);
					return;
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					buf.addStatic("REAL", 4);
					return;
				case DatabaseColumnType::CHAR:
				case DatabaseColumnType::NCHAR:
				case DatabaseColumnType::VARCHAR:
				case DatabaseColumnType::NVARCHAR:
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					buf.addStatic("TEXT", 4);
					return;
				case DatabaseColumnType::BINARY:
				case DatabaseColumnType::VARBINARY:
				case DatabaseColumnType::TINYBLOB:
				case DatabaseColumnType::BLOB:
				case DatabaseColumnType::MEDIUMBLOB:
				case DatabaseColumnType::LONGBLOB:
					buf.addStatic("BLOB", 4);
					return;
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					buf.addStatic("boolean", 7);
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						buf.addStatic("smallserial", 11);
					} else {
						buf.addStatic("smallint", 8);
					}
					return;
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::INTEGER:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						buf.addStatic("serial", 6);
					} else {
						buf.addStatic("integer", 7);
					}
					return;
				case DatabaseColumnType::BIGINT:
					if (flags & DatabaseColumnFlags::AutoIncrement) {
						buf.addStatic("bigserial", 9);
					} else {
						buf.addStatic("bigint", 6);
					}
					return;
				case DatabaseColumnType::SMALLSERIAL:
					buf.addStatic("smallserial", 11);
					return;
				case DatabaseColumnType::SERIAL:
					buf.addStatic("serial", 6);
					return;
				case DatabaseColumnType::BIGSERIAL:
					buf.addStatic("bigserial", 9);
					return;
				case DatabaseColumnType::DECIMAL:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("decimal")
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("numeric")
				case DatabaseColumnType::FLOAT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("float")
				case DatabaseColumnType::REAL:
					buf.addStatic("real", 4);
					return;
				case DatabaseColumnType::DOUBLE:
					buf.addStatic("double precision", 16);
					return;
				case DatabaseColumnType::DATE:
					buf.addStatic("date", 4);
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
						buf.addStatic("timestamp", 9);
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
						buf.addStatic("time", 4);
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
					buf.addStatic("bytea", 5);
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					buf.addStatic("text", 4);
					return;
			}
		} else if (dialect == DatabaseDialect::MySQL) {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					buf.addStatic("BOOLEAN", 7);
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
					buf.addStatic("REAL", 4);
					return;
				case DatabaseColumnType::DOUBLE:
					buf.addStatic("DOUBLE", 6);
					return;
				case DatabaseColumnType::DATE:
					buf.addStatic("DATE", 4);
					return;
				case DatabaseColumnType::DATETIME:
					buf.addStatic("DATETIME", 8);
					return;
				case DatabaseColumnType::TIMESTAMP:
					buf.addStatic("TIMESTAMP", 9);
					return;
				case DatabaseColumnType::TIME:
					buf.addStatic("TIME", 4);
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
					buf.addStatic("TINYBLOB", 8);
					return;
				case DatabaseColumnType::BLOB:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("BLOB")
				case DatabaseColumnType::MEDIUMBLOB:
					buf.addStatic("MEDIUMBLOB", 10);
					return;
				case DatabaseColumnType::LONGBLOB:
					buf.addStatic("LONGBLOB", 8);
					return;
				case DatabaseColumnType::TINYTEXT:
					buf.addStatic("TINYTEXT", 8);
					return;
				case DatabaseColumnType::TEXT:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH("TEXT")
				case DatabaseColumnType::MEDIUMTEXT:
					buf.addStatic("MEDIUMTEXT", 10);
					return;
				case DatabaseColumnType::LONGTEXT:
					buf.addStatic("LONGTEXT", 8);
					return;
			}
		} else {
			switch (type.type) {
				case DatabaseColumnType::BOOLEAN:
					buf.addStatic("BOOLEAN", 7);
					return;
				case DatabaseColumnType::TINYINT:
				case DatabaseColumnType::SMALLSERIAL:
				case DatabaseColumnType::SMALLINT:
				case DatabaseColumnType::MEDIUMINT:
				case DatabaseColumnType::SERIAL:
				case DatabaseColumnType::INTEGER:
					buf.addStatic("INT", 3);
					return;
				case DatabaseColumnType::BIGSERIAL:
				case DatabaseColumnType::BIGINT:
					buf.addStatic("BIGINT", 6);
					return;
				case DatabaseColumnType::DECIMAL:
				case DatabaseColumnType::NUMERIC:
					DEFINE_COLUMN_TYPE_NAME_WITH_LENGTH_AND_SCALE("NUMERIC")
				case DatabaseColumnType::FLOAT:
				case DatabaseColumnType::REAL:
				case DatabaseColumnType::DOUBLE:
					buf.addStatic("DOUBLE", 6);
					return;
				case DatabaseColumnType::DATE:
					buf.addStatic("DATE", 4);
					return;
				case DatabaseColumnType::DATETIME:
				case DatabaseColumnType::TIMESTAMP:
				case DatabaseColumnType::TIME:
					buf.addStatic("DATETIME", 8);
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
					buf.addStatic("BLOB", 4);
					return;
				case DatabaseColumnType::TINYTEXT:
				case DatabaseColumnType::TEXT:
				case DatabaseColumnType::MEDIUMTEXT:
				case DatabaseColumnType::LONGTEXT:
					buf.addStatic("TEXT", 4);
					return;
			}
		}
		buf.add(*pStrType);
		if (flagAppendLength) {
			buf.addStatic("(", 1);
			buf.add(String::fromUint32(type.length));
			if (flagAppendScale && type.scale) {
				buf.addStatic(",", 1);
				buf.add(String::fromUint32(type.scale));
			}
			buf.addStatic(")", 1);
		}
		if (dialect == DatabaseDialect::MySQL) {
			if (flags & DatabaseColumnFlags::Unsigned) {
				buf.addStatic(" UNSIGNED", 9);
			}
			if (flags & DatabaseColumnFlags::ZeroFill) {
				buf.addStatic(" ZEROFILL", 9);
			}
		} else if (dialect == DatabaseDialect::PostgreSQL) {
			if (flags & DatabaseColumnFlags::WithTimeZone) {
				buf.addStatic(" with time zone", 15);
			}
		}
	}

	void SqlBuilder::appendReferentialActionString(DatabaseReferentialAction action)
	{
		switch (action) {
			case DatabaseReferentialAction::Default:
				return;
			case DatabaseReferentialAction::SetNull:
				buf.addStatic("SET NULL", 8);
				return;
			case DatabaseReferentialAction::SetDefault:
				buf.addStatic("SET DEFAULT", 11);
				return;
			case DatabaseReferentialAction::Cascade:
				buf.addStatic("CASCADE", 7);
				return;
			case DatabaseReferentialAction::Restrict:
				buf.addStatic("RESTRICT", 8);
				return;
			case DatabaseReferentialAction::NoAction:
				buf.addStatic("NO ACTION", 9);
				return;
		}
	}

	void SqlBuilder::appendColumn(const DatabaseColumn& column)
	{
		if (column.expression.isNotNull()) {
			column.expression.appendTo(*this, sl_true);
			if (column.name.isNotEmpty()) {
				buf.addStatic(" AS ", 4);
				appendIdentifier(column.name);
			}
		} else {
			appendIdentifier(column.name);
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
		buf.addStatic("REFERENCES ", 11);
		appendIdentifier(reference.tableName);
		ListLocker<String> columns(reference.columns);
		if (columns.count > 0) {
			buf.addStatic(" (", 2);
			for (sl_size i = 0; i < columns.count; i++) {
				if (i) {
					buf.addStatic(", ", 2);
				}
				appendIdentifier(columns[i]);
			}
			buf.addStatic(")", 1);
		}
		switch (reference.match) {
			case DatabaseReferenceMatch::Simple:
				buf.addStatic(" MATCH SIMPLE", 13);
				break;
			case DatabaseReferenceMatch::Full:
				buf.addStatic(" MATCH FULL", 11);
				break;
			case DatabaseReferenceMatch::Partial:
				buf.addStatic(" MATCH PARTIAL", 14);
				break;
			case DatabaseReferenceMatch::Default:
				break;
		}
		if (reference.onDelete != DatabaseReferentialAction::Default) {
			buf.addStatic(" ", 1);
			appendReferentialActionString(reference.onDelete);
		}
		if (reference.onUpdate != DatabaseReferentialAction::Default) {
			buf.addStatic(" ", 1);
			appendReferentialActionString(reference.onUpdate);
		}
	}

	void SqlBuilder::appendColumnDefinition(const DatabaseColumnDefinition& column)
	{
		appendIdentifier(column.name);
		buf.addStatic(" ", 1);
		appendColumnTypeName(column.type, column.flags);
		if (column.collate.isNotEmpty()) {
			buf.addStatic(" COLLATE ", 9);
			buf.add(column.collate);
		}
		if (column.flags & DatabaseColumnFlags::NotNull) {
			buf.addStatic(" NOT NULL", 9);
		}
		if (column.defaultValue.isNotNull()) {
			buf.addStatic(" DEFAULT ", 9);
			column.defaultValue.appendTo(*this, sl_true);
		}
		if (column.flags & DatabaseColumnFlags::AutoIncrement) {
			if (dialect == DatabaseDialect::MySQL) {
				buf.addStatic(" AUTO_INCREMENT", 15);
			}
		}
		if (column.flags & DatabaseColumnFlags::PrimaryKey) {
			buf.addStatic(" PRIMARY KEY", 12);
			if (dialect == DatabaseDialect::SQLite) {
				if (column.flags & DatabaseColumnFlags::Asc) {
					buf.addStatic(" ASC", 4);
				} else if (column.flags & DatabaseColumnFlags::Desc) {
					buf.addStatic(" DESC", 5);
				}
				if (column.flags & DatabaseColumnFlags::AutoIncrement) {
					buf.addStatic(" AUTOINCREMENT", 14);
				}
			}
		}
		if (column.flags & DatabaseColumnFlags::Unique) {
			buf.addStatic(" UNIQUE", 7);
		}
		if (column.reference.tableName.isNotEmpty()) {
			buf.addStatic(" ", 1);
			appendReferenceDefinition(column.reference);
		}
	}

	void SqlBuilder::appendTableConstraint(const DatabaseTableConstraint& constraint)
	{
		buf.addStatic("CONSTRAINT ", 11);
		buf.add(constraint.name);
		buf.addStatic(" ", 1);
		switch (constraint.type) {
			case DatabaseConstaintType::PrimaryKey:
				buf.addStatic(" PRIMARY KEY", 12);
				break;
			case DatabaseConstaintType::Unique:
				buf.addStatic(" UNIQUE", 7);
				break;
			case DatabaseConstaintType::ForeignKey:
				buf.addStatic(" FOREIGN KEY", 12);
				break;
		}
		buf.addStatic(" (", 2);
		ListLocker<String> columns(constraint.columns);
		for (sl_size i = 0; i < columns.count; i++) {
			if (i) {
				buf.addStatic(", ", 2);
			}
			appendIdentifier(columns[i]);
		}
		buf.addStatic(")", 1);
		if (constraint.type == DatabaseConstaintType::ForeignKey) {
			buf.addStatic(" ", 1);
			appendReferenceDefinition(constraint.reference);
		}
	}

	void SqlBuilder::appendIndexColumn(const DatabaseIndexColumn& column)
	{
		appendColumnNoAs(column.column);
		if (column.collate.isNotEmpty()) {
			if (dialect != DatabaseDialect::MySQL) {
				buf.addStatic(" COLLATE ", 9);
				buf.add(column.collate);
			}
		}
		if (column.order == DatabaseOrderType::Asc) {
			buf.addStatic(" ASC", 4);
		} else if (column.order == DatabaseOrderType::Desc) {
			buf.addStatic(" DESC", 5);
		}
	}

}
