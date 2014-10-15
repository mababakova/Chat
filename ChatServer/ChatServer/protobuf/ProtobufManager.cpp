#include "ProtobufManager.h"

MessageType ProtobufManager::getMessageType(const std::string &message)
{
	ChatMessage chatMessage;
	chatMessage.ParseFromString(message);
	return (chatMessage.HasExtension(login) ? MessageType::Login :
		(chatMessage.HasExtension(msg) ? MessageType::Message : MessageType::Incorrect));
}

std::pair<std::string, std::string> ProtobufManager::getLogin(const std::string &message)
{
	ChatMessage chatMessage;
	chatMessage.ParseFromString(message);
	return std::make_pair(chatMessage.GetExtension(login).login(), chatMessage.GetExtension(login).password());
}

std::string ProtobufManager::createLoginAnswer(bool result)
{
	ChatMessage chatMessage;
	chatMessage.MutableExtension(loginResult)->set_result((result ? LoginResult_Result::LoginResult_Result_OK : LoginResult_Result::LoginResult_Result_NOT_OK));
	return chatMessage.SerializeAsString();
}