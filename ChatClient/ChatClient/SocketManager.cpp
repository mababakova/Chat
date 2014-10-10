#include "SocketManager.h"
#include "ProtobufManager.h"

class Socket;
SocketManager *SocketManager::instance = nullptr;

SocketManager::SocketManager(const std::string &address, const std::string &port, 
	std::function<void(bool)> connectionCallback, std::function<void(bool)> loginCallback):
	connectionCallback(connectionCallback), loginCallback(loginCallback)
{
	socket = Socket::getInstance(address, port, boost::bind(&SocketManager::onConnection, this, _1), boost::bind(&SocketManager::onRead, this, _1));
}

SocketManager* SocketManager::getInstance(const std::string &address, const std::string &port, 
	std::function<void(bool)> connectionCallback, std::function<void(bool)> loginCallback)
{
	if (instance == nullptr)
		instance = new SocketManager(address, port, connectionCallback, loginCallback);
	return instance;
}

void SocketManager::onConnection(bool answer)
{
	connectionCallback(answer);
}

void SocketManager::autorize(const std::string &login, const std::string &pwd)
{
	std::string message = protobuf.getLoginMessage(login, pwd);
	socket->write(message);
}

void SocketManager::onRead(std::string &message)
{
	switch (protobuf.getMessageType(message))
	{
	case MessageType::Login:
	{
							   loginCallback(protobuf.getLoginResult(message));
							   break;
	}
	}
}