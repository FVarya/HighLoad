#ifndef REPLY_H
#define REPLY_H


#include <unordered_map>
#include <boost/asio.hpp>

struct reply{
	
	enum status_type{
		ok = 200,
		forbidden = 403,
		not_found = 404,
		not_allowed = 405,
		not_implemented = 501
	} status;



	/*std::unordered_map<std::string, std::string> mime_types = {
                    { "txt", "text/plain" },
                    { "html", "text/html" },
                    { "css", "text/css" },
                    { "js", "application/javascript" },
                    { "jpg", "image/jpeg" },
                    { "jpeg", "image/jpeg" },
                    { "png", "image/png" },
                    { "gif", "image/gif" },
                    { "swf", "application/x-shockwave-flash" }
        };*/


	static std::string message_headers();

	static std::string build_headers(int length, const std::string& ext, const std::string &protocol, status_type status);

};
#endif
