#include "Log.h"
#include "Configuration.h"

Log* Log::instance = 0;

Log::Log() : log(new src::severity_logger< logging::trivial::severity_level >), message(new std::string(""))
{
	logging::add_file_log
		(
		keywords::open_mode = (std::ios::out | std::ios::app),
		keywords::auto_flush = true,
		keywords::file_name = Configuration::getInstance()->getLogPath() + "ChatServer_%Y-%m-%d.log",
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::format =
		(
		expr::stream
		<< "[" << logging::trivial::severity
		<< "]\t" << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
		<< ":\t" << expr::smessage
		)
		);
	logging::core::get()->set_filter
		(
		logging::trivial::severity >= logging::trivial::trace
		);
	logging::add_common_attributes();
}

Log* Log::getInstance()
{
	if (!instance)
	{
		instance = new Log();
	}
	return instance;
}