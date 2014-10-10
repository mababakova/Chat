#pragma once
#include <boost\asio.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;

class Socket
{
	static Socket *instance;
	typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
	typedef boost::system::error_code error_code;

	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];

	io_service service;
	socket_ptr sock;
	ip::tcp::endpoint *ep;

	std::string login, pwd;
	const std::string delimeter = "\r\n";

	std::function<void(bool)> connectionCallback;
	std::function<void(std::string&)> readMessageCallback;

public:
	Socket(const std::string &address, const std::string &port, std::function<void(bool)> connectionCallback, std::function<void(std::string&)> readMessageCallback);

	static Socket *getInstance(const std::string &address = "", const std::string &port = "", 
		std::function<void(bool)> connectionCallback = nullptr, std::function<void(std::string&)> readMessageCallback = nullptr);

	void onConnect(const error_code &err);

	void onWrite(const error_code &err, size_t bytes);

	void write(const std::string &message);

	void read();

	void onRead(const error_code &err, size_t bytes);

	size_t onReadComplete(const error_code &err, size_t bytes);
};