//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include <sys/socket.h>
#include <sys/types.h>

#include <sstream>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "HttpRequest.hpp"
#include "HttpBase.hpp"
#include "WebServ.hpp"
#include "Logger.hpp"

struct Request {
  std::map<std::string, std::string> headers;
  std::string method;
  std::string path;
  std::string http_version;
  std::string host;
  std::string body;
  int body_size;
  int finished;

  Request(int fd);

  ~Request();

  bool is_valid() const ;

  Request* receive(int fd);
private:
  int fd;
  bool valid;
  char *raw;
  size_t nbytes;

  std::vector<std::string>* tokenize_request(char *payload);
  void parse_request(std::vector<std::string>* tokens);

};
std::ostream& operator<<(std::ostream& out, const Request& request);
#endif // !HTTP_REQUEST_HPP
