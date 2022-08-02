//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include <iostream>
#include <map>

struct Request {
  std::map<std::string, std::string> headers;
  std::string method;
  std::string path;
  std::string http_version;
  std::string host;
  std::string body;
  unsigned int error;
  int body_size;
  int finished;

  Request();

  ~Request();

  bool is_valid() const ;

};

std::ostream& operator<<(std::ostream& out, const Request& request);
#endif // !HTTP_REQUEST_HPP
