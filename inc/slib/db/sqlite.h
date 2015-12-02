#ifndef CHECKHEADER_SLIB_DATABASE_SQLITE
#define CHECKHEADER_SLIB_DATABASE_SQLITE

#include "db.h"

SLIB_DATABASE_NAMESPACE_BEGIN

class SLIB_EXPORT SqliteDatabase : public Database
{
	SLIB_DECLARE_OBJECT(SqliteDatabase, Database)
protected:
	SqliteDatabase();

public:
	static Ref<SqliteDatabase> connect(const String& filePath);
};

SLIB_DATABASE_NAMESPACE_END

#endif
