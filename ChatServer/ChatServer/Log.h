#pragma once
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/shared_ptr.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/log/support/date_time.hpp>
#include <boost/lexical_cast.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

#define logger Log::getInstance()

class Log
{
private:
	typedef boost::shared_ptr<std::string> message_ptr;
	typedef boost::shared_ptr<src::severity_logger< logging::trivial::severity_level > > log_ptr;

	log_ptr log;
	message_ptr message;
	static Log *instance;
public:
	Log();

	static Log *getInstance();

	inline void printArgs(logging::trivial::severity_level level, const std::string & format, std::size_t offset)
	{
		*message += format.substr(offset);
		BOOST_LOG_SEV(*log, level) << *message;
	}

	template<class T, class... Args>
	void printArgs(logging::trivial::severity_level level, const std::string & format, std::size_t offset, const T & value, Args... args)
	{
		for (std::size_t i = offset; i < format.size(); ++i)
		{
			if (format[i] == '%')
			{
				*message += boost::lexical_cast<std::string>(value);
				return printArgs(level, format, i + 1, args...);
			}
			else
				*message += format[i];
		}
		BOOST_LOG_SEV(*log, level) << *message;
	}

	template<class... Args>
	void print(logging::trivial::severity_level level, const std::string & format, Args... args)
	{
		*message = "";
		printArgs(level, format, 0, args...);
	}

	template<class... Args>
	void trace(const std::string &format, Args... args) { print(logging::trivial::trace, format, args...); }

	template<class... Args>
	void debug(const std::string &format, Args... args) { print(logging::trivial::debug, format, args...); }

	template<class... Args>
	void info(const std::string &format, Args... args) { print(logging::trivial::info, format, args...); }

	template<class... Args>
	void error(const std::string &format, Args... args) { print(logging::trivial::error, format, args...); }

	template<class... Args>
	void fatal(const std::string &format, Args... args) { print(logging::trivial::fatal, format, args...); }
};