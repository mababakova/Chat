#include "Configuration.h"

Configuration* Configuration::instance = 0;

Configuration::Configuration(const std::string &path)
{
	// Read ini-file
	this->path = path;
	try
	{
		boost::property_tree::ini_parser::read_ini(path + fileName, config);
	}
	catch (boost::property_tree::ini_parser_error e)
	{
		throw e.what();
	}

	try
	{
		// Save data from settings section
		address = config.get<std::string>(settingsSection + "." + addressField);
		port = stoi(config.get<std::string>(settingsSection + "." + portField));
		maxConnectionNumber = stoi(config.get<std::string>(settingsSection + "." + maxConnectionNumberField));
		logPath = config.get<std::string>(settingsSection + "." + logField);

		// Save data from db section
		db = config.get<std::string>(dbSection + "." + dbField);
		table = config.get<std::string>(dbSection + "." + tableField);
	}
	catch (std::exception &e)
	{
		throw e.what();
	}
}

Configuration* Configuration::getInstance(const std::string &path)
{
	if (!instance)
	{
		instance = new Configuration(path);
	}
	return instance;
}