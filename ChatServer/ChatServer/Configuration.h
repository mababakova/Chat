#pragma once
#include "boost/property_tree/ini_parser.hpp"

class Configuration
{
private:
	static Configuration *instance;
	std::string path;
	boost::property_tree::ptree config;
	const std::string fileName = "ChatServer.ini";

	const std::string settingsSection = "settings", dbSection = "db";
	const std::string addressField = "address", portField = "port", maxConnectionNumberField = "maxConnectionNumber", logField = "logPath";
	const std::string dbField = "db", tableField = "table";

	// Settings section
	std::string address, logPath;
	int port, maxConnectionNumber;

	// DB section
	std::string db, table;
public:

	Configuration(){};

	Configuration(const std::string &path);

	Configuration& operator=(const Configuration &obj)
	{
		address = obj.address; port = obj.port; maxConnectionNumber = obj.maxConnectionNumber;
		db = obj.db; table = obj.table;
		return *this;
	};

	static Configuration *getInstance(const std::string &path = "");

	std::string getPath() { return this->path; }

	inline std::string getLogPath() { return this->logPath; }
	inline std::string getAddress() { return this->address; }
	inline int getPort() { return this->port; }

	std::string getDB() { return this->db; }
	std::string getTable() { return this->table; }
};