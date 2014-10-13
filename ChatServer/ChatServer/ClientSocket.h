#pragma once
#include <boost\asio.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\bind.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "protobuf/ProtobufManager.h"

using namespace boost::asio;
static io_service service;

class ClientSocket : public boost::enable_shared_from_this<ClientSocket>, boost::noncopyable
{
	ProtobufManager protobuf;

	std::string login;

	ip::tcp::socket sock;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];

	std::function<void(boost::shared_ptr<ClientSocket>, const std::string &, const std::string &)> loginCallback;
	std::function<void(boost::shared_ptr<ClientSocket>)> disconnectCallback;
	std::function<void(std::string &)> readCallback;
	
	const std::string delimeter = "\r\n";
	
	ClientSocket() : sock(service) {}
public:
	typedef boost::system::error_code error_code;

	static boost::shared_ptr<ClientSocket> getInstance() { boost::shared_ptr<ClientSocket> client(new ClientSocket); return client; }

	void start(std::function<void(boost::shared_ptr<ClientSocket>, const std::string &, const std::string &)> loginCallback,
		std::function<void(boost::shared_ptr<ClientSocket>)> disconnectCallback,
		std::function<void(std::string &)> readCallback);
	
	void stop() { sock.close(); disconnectCallback(shared_from_this()); }

	void read();

	void write(const std::string &message);

	void writeLoginAnswer(bool answer);

	void onWrite(const error_code &err, size_t bytes);

	void onRead(const error_code &err, size_t bytes);

	size_t onReadComplete(const error_code &err, size_t bytes);

	ip::tcp::socket &getSocket() { return sock; }

	std::string getLogin() { return login; }
};