#include <iostream>
#include <sstream>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "reply.h"

namespace status_strings {

	const std::string ok =
	  "200 OK\r\n";
	const std::string forbidden =
	  "403 Forbidden\r\n";
	const std::string not_found =
	  "404 Not Found\r\n";
	const std::string not_allowed =
	  "405 Method Not Allowed\r\n";
	const std::string not_implemented =
	  "501 Not Implemented\r\n";


	std::string to_string(reply::status_type status){
	  switch (status){
		  case reply::ok:
		    return ok;
		  case reply::forbidden:
		    return forbidden;
		  case reply::not_found:
		    return not_found;
		  case reply::not_allowed:
		    return not_allowed;
		  case reply::not_implemented:
		    return not_implemented;
		  default:
		    return not_implemented;
	  	}
	}	
}

namespace content_strings {
	//const char ok[] = "";
	const std::string forbidden =
	  "<html>"
	  "<head><title>Forbidden</title></head>"
	  "<body><h1>403 Forbidden</h1></body>"
	  "</html>";
	const std::string not_found =
	  "<html>"
	  "<head><title>Not Found</title></head>"
	  "<body><h1>404 Not Found</h1></body>"
	  "</html>";
	const std::string not_allowed =
	  "<html>"
	  "<head><title>Not Allowed</title></head>"
	   "<body><h1>405 Method Not Allowed</h1></body>"
	   "</html>";
	const std::string not_implemented =
	  "<html>"
	  "<head><title>Not Implemented</title></head>"
	  "<body><h1>501 Not Implemented</h1></body>"
	  "</html>";

	std::string to_string(reply::status_type status){
	  switch (status){
		  case reply::forbidden:
		    return forbidden;
		  case reply::not_found:
		    return not_found;
		  case reply::not_allowed:
		    return not_allowed;
		  case reply::not_implemented:
		    return not_implemented;
		  default:
		    return not_implemented;
	  	}
	}
}

namespace mime_types{
	struct mapping
	{
	  const char* extension;
	  const char* mime_type;
	} mappings[] =
	{
	  { "gif", "image/gif" },
	  { "htm", "text/html" },
	  { "html", "text/html" },
	  { "jpg", "image/jpeg" },
	  { "png", "image/png" },
	  { "css", "text/css"}, 
	  { "js", "application/javascript" },
	  { "jpeg", "image/jpeg" },
          { "swf", "application/x-shockwave-flash" },
	  { 0, 0 } 
	};

	std::string extension_to_type(const std::string& extension)
	{
	  for (mapping* m = mappings; m->extension; ++m)
	  {
	    if (m->extension == extension)
	    {
	      return m->mime_type;
	    }
	  }

	  return "text/plain";
	}
}

std::string reply::message_headers() {
    std::stringstream headers;
    headers.imbue(std::locale(headers.getloc(), new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S GMT")));
    headers << "Server: HigloadWebServer/1.0 (Linux)\r\n"
            << "Date: " << boost::posix_time::second_clock::universal_time() << "\r\n"
            << "Connection: close\r\n";
    return headers.str();
}

std::string reply::build_headers(int length, const std::string& ext, const std::string &protocol, reply::status_type status){
	/*std::ostringstream headers;
	headers << "Content-Length: " << length << "\r\n"
    		<< "Content-Type: " << mime_types[ext] << "\r\n";*/

	std::stringstream response;

	const std::string content = content_strings::to_string(status);
	const int size =  (length == -1) ? content.size() : length;	
	
	response << protocol << ' ' 
		 << status_strings::to_string(status) 
		 << message_headers()
		 << "Content-Length: " << size << "\r\n"
		 << "Content-Type: " << mime_types::extension_to_type(ext) << "\r\n\r\n";


	if(status != ok){
		response << content;		
	}
	const std::string str = response.str();
        const char* ch = str.c_str();
	return response.str();

	/*char buffer[1024];
	response.read(buffer, 1024);
        return boost::asio::buffer(buffer, response.gcount());*/
}
