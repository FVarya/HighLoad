#include <iostream>
#include <boost/thread.hpp>
#include <vector>
#include "request.h"
#include "server.h"

void Server::startAccept() {
	boost::shared_ptr<Connection> connection(new Connection(io_service_, request_handler,
		[this](const std::string& reason) {
		std::cout << reason << std::endl;
	}));

	acceptor.async_accept(connection->sock(), boost::bind(&Server::handleAccept, this, connection, placeholders::error));
}

void Server::handleAccept(boost::shared_ptr<Connection> connection, const boost::system::error_code& e) {
	if (!e) {
		connection->start();
	}

	startAccept();
}
Server::Server(const std::string& address, const int& port,
	std::size_t threadPoolSize, const std::string& rootDir)
  : threadPoolSize(threadPoolSize),
	acceptor(io_service_),
	request_handler(rootDir)
{


	ip::tcp::endpoint ep(ip::address::from_string(address), port);
	
	acceptor.open(ep.protocol());
	acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	acceptor.bind(ep);
	acceptor.listen();

	startAccept();
}

void Server::start() {
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < threadPoolSize; ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(
			boost::bind(&io_service::run, &io_service_)));
		threads.push_back(thread);
	}

	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}

void Server::stop() {
	io_service_.stop();
}
