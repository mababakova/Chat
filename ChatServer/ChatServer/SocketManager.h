#pragma once
#include <boost\asio.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\bind.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "ClientSocket.h"
#include "sqlite\SQLConnector.h"

using namespace boost::asio;

class SocketManager
{
private:
	ip::tcp::acceptor *acceptor;
	SQLConnector sqlConnector;

	// Client list
	std::mutex clientsMutex;
	std::vector<boost::shared_ptr<ClientSocket> > clients;

	typedef boost::shared_ptr<ip::tcp::acceptor> acceptor_ptr;
	typedef boost::shared_ptr<ip::tcp::endpoint> endpoint_ptr;

	typedef boost::system::error_code error_code;

	ip::tcp::endpoint *ep;
	
public:
	SocketManager();

	void start();

	void startAccept();

	void handleAccept(boost::shared_ptr<ClientSocket> client, const error_code & err);

	void onLogin(boost::shared_ptr<ClientSocket> client, const std::string &login, const std::string &pwd);

	void onDisconnect(boost::shared_ptr<ClientSocket> client);

	void messageDelivery(const std::string &message);
};