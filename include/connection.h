#ifndef CONNECTION_H
#define CONNECTION_H


#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request.h"

using namespace boost::asio;

class Connection : public boost::enable_shared_from_this<Connection>, private boost::noncopyable {
private:
	ip::tcp::socket socket;
	RequestHandler request_handler;
	char readBuffer[1024];
	boost::asio::io_service::strand strand;

	void doRead();

	void onRead(const boost::system::error_code& err, std::size_t bytes);

	void doWrite(const std::string& message);

	void close();
public: 
	Connection(io_service& io_service,const RequestHandler& request_handler);

	void start();

	ip::tcp::socket& sock();
};
#endif



