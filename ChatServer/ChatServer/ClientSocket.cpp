#include "ClientSocket.h"
#include "Log.h"

void ClientSocket::start(std::function<void(boost::shared_ptr<ClientSocket>, const std::string &, const std::string &)> loginCallback,
	std::function<void(boost::shared_ptr<ClientSocket>)> disconnectCallback)
{
	this->loginCallback = loginCallback;
	this->disconnectCallback = disconnectCallback;
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
	case MessageType::Message:	logger->info("Get message from client"); break;
	}
	read();
}

size_t ClientSocket::onReadComplete(const error_code &err, size_t bytes)
{
	if (err) return 0;
	bool found = std::string(read_buffer_, bytes).find("\r\n") != std::string::npos;
	return found ? 0 : 1;
}

void ClientSocket::write(const std::string &message)
{
	std::string msg = message + delimeter;
	std::copy(msg.begin(), msg.end(), write_buffer_);
	sock.async_write_some(buffer(write_buffer_, msg.size()), boost::bind(&ClientSocket::onWrite, this, _1, _2));
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