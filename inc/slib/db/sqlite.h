#ifndef CHECKHEADER_SLIB_DB_SQLITE
#define CHECKHEADER_SLIB_DB_SQLITE

#include "database.h"

namespace slib
{

	class SLIB_EXPORT SQLiteDatabase : public Database
	{
		SLIB_DECLARE_OBJECT

	protected:
		SQLiteDatabase();

		~SQLiteDatabase();

	public:
		static Ref<SQLiteDatabase> connect(const String& filePath);

	};

}

#endif
