//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Config.hpp"

Config::Config(void) { backlog = DFL_BACKLOG; }

Config::Config(const Config& src) { *this = src; }

Config::~Config(void) {}

Config& Config::operator=(const Config& rhs) {
  if (this != &rhs) {
    backlog = rhs.backlog;
    _servers = rhs._servers;
  }
  return (*this);
}

const Server& Config::operator[](size_t n) { return (_servers[n]); }

size_t Config::size(void) { return (_servers.size()); }

void Config::load(char* file) {
  std::istringstream is(_sanitize(_open(file)));
  _parse(&is);
}

std::string Config::_open(char* file) {
  std::ifstream ifs;
  std::stringstream ss;

  ifs.open(file);
  ss << ifs.rdbuf();
  return (ss.str());
}

std::string Config::_sanitize(const std::string& file_content) {
  std::string tmp(file_content);

  String::replace_all(&tmp, "\n", " ");
  String::replace_all(&tmp, "\t", " ");

  String::replace_all(&tmp, "server{", "server {");

  String::replace_all(&tmp, ";", ";\n");
  String::replace_all(&tmp, "{", "{\n");
  String::replace_all(&tmp, "}", "}\n");

  String::replace_unique(&tmp, ' ');

  String::trim_lines(&tmp, " ");

  return (tmp);
}

ServerLocation Config::_parse_location(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;

  ServerLocation location;

  while (std::getline(*is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");
    directive = tokens[0];

    ConfigHelper helper(tokens);

    if (directive == "root") {
      location.root = helper.get_root();
    } else if (directive == "index") {
      location.index = helper.get_index();
    } else if (directive == "limit_except") {
      location.limit_except = helper.get_limit_except();
    } else if (directive == "client_max_body_size") {
      location.client_max_body_size = helper.get_client_max_body_size();
    } else if (directive == "autoindex") {
      location.autoindex = helper.get_autoindex();
    } else if (directive == "cgi") {
      location.cgi["." + tokens[1]] = helper.get_cgi();
    } else if (directive == "return") {
      location.redirect = helper.get_redirect();
    } else if (directive[0] == '#') {
      continue;
    } else if (directive == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(directive);
    }
  }

  return (location);
}

Server Config::_parse_server(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;

  Server srv;

  while (std::getline(*is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");
    directive = tokens[0];

    ConfigHelper helper(tokens);

    if (directive == "listen") {
      srv.ip = helper.get_listen().first;
      srv.port = helper.get_listen().second;
    } else if (directive == "server_name") {
      srv.server_name = helper.get_server_name();
    } else if (directive == "root") {
      srv.root = helper.get_root();
    } else if (directive == "index") {
      srv.index = helper.get_index();
    } else if (directive == "error_page") {
      int code = String::to_int(tokens[1]);
      srv.error_page[code] = helper.get_error_page();
    } else if (directive == "timeout") {
      srv.timeout = helper.get_timeout();
    } else if (directive == "client_max_body_size") {
      srv.client_max_body_size = helper.get_client_max_body_size();
    } else if (directive == "access_log") {
      srv.log["access_log"] = helper.get_access_log();
    } else if (directive == "error_log") {
      srv.log["error_log"] = helper.get_error_log();
    } else if (directive == "autoindex") {
      srv.autoindex = helper.get_autoindex();
    } else if (directive == "cgi") {
      srv.cgi["." + tokens[1]] = helper.get_cgi();
    } else if (directive == "return") {
      srv.redirect = helper.get_redirect();
    } else if (directive == "location") {
      srv.location[tokens[1]] = _parse_location(is);
    } else if (directive[0] == '#') {
      continue;
    } else if (directive == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(directive);
    }
  }

  srv.fill();
  if (srv.is_invalid())
    throw LoadException("Invalid Server");

  return (srv);
}

void Config::_parse(std::istringstream* is) {
  std::string line, directive;
  std::vector<std::string> tokens;

  while (std::getline(*is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");
    directive = tokens[0];

    ConfigHelper helper(tokens);

    if (directive == "workers")
      backlog = helper.get_backlog();
    else if (directive == "server")
      _servers.push_back(_parse_server(is));
    else
      throw ConfigHelper::DirectiveUnknown(tokens[0]);
  }
}
