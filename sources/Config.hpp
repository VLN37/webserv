//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Server.hpp"

class Server;

class Config {
 public:
  Config(void);
  Config(const Config& src);
  ~Config(void);

  Config& operator=(const Config& rhs);
  const Server& operator[](size_t n);

  size_t size(void);

  void parse(char* file);

  const std::string& get_error(void);

 private:
  std::string _sanitize(const std::string& file_content);
  std::string _sub_host(const std::string& file_content);
  std::vector<std::string> _sub_vhost(const std::string& file_content);
  void _parse_host(const std::string& host);
  void _parse_vhost(const std::vector<std::string>& vhost);

  void _replace_all(std::string* str,
                    const std::string& old_word,
                    const std::string& new_word);

  void _replace_unique(std::string* str, char pattern);

  std::string _trim(const std::string& str, const std::string& set);
  void _trim_line(std::string* str, const std::string& set);

  std::vector<std::string> _split(const std::string& str,
                                  const std::string& del);

  size_t _stoi(const std::string& str);

 public:
  int backlog;

 private:
  std::vector<Server> _servers;
  int _error;
  std::string _str_error;
};

#endif  // CONFIG_HPP_
