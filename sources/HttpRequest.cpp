//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpRequest.hpp"
#include "Logger.hpp"
#include "webserv.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

#include <map>
#include <string>
#include <sstream>
#include <cstring>

// TODO: remove this
std::ostream& operator<<(std::ostream& out, const Request& request) {
  out
    << "{\n"
    << "  method: \"" << request.method << "\"\n"
    << "  path: \"" << request.path << "\"\n"
    << "  version: \"" << request.http_version << "\"\n"
    << "  headers: {";

  if (request.headers.empty())
    out << " }\n";
  else {
    out << "\n";
    std::map<std::string, std::string>::const_iterator iter = request.headers.begin();
    for (; iter != request.headers.end(); ++iter) {
      out << "    " << iter->first << ": \"" << iter->second << "\"\n";
    }
    out << "  }\n";
  }
  out << "vvv     body    vvv\n\n";
  out << request.body;
  out << "\n\n^^^ end of body ^^^\n";
  out << "}" << std::endl;
  return out;
}

Request::Request() { }

Request::~Request() { }

bool Request::is_valid() const {
  return this->valid;
}
