#include "ClientSocket.h"
#include "Log.h"

void ClientSocket::start(std::function<void(boost::shared_ptr<ClientSocket>, const std::string &, const std::string &)> loginCallback,
	std::function<void(boost::shared_ptr<ClientSocket>)> disconnectCallback,
	std::function<void(std::string &)> readCallback)
{
	this->loginCallback = loginCallback;
	this->disconnectCallback = disconnectCallback;
	this->readCallback = readCallback;
	logger->trace("Start read client login/pwd");
	read();
}

void ClientSocket::read()
{
	async_read(sock, buffer(read_buffer_),
		boost::bind(&ClientSocket::onReadComplete, shared_from_this(), _1, _2),
		boost::bind(&ClientSocket::onRead, shared_from_this(), _1, _2));
}

void ClientSocket::onRead(const error_code &err, size_t bytes)
{
	logger->info("% bytes were read", bytes);
	if (err)
	{
		stop();
		return;
	}
	std::string msg(read_buffer_, bytes);
	msg.erase(msg.begin(), msg.begin() + msg.find(delimeter) + 2);
	logger->info("Message: %", msg);
	switch (protobuf.getMessageType(msg))
	{
	case MessageType::Login:
	{
							   logger->info("Get login/pwd from client");
							   std::pair<std::string, std::string> loginPair = protobuf.getLogin(msg);
							   this->login = loginPair.first;
							   loginCallback(shared_from_this(), loginPair.first, loginPair.second);
							   break;
	}
	case MessageType::Message:
	{
								 logger->info("Get message from client");
								 readCallback(msg);
								 break;
	}
	}
	read();
}

size_t ClientSocket::onReadComplete(const error_code &err, size_t bytes)
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

void ClientSocket::write(const std::string &message)
{
	std::string msg = std::to_string(message.size()) + delimeter + message;
	std::copy(msg.begin(), msg.end(), write_buffer_);
	async_write(sock, buffer(write_buffer_, msg.size()), boost::bind(&ClientSocket::onWrite, shared_from_this(), _1, _2));
}

void ClientSocket::writeLoginAnswer(bool answer)
{
	std::string message = protobuf.createLoginAnswer(answer);
	write(message);
}

void ClientSocket::onWrite(const error_code &err, size_t bytes)
{
	logger->debug("% bytes were sent to client", bytes);
}