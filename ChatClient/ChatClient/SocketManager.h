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
	std::function<void(std::string &, std::string &)> readCallback;
	ProtobufManager protobuf;
	Socket *socket;
	std::string login;
public:
	SocketManager(){}
	SocketManager(const std::string &address, const std::string &port, std::function<void(bool)> connectionCallback, std::function<void(bool)> loginCallback);
	static SocketManager* getInstance(const std::string &address = "", const std::string &port = "", 
		std::function<void(bool)> connectionCallback = nullptr, std::function<void(bool)> loginCallback = nullptr);
	void autorize(const std::string &login, const std::string &pwd);
	void onConnection(bool answer);
	void onRead(std::string &message);
	void write(std::string &data, std::function<void(std::string &, std::string &)> readCallback);
	void write(std::string &data);
};