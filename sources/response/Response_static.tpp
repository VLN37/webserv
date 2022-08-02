//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

Response::status_map Response::statuslist = Response::init_status_map();
Response::status_map Response::init_status_map(void) {
  status_map _map;

  _map[200] = "OK\n";
  _map[201] = "Created\n";
  _map[202] = "Accepted\n";
  _map[300] = "Multiple Choice\n";
  _map[301] = "Moved Permanently\n";
  _map[302] = "Found\n";
  _map[400] = "Bad Request\n";
  _map[401] = "Unauthorized\n";
  _map[403] = "Forbidden\n";
  _map[404] = "Not Found\n";
  _map[405] = "Method Not Allowed\n";
  _map[413] = "Request Entity Too Large\n";
  _map[415] = "Unsupported Media Type\n";
  _map[500] = "Internal Server Error\n";
  _map[502] = "Bad Gateway\n";
  _map[504] = "Gateway Timeout\n";
  _map[506] = "Http Version Unsupported\n";
  return _map;
}

Response::mimetypes_map Response::mimetypes = Response::init_mimetypes();
Response::mimetypes_map Response::init_mimetypes(void) {
  mimetypes_map _map;

  _map["text"] = "Content-Type: text/plain\n";
  _map[".html"] = "Content-Type: text/html; charset=utf-8\n";
  _map[".css"] = "Content-Type: text/css; charset=utf-8\n";
  _map[".jpg"] = "Content-type: image/jpg\n";
  _map[".php"] = "Content-Type: text/plain; charset=utf-8\n";
  return _map;
}

Response::meth_map Response::method_map = Response::init_map();
Response::meth_map Response::init_map(void) {
  meth_map _map;

  _map["GET"] = &Response::_get;
  _map["POST"] = &Response::_post;
  _map["DELETE"] = &Response::_delete;
  return _map;
}

Response::function_vector Response::validation_functions = Response::init_pre();
Response::function_vector Response::init_pre(void) {
  function_vector vec;

  vec.push_back(&Response::validate_limit_except);
  vec.push_back(&Response::validate_http_version);
  return vec;
}

Response::function_vector Response::get_functions = Response::init_get();
Response::function_vector Response::init_get(void) {
  function_vector vec;

  vec.push_back(&Response::validate_index);
  vec.push_back(&Response::validate_folder);
  vec.push_back(&Response::validate_path);
  return vec;
}