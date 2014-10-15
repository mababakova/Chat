#pragma once
#include <iostream>
#include <protobuf\ChatMessage.pb.h>

enum MessageType { Login = 1, Message, Incorrect };

using namespace chat;

class ProtobufManager
{
public:
	ProtobufManager() {}

	MessageType getMessageType(const std::string &message);

	std::pair<std::string, std::string> getLogin(const std::string &message);

	std::string createLoginAnswer(bool result);
};