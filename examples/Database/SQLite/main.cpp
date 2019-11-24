#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	SQLiteParam param;
	param.path = System::getHomeDirectory() + "/test.sqlite";
	param.encryptionKey = "test";
	Ref<SQLiteDatabase> db = SQLiteDatabase::connect(param);
	if (db.isNull()) {
		Println("Cannot connect to database file.");
		return 1;
	}
	
	db->execute(
		R"sql(
			DROP TABLE IF EXISTS book;
			CREATE TABLE book
			(
			 title TEXT,
			 abstract TEXT
			);
		)sql"
	);
	
	for (int i = 0; i < 1000; i++) {
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
