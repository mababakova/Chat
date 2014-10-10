#pragma once
#include "protobuf\ChatMessage.pb.h"

using namespace chat;

enum MessageType { Login = 1, Message };

class ProtobufManager
{

public:
	ProtobufManager() {}

	std::string getLoginMessage(const std::string &login, const std::string &pwd);

	MessageType getMessageType(const std::string &message);

	bool getLoginResult(const std::string &message);
};