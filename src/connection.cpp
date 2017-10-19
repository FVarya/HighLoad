#include <boost/thread.hpp>
#include "connection.h"


void Connection::doRead() {
	socket.async_receive(buffer(readBuffer), strand.wrap(boost::bind(&Connection::onRead, shared_from_this(), placeholders::error, placeholders::bytes_transferred)));
}

void Connection::onRead(const boost::system::error_code& err, std::size_t bytes) {
	if (!err){
		request_handler.handle_request(std::string(readBuffer, bytes), std::bind(&Connection::doWrite, shared_from_this(), std::placeholders::_1));
	}
	close();
}

void Connection::doWrite(const std::string& message){
	socket.write_some(boost::asio::buffer(message));
}

void Connection::close() {
	if (socket.is_open()) socket.close();
}

Connection::Connection(io_service& io_service,const RequestHandler& request_handler)
	: socket(io_service),
	request_handler(request_handler),
	strand(io_service){};

void Connection::start(){
	doRead();
}

ip::tcp::socket& Connection::sock() {
	return socket;
}



