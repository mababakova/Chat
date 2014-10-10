#pragma once
#include <iostream>
#include <functional>
#include "ProtobufManager.h"
#include "Socket.h"

class SocketManager
{
	static SocketManager *instance;
	std::function<void(bool)> connectionCallback;
	std::function<void(bool)> loginCallback;
	ProtobufManager protobuf;
	Socket *socket;
public:
	SocketManager(){}
	SocketManager(const std::string &address, const std::string &port, std::function<void(bool)> connectionCallback, std::function<void(bool)> loginCallback);
	static SocketManager* getInstance(const std::string &address = "", const std::string &port = "", 
		std::function<void(bool)> connectionCallback = nullptr, std::function<void(bool)> loginCallback = nullptr);
	void autorize(const std::string &login, const std::string &pwd);
	void onConnection(bool answer);
	void onRead(std::string &message);
};