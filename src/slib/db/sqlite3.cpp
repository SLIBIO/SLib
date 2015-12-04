#include "../../../inc/thirdparty/sqlite/sqlite3.h"

#include "../../../inc/slib/db/sqlite.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/file.h"

SLIB_DATABASE_NAMESPACE_BEGIN
SqliteDatabase::SqliteDatabase()
{
}

class _Sqlite3Database : public SqliteDatabase
{
public:
	sl_bool m_flagOpened;
	sqlite3* m_db;

	SLIB_INLINE _Sqlite3Database()
	{
		m_flagOpened = sl_false;
		m_db = sl_null;
	}

	~_Sqlite3Database()
	{
		close();
	}

	static Ref<_Sqlite3Database> connect(const String& _filePath)
	{
		sqlite3* db;
		String8 filePath = _filePath;
		if (File::exists(filePath)) {
			Ref<_Sqlite3Database> ret;
			sl_int32 iResult = sqlite3_open(filePath.getBuf(), &db);
			if (SQLITE_OK == iResult) {
				ret = new _Sqlite3Database();
				if (ret.isNotNull()) {
					ret->m_db = db;
					ret->m_flagOpened = sl_true;
					return ret;
				}
				sqlite3_close(db);
			}
			return ret;
		}
		return sl_null;
	}

	void close()
	{
		if (m_flagOpened) {
			m_flagOpened = sl_false;
			sqlite3_close(m_db);
			m_db = sl_null;
		}
	}

	sl_bool isOpened()
	{
		return m_flagOpened;
	}

	sl_bool executeUpdate(const String& _query)
	{
		if (!m_flagOpened) {
			return sl_false;
		}
		char *zErrMsg = 0;
		String8 query = _query;
		if (SQLITE_OK == sqlite3_exec(m_db, query.getBuf(), 0, 0, &zErrMsg)) {
			return sl_true;
		}
		sqlite3_free(zErrMsg);

		return sl_false;
	}

	static List<String> _getColumnNames(sqlite3_stmt* stmt)
	{
		List<String> ret;
		sl_int32 cols = sqlite3_column_count(stmt);
		for (int i = 0; i < cols; i++) {
			const char* buf = sqlite3_column_name(stmt, i);
			String name(buf);
			ret.push(name);
		}
		return ret;
	}

	static Map<String, Variant> _getRow(sqlite3_stmt* stmt, String* columns, sl_uint32 nColumns)
	{
		Map<String, Variant> row = Map<String, Variant>::createList();
		for (sl_uint32 cIndex = 0; cIndex < nColumns; cIndex++) {
			String cValue = String::fromUtf8(sqlite3_column_text(stmt, (int)cIndex));
			row.put(columns[cIndex], cValue);
		}
		return row;
	}

	class _DatabaseCursor : public DatabaseCursor
	{
	public:
		Ref<Database> db;
		sqlite3_stmt* stmt;
		List<String> listColumns;
		Map<String, sl_int32> mapColumns;

		SLIB_INLINE _DatabaseCursor(const Ref<Database> _db, sqlite3_stmt* _stmt)
		{
			db = _db;
			stmt = _stmt;
			sl_int32 cols = sqlite3_column_count(stmt);
			for (sl_int32 i = 0; i < cols; i++) {
				const char* buf = sqlite3_column_name(stmt, (int)i);
				String name(buf);
				listColumns.push(name);
				mapColumns.put(name, i);
			}
		}

		~_DatabaseCursor()
		{
			sqlite3_finalize(stmt);
		}

		sl_uint32 getColumnCount()
		{
			return (sl_uint32)(listColumns.count());
		}

		String getColumnName(sl_uint32 index)
		{
			return listColumns.getItemValue(index, String::null());
		}

		sl_int32 getColumnIndex(const String& name)
		{
			return mapColumns.getValue(name, -1);
		}

		Map<String, Variant> getRow()
		{
			return _getRow(stmt, listColumns.getBuffer(), (sl_uint32)(listColumns.getCount()));
		}

		Variant getColumnValue(sl_uint32 index)
		{
			String value = String::fromUtf8(sqlite3_column_text(stmt, (int)index));
			return value;
		}

		Variant getColumnValue(const String& name)
		{
			sl_int32 index = mapColumns.getValue(name, -1);
			if (index >= 0) {
				String value = String::fromUtf8(sqlite3_column_text(stmt, (int)index));
				return value;
			} else {
				return Variant::null();
			}
		}

		sl_bool moveNext()
		{
			sl_int32 nRet = sqlite3_step(stmt);
			if (nRet == SQLITE_ROW) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
	};

	Ref<DatabaseCursor> executeQuery(const String& _query)
	{
		if (!m_flagOpened) {
			return Ref<DatabaseCursor>::null();
		}

		Ref<DatabaseCursor> ret;

		sqlite3_stmt* stmt;

		String8 query = _query;
		if (SQLITE_OK == sqlite3_prepare_v2(m_db, query.getBuf(), -1, &stmt, sl_null)) {	
			ret = new _DatabaseCursor(this, stmt);
		}
		return ret;
	}

	List< Map<String, Variant> > getListForQueryResult(const String& _query)
	{
		if (!m_flagOpened) {
			return List< Map<String, Variant> >::null();
		}

		List< Map<String, Variant> > ret;

		sqlite3_stmt* stmt;
		String8 query = _query;
		if (SQLITE_OK == sqlite3_prepare_v2(m_db, query.getBuf(), -1, &stmt, sl_null)) {
			List<String> columns = _getColumnNames(stmt);
			String* bufColumns = columns.getBuffer();
			sl_uint32 nColumns = (sl_uint32)(columns.getCount());
			while (sl_true) {
				sl_int32 nRet = sqlite3_step(stmt);
				if (nRet == SQLITE_ROW) {
					Map<String, Variant> row = _getRow(stmt, bufColumns, nColumns);
					ret.add(row);
				} else {
					break;
				}
			}
			sqlite3_finalize(stmt);
		}

		return ret;
	}

	Map<String, Variant> getRecordForQueryResult(const String& _query)
	{
		Map<String, Variant> ret;
		if (!m_flagOpened) {
			return ret;
		}
		sqlite3_stmt* stmt;
		String8 query = _query;
		if (SQLITE_OK == sqlite3_prepare_v2(m_db, query.getBuf(), -1, &stmt, sl_null)) {
			List<String> columns = _getColumnNames(stmt);
			sl_int32 nRet = sqlite3_step(stmt);
			if (nRet == SQLITE_ROW) {
				ret = _getRow(stmt, columns.getBuffer(), (sl_uint32)(columns.getCount()));
			}
			sqlite3_finalize(stmt);
		}
		return ret;
	}
	
	Variant getValueForQueryResult(const String& _query)
	{
		Variant ret;
		if (!m_flagOpened) {
			return ret;
		}
		sqlite3_stmt* stmt;
		String8 query = _query;
		if (SQLITE_OK == sqlite3_prepare_v2(m_db, query.getBuf(), -1, &stmt, sl_null)) {
			sl_int32 nRet = sqlite3_step(stmt);
			if (nRet == SQLITE_ROW) {
				ret = String::fromUtf8(sqlite3_column_text(stmt, 0));
			}
			sqlite3_finalize(stmt);
		}
		return ret;
	}
};

Ref<SqliteDatabase> SqliteDatabase::connect(const String& path)
{
	Ref<SqliteDatabase> ret = Ref<SqliteDatabase>::from(_Sqlite3Database::connect(path));
	return ret;
}
SLIB_DATABASE_NAMESPACE_END
