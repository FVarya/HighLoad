#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "connection.h"


void Connection::doRead() {
	socket.async_receive(buffer(readBuffer), strand.wrap(boost::bind(&Connection::onRead, shared_from_this(), placeholders::error, placeholders::bytes_transferred)));
}

void Connection::onRead(const boost::system::error_code& err, std::size_t bytes) {
	if (!err)
	{
		request_handler.handle_request(std::string(readBuffer, bytes), strand.wrap(std::bind(&Connection::doWrite, shared_from_this(), std::placeholders::_1)));
	}
	else close("onRead error");
}

void Connection::doWrite(std::vector<const_buffer> msg){
	writeBuffer = std::move(msg);
	socket.async_send(writeBuffer, strand.wrap(boost::bind(&Connection::onWrite, shared_from_this(), placeholders::error, placeholders::bytes_transferred)));		
}

void Connection::onWrite(const boost::system::error_code& e, std::size_t bytes) {
	if (!e){
		socket.close();
	}
}

void Connection::close(const std::string& reason) {
	if (socket.is_open()) socket.close();
		abortedCallback(reason);
}

Connection::Connection(io_service& io_service,RequestHandler& request_handler, std::function<void(const std::string&)> abortedCallback)
	: socket(io_service),
	request_handler(request_handler),
	strand(io_service),
	abortedCallback(abortedCallback){};

void Connection::start(){
	doRead();
}

ip::tcp::socket& Connection::sock() {
	return socket;
}



