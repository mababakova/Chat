#include "SQLConnector.h"
#include "Configuration.h"

SQLConnector::SQLConnector()
{
	std::string dbPath(Configuration::getInstance()->getPath() + Configuration::getInstance()->getDB() + ".db");
	int retCode = sqlite3_open(dbPath.c_str(), &db);
	if (retCode)
	{
		return;
	}
}

void SQLConnector::getLogins()
{
	sqlite3_stmt *stmt = nullptr;
	std::string command = "select * from " + Configuration::getInstance()->getTable();
	int retCode = sqlite3_prepare_v2(db, command.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string login((char*)sqlite3_column_text(stmt, 1));
		std::string password((char*)sqlite3_column_text(stmt, 2));
		std::unique_lock<std::mutex> lock(dataMutex);
		data.insert(std::make_pair(login, password));
	}
	sqlite3_finalize(stmt);	
}