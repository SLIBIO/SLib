#ifndef CHECKHEADER_SLIB_DB_SQLITE
#define CHECKHEADER_SLIB_DB_SQLITE

#include "database.h"

SLIB_DB_NAMESPACE_BEGIN

class SLIB_EXPORT SQLiteDatabase : public Database
{
	SLIB_DECLARE_OBJECT(SQLiteDatabase, Database)
protected:
	SLIB_INLINE SQLiteDatabase()
	{		
	}

public:
	static Ref<SQLiteDatabase> connect(const String& filePath);
};

SLIB_DB_NAMESPACE_END

#endif
