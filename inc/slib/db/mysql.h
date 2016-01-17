#ifndef CHECKHEADER_SLIB_DB_MYSQL
#define CHECKHEADER_SLIB_DB_MYSQL

#include "database.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)
#define SLIB_DATABASE_SUPPORT_MYSQL
#endif

#if defined(SLIB_DATABASE_SUPPORT_MYSQL)

SLIB_DB_NAMESPACE_BEGIN

class SLIB_EXPORT MySQL_Param
{
public:
	String host;
	String user;
	String password;
	String db;
	sl_uint32 port;
	sl_bool flagAutoReconnect;

public:
	MySQL_Param();
};

class SLIB_EXPORT MySQL_Database : public Database
{
	SLIB_DECLARE_OBJECT(MySQL_Database, Database)
	
public:
	static Ref<MySQL_Database> connect(const MySQL_Param& param);
	
	static Ref<MySQL_Database> connect(const MySQL_Param& param, String& outErrorMessage);

	static void initThread();
};

SLIB_DB_NAMESPACE_END

#endif

#endif
