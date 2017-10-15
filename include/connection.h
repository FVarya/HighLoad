#ifndef CONNECTION_H
#define CONNECTION_H


#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request.h"

using namespace boost::asio;

#define BUFF_SIZE 1024

class Connection : public boost::enable_shared_from_this<Connection>, private boost::noncopyable {
private:
	ip::tcp::socket socket;
	std::function<void(const std::string&)> abortedCallback;
	RequestHandler& request_handler;
	char readBuffer[BUFF_SIZE];
	std::vector<const_buffer> writeBuffer;
	boost::asio::io_service::strand strand;

	void doRead();

	void onRead(const boost::system::error_code& err, std::size_t bytes);

	void doWrite(std::vector<const_buffer> msg);

	void onWrite(const boost::system::error_code& e, std::size_t bytes);

	void close(const std::string& reason);
public: 
	Connection(io_service& io_service,RequestHandler& request_handler, std::function<void(const std::string&)> abortedCallback);

	void start();

	ip::tcp::socket& sock();
};
#endif



