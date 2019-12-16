#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	MySQL_Param param;
	param.host = "172.20.30.1";
	param.user = "test";
	param.password = "test";
	param.db = "test";
	
	Ref<Database> db = MySQL_Database::connect(param);
	if (db.isNull()) {
		Println("Cannot connect to MySQL server");
		return -1;
	}
	
	Println("test database existing: %s", db->isDatabaseExisting("test"));
	Println("Databases=%s", Json(db->getDatabases()));

	Println("book table existing: %s", db->isTableExisting("book"));
	Println("Tables=%s", Json(db->getTables()));
	
	db->execute(
		R"sql(
			DROP TABLE IF EXISTS book;
			CREATE TABLE book
			(
			 no BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
			 title VARCHAR(60),
			 abstract TEXT,
			 PRIMARY KEY (no)
			);
		)sql"
	);
	
	for (int i = 0; i < 10; i++) {
		String title = String::format("Book %d", i);
		String abstract = String::format("Abstract %d", i);
		db->execute("INSERT INTO book (title, abstract) VALUES(?, ?)", title, abstract);
	}
	
	for (auto& row : db->getListForQueryResult("SELECT * from book")) {
		String title = row["title"].getString();
		String abstract = row["abstract"].getString();
		Println("Title: %s, Abstract: %s", title, abstract);
	}
	
	return 0;
}
