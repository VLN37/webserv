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
  std::string str = _sanitize(_open(file));
  _parse(str);
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
  std::string line;
  std::vector<std::string> tokens;

  ServerLocation location;

  while (std::getline(*is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");

    if (tokens[0] == "root") {
      location.root = String::trim(std::string(tokens[1]), "/");
    } else if (tokens[0] == "index") {
      for (size_t i = 1; i < tokens.size(); i++) {
        location.index.push_back(tokens[i]);
      }
    } else if (tokens[0] == "limit_except") {
      if (location.limit_except[0] == DFL_LIM_EXCEPT) {
        location.limit_except.pop_back();
      }
      for (size_t i = 1; i < tokens.size(); i++) {
        std::transform(tokens[i].begin(),
                       tokens[i].end(),
                       tokens[i].begin(),
                       ::toupper);
        location.limit_except.push_back(tokens[i]);
      }
    } else if (tokens[0] == "client_max_body_size") {
      location.client_max_body_size = String::to_int(tokens[1]);
    } else if (tokens[0] == "autoindex") {
      location.autoindex = (tokens[1] == "on") ? true : false;
    } else if (tokens[0] == "cgi") {
      location.cgi["." + tokens[1]] = tokens[2];
    } else if (tokens[0] == "return") {
      if (location.redirect.first == 0 &&
          location.redirect.second.empty()) {
        location.redirect.first = String::to_int(tokens[1]);
        location.redirect.second = tokens[2];
      }
    } else if (tokens[0][0] == '#') {
      continue;
    } else if (tokens[0] == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(tokens[0]);
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

    if (directive == "listen") {
      srv.ip = ConfigHelper::get_listen(tokens).first;
      srv.port = ConfigHelper::get_listen(tokens).second;
    } else if (directive == "server_name") {
      srv.server_name = ConfigHelper::get_server_name(tokens);
    } else if (directive == "root") {
      srv.root = ConfigHelper::get_root(tokens);
    } else if (directive == "index") {
      srv.index = ConfigHelper::get_index(tokens);
    } else if (directive == "error_page") {
      int code = String::to_int(tokens[1]);
      srv.error_page[code] = ConfigHelper::get_error_page(tokens);
    } else if (tokens[0] == "timeout") {
      srv.timeout = ConfigHelper::get_timeout(tokens);
    } else if (tokens[0] == "client_max_body_size") {
      srv.client_max_body_size = String::to_int(tokens[1]);
    } else if (tokens[0] == "access_log") {
      srv.log["access_log"] = tokens[1];
    } else if (tokens[0] == "error_log") {
      srv.log["error_log"] = tokens[1];
    } else if (tokens[0] == "autoindex") {
      srv.autoindex = (tokens[1] == "on") ? true : false;
    } else if (tokens[0] == "cgi") {
      srv.cgi["." + tokens[1]] = tokens[2];
    } else if (tokens[0] == "return") {
      srv.redirect.first = String::to_int(tokens[1]);
      srv.redirect.second = tokens[2];
    } else if (tokens[0] == "location") {
      srv.location[tokens[1]] = _parse_location(is);
    } else if (tokens[0][0] == '#') {
      continue;
    } else if (tokens[0] == "}") {
      break;
    } else {
      throw ConfigHelper::DirectiveUnknown(tokens[0]);
    }
  }

  return (srv);
}

void Config::_parse(const std::string& file_content) {
  std::istringstream is(file_content);
  std::string line, directive;
  std::vector<std::string> tokens;

  while (std::getline(is, line)) {
    line = String::trim(line, "; ");
    tokens = String::split(line, " ");
    directive = tokens[0];

    if (directive == "workers")
      backlog = ConfigHelper::get_backlog(tokens);
    else if (directive == "server")
      _servers.push_back(_parse_server(&is));
    else
      throw ConfigHelper::DirectiveUnknown(tokens[0]);
  }
}
