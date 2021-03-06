#include "Socket.h"
#include "boost\thread.hpp"

Socket *Socket::instance = nullptr;

Socket::Socket(const std::string &address, const std::string &port, std::function<void(bool)> connectionCallback, std::function<void(std::string&)> readMessageCallback) :
sock(new ip::tcp::socket(service)),
connectionCallback(connectionCallback), 
readMessageCallback(readMessageCallback)
{
	// Connect to server
	ep = new ip::tcp::endpoint(ip::address::from_string(address), stoi(port));
	sock->async_connect(*ep, boost::bind(&Socket::onConnect, this, _1));
	boost::scoped_ptr<boost::thread> bt;
	bt.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &service)));
}

Socket *Socket::getInstance(const std::string &address, const std::string &port, std::function<void(bool)> connectionCallback, std::function<void(std::string&)> readMessageCallback)
{
	if (instance == nullptr)
	{
		instance = new Socket(address, port, connectionCallback, readMessageCallback);
	}
	return instance;
}

void Socket::onConnect(const error_code &err)
{
	connectionCallback(!err);
	read();
	if (err)
	{
		Sleep(1000);
		sock->async_connect(*ep, boost::bind(&Socket::onConnect, this, _1));
	}
}

void Socket::write(const std::string &msg)
{
	std::string message = std::to_string(msg.size()) + delimeter + msg;
	std::copy(message.begin(), message.end(), write_buffer_);
	sock->async_write_some(buffer(write_buffer_, message.size()), boost::bind(&Socket::onWrite, this, _1, _2));
}

void Socket::onWrite(const error_code &err, size_t bytes)
{
	int i = 0;
}

void Socket::read()
{
	async_read(*sock, buffer(read_buffer_),
		boost::bind(&Socket::onReadComplete, this, _1, _2),
		boost::bind(&Socket::onRead, this, _1, _2));
}

void Socket::onRead(const error_code &err, size_t bytes)
{
	if (err) return;
	std::string msg(read_buffer_, bytes);
	msg.erase(msg.begin(), msg.begin() + msg.find(delimeter) + 2);
	readMessageCallback(msg);
	read();
}

size_t Socket::onReadComplete(const error_code &err, size_t bytes)
{
	if (err) return 0;
	size_t pos;
	if ((pos = std::string(read_buffer_, bytes).find(delimeter)) != std::string::npos)
	{
		size_t volume = stoi(std::string(read_buffer_, bytes).substr(0, pos));
		return bytes == (volume + pos + 2) ? 0 : 1;
	}
	return 1;
}