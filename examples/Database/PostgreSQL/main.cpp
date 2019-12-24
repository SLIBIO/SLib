#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	PostgreSQL::Param param;
	param.host = "localhost";
	param.user = "postgres";
	param.db = "test";
	
	Ref<Database> db = PostgreSQL::connect(param);
	if (db.isNull()) {
		Println("Cannot connect to PostgreSQL server");
		return -1;
	}
	
	Println("test database existing: %s", db->isDatabaseExisting("test"));
	Println("Databases=%s", Json(db->getDatabases()));

	Println("book table existing: %s", db->isTableExisting("book"));
	Println("Tables=%s", Json(db->getTables()));
	
	db->dropTable("book", DatabaseFlags::IfExists);
	{
		DatabaseColumnDefinition columns[] = {
			DatabaseColumnDefinition("title", DatabaseColumnType::TEXT),
			DatabaseColumnDefinition("abstract", DatabaseColumnType::TEXT)
		};
		db->createTable("book", columns, DatabaseFlags::IfExists);
	}
	
	db->startTransaction();
	for (int i = 0; i < 1000; i++) {
		String title = String::format("Book %d", i);
		String abstract = String::format("Abstract %d", i);
		HashMap<String, String> map;
		map.put_NoLock("title", title);
		map.put_NoLock("abstract", abstract);
		Println("Insert: %d row", db->insert("book", map));
	}
	db->commitTransaction();
	
	for (auto& row : db->findRecords("book", DatabaseExpression::null())) {
		String title = row["title"].getString();
		String abstract = row["abstract"].getString();
		Println("Title: %s, Abstract: %s", title, abstract);
	}
	
	return 0;
}
