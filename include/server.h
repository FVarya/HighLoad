#ifndef SERVER_H
#define SERVER_H


#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include "connection.h"

using namespace boost::asio;

class Server : private boost::noncopyable {
private:
	std::size_t threadPoolSize;
	io_service io_service_;
	ip::tcp::acceptor acceptor;
	RequestHandler request_handler;

	void startAccept();

	void handleAccept(boost::shared_ptr<Connection> connection, const boost::system::error_code& e);

public:
	Server(const std::string& address, const int& port, std::size_t threadPoolSize, const std::string& rootDir);

	void start();

	void stop();
};
#endif
