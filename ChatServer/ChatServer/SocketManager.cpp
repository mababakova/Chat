#include "SocketManager.h"
#include "Log.h"
#include "Configuration.h"
#include <boost/thread.hpp>

SocketManager::SocketManager()
{
	sqlConnector.getLogins();
	ep = new ip::tcp::endpoint(ip::address::from_string(Configuration::getInstance()->getAddress()), Configuration::getInstance()->getPort());
	acceptor = new ip::tcp::acceptor(service, *ep);
	startAccept();
	service.run();
}

void SocketManager::startAccept()
{
	boost::shared_ptr<ClientSocket> client = ClientSocket::getInstance();
	acceptor->async_accept(client->getSocket(), boost::bind(&SocketManager::handleAccept, this, client, _1));
}

void SocketManager::handleAccept(boost::shared_ptr<ClientSocket> client, const error_code & err)
{
	client->start(boost::bind(&SocketManager::onLogin, this, _1, _2, _3), boost::bind(&SocketManager::onDisconnect, this, _1));
	boost::shared_ptr<ClientSocket> newClient = ClientSocket::getInstance();
	acceptor->async_accept(newClient->getSocket(), boost::bind(&SocketManager::handleAccept, this, newClient, _1));
}

void SocketManager::onLogin(boost::shared_ptr<ClientSocket> client, const std::string &login, const std::string &pwd)
{
	bool clientValidation = sqlConnector.isValidLogin(login, pwd);
	if (clientValidation)
		logger->info("Client login is valid");
	else
		logger->error("Client login is NOT valid");
	client->writeLoginAnswer(clientValidation);
	std::lock_guard<std::mutex> lock(clientsMutex);
	clients.push_back(client);
}

void SocketManager::onDisconnect(boost::shared_ptr<ClientSocket> client)
{
	logger->info("Client % disconnected", client->getLogin());
	std::unique_lock<std::mutex> lock(clientsMutex);
	clients.erase(remove(clients.begin(), clients.end(), client));
}