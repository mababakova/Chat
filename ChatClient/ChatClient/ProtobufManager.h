#pragma once
#include "protobuf\ChatMessage.pb.h"

using namespace chat;

enum MessageType { Login = 1, Message };

class ProtobufManager
{

public:
	ProtobufManager() {}

	std::string getLoginMessage(const std::string &login, const std::string &pwd);

	std::string getMessage(const std::string &login, const std::string &data);

	std::pair<std::string, std::string> getMessageFromProtobuf(const std::string &message);

	MessageType getMessageType(const std::string &message);

	bool getLoginResult(const std::string &message);
};