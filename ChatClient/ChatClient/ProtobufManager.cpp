#include "ProtobufManager.h"

std::string ProtobufManager::getLoginMessage(const std::string &lg, const std::string &pwd)
{
	ChatMessage message;
	message.MutableExtension(login)->set_login(lg);
	message.MutableExtension(login)->set_password(pwd);
	return message.SerializeAsString();
}

std::string ProtobufManager::getMessage(const std::string &lg, const std::string &data)
{
	ChatMessage message;
	message.MutableExtension(msg)->set_login(lg);
	message.MutableExtension(msg)->set_msg(data);
	return message.SerializeAsString();
}

std::pair<std::string, std::string> ProtobufManager::getMessageFromProtobuf(const std::string &message)
{
	ChatMessage data;
	data.ParseFromString(message);
	return std::make_pair(data.GetExtension(msg).login(), data.GetExtension(msg).msg());
}

MessageType ProtobufManager::getMessageType(const std::string &message)
{
	ChatMessage msg;
	msg.ParseFromString(message);
	return (msg.HasExtension(loginResult) ? MessageType::Login : MessageType::Message);
}

bool ProtobufManager::getLoginResult(const std::string &message)
{
	ChatMessage msg;
	msg.ParseFromString(message);
	return (msg.GetExtension(loginResult).result() == LoginResult_Result::LoginResult_Result_OK);
}