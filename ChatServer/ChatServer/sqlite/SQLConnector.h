#pragma once
#include "sqlite\sqlite3.h"
#include <mutex>
#include <set>

class SQLConnector
{
	sqlite3 *db;

	std::mutex dataMutex;
	std::set<std::pair<std::string, std::string>> data;

public:
	SQLConnector();

	void getLogins();

	bool isValidLogin(const std::string &login, const std::string &password)
	{
		std::unique_lock<std::mutex> lock(dataMutex);
		return data.find(make_pair(login, password)) != data.end();
	}

	~SQLConnector() { sqlite3_close(db); }
};