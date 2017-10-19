#ifndef REPLY_H
#define REPLY_H

#include <string>

struct reply{
	
	enum status_type{
		ok = 200,
		forbidden = 403,
		not_found = 404,
		not_allowed = 405,
		not_implemented = 501
	} status;

	static std::string message_headers();

	static std::string build_headers(int length, const std::string& ext, const std::string &protocol, status_type status);

};
#endif
