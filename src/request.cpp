#include <fstream>
#include <sys/stat.h>
#include "request.h"

RequestHandler::RequestHandler(const std::string &dir)
        : root_dir(dir) {}

void RequestHandler::handle_request(const std::string &request, std::function<void (const std::string&)> writeCallback) {
    std::istringstream iss(request);
    std::string method, url, protocol = "HTTP/1.1";
    iss >> method >> url >> protocol;
    url = url_decode(url);
    size_t pos;

    while ((pos = url.find("../")) != std::string::npos) {
        url.erase(pos, 3);
    }

    if ((pos = url.find('?')) != std::string::npos) {
        url = url.substr(0, pos);
    }


    if (method == "GET" || method == "") {
        GET(url, protocol, writeCallback);
    } else if (method == "HEAD") {
        HEAD(url, protocol, writeCallback);
    } else if (method == "POST" || method == "PUT" || method == "PATCH" ||
               method == "DELETE" || method == "TRACE" || method == "CONNECT" ||
               method == "OPTIONS") {
        NotAllowed(protocol, writeCallback);
    }
}

void RequestHandler::GET(const std::string &url, const std::string &protocol,
                         std::function<void (const std::string&)> writeCallback) {
    std::string path = root_dir + url;
    bool dir;
    if ((dir = is_directory(path))) {
        if (path.back() != '/') path += '/';
        path += "index.html";
    }

    if (file_exists(path)) {
	std::string ext = dir ? "html" : get_extension(url);
	writeCallback(reply::build_headers(file_size(path), ext, protocol, reply::ok));

        std::ifstream in(path);

        char buffer[BUFFER_SIZE];
        while (size_t count = (size_t)in.readsome(buffer, BUFFER_SIZE)) {
            writeCallback(std::string(buffer, count));
	}
    } else {
        writeCallback(reply::build_headers(-1, "html", protocol, (dir) ? reply::forbidden : reply::not_found));
    }
}

void RequestHandler::HEAD(const std::string &url, const std::string &protocol,
                          std::function<void (const std::string&)> writeCallback) {
    std::string path = root_dir + url;
    bool dir;
    if ((dir = is_directory(path))) {
        if (path.back() != '/') path += '/';
        path += "index.html";
    }

    if (file_exists(path)) {
	std::string ext = dir ? "html" : get_extension(url);
	writeCallback(reply::build_headers(file_size(path), ext, protocol, reply::ok));
    } else {
	writeCallback(reply::build_headers(-1, "html", protocol, (dir) ? reply::forbidden : reply::not_found));
    }
}

void RequestHandler::NotAllowed(const std::string &protocol,
                                std::function<void (const std::string&)> writeCallback) {
    writeCallback(reply::build_headers(-1, "html", protocol, reply::not_allowed));
}

std::string RequestHandler::url_decode(const std::string &url) {
    std::string res;

    for (size_t i = 0; i < url.length(); ++i) {
        if (url[i] == '%') {
            int val;
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &val);
            res += (char)val;
            i += 2;
        } else if (url[i] == '+') {
            res += ' ';
        } else {
            res += url[i];
        }
    }
    return res;
}

bool RequestHandler::is_directory(const std::string& path) {
    struct stat s;
    return (stat(path.c_str(), &s) == 0) && (s.st_mode & S_IFDIR);
}

size_t RequestHandler::file_size(const std::string& path) {
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return (size_t)in.tellg();
}

bool RequestHandler::file_exists(const std::string &path) {
    std::ifstream in(path);
    return in.good();
}

std::string RequestHandler::get_extension(const std::string &url) {
    size_t pos = url.find_last_of('.');
    return (pos != std::string::npos) ? url.substr(pos + 1) : "txt";
}
