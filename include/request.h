#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <boost/asio.hpp>
#include "reply.h"

class RequestHandler {
    static const unsigned BUFFER_SIZE = 16384;

    std::string root_dir;

    void GET(const std::string &url, const std::string &protocol,
             std::function<void (const std::string&)> writeCallback);
    void HEAD(const std::string &url, const std::string &protocol,
             std::function<void (const std::string&)> writeCallback);
    void NotAllowed(const std::string &protocol,
                    std::function<void (const std::string&)> writeCallback);

    std::string url_decode(const std::string& url);
    bool file_exists(const std::string& path) ;
    size_t file_size(const std::string& path) ;
    std::string get_extension(const std::string& url) ;
    bool is_directory(const std::string& path) ;

public:
    void handle_request(const std::string& request, std::function<void (const std::string&)> writeCallback) ;

    RequestHandler(const std::string& dir);
};
#endif
