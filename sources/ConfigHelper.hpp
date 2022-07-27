//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef CONFIG_HELPER_HPP_
#define CONFIG_HELPER_HPP_

#define PARSE_ERROR "WebServ Failed to parse config: "

#include <string>
#include <vector>

#include "LoadException.hpp"
#include "String.hpp"

class ConfigHelper {
 public:
  static int get_backlog(const std::vector<std::string>& tokens);

  class InvalidNumberArgs : public LoadException {
   public:
    explicit InvalidNumberArgs(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveDuplicate : public LoadException {
   public:
    explicit DirectiveDuplicate(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveInvValue : public LoadException {
   public:
    explicit DirectiveInvValue(const std::string& str);
    const char* what(void) const throw();
  };

  class DirectiveUnknown : public LoadException {
   public:
    explicit DirectiveUnknown(const std::string& str);
    const char* what(void) const throw();
  };
};

#endif  // CONFIG_HELPER_HPP_
