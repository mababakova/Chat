#include "ProtobufManager.h"

MessageType ProtobufManager::getMessageType(const std::string &message)
{
	ChatMessage msg;
	msg.ParseFromString(message);
	return (msg.HasExtension(login) ? MessageType::Login : MessageType::Message);
}

std::pair<std::string, std::string> ProtobufManager::getLogin(const std::string &message)
{
	ChatMessage msg;
	msg.ParseFromString(message);
	return std::make_pair(msg.GetExtension(login).login(), msg.GetExtension(login).password());
}

std::string ProtobufManager::createLoginAnswer(bool result)
{
	ChatMessage msg;
	msg.MutableExtension(loginResult)->set_result((result? LoginResult_Result::LoginResult_Result_OK:LoginResult_Result::LoginResult_Result_NOT_OK));
	return msg.SerializeAsString();
}