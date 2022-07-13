//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

#include "Pollfd.hpp"

_pollfd::_pollfd(void): fd(0), events(0), revents(0) { }
_pollfd::_pollfd(int _fd): fd(_fd), events(0), revents(0) { }
_pollfd::_pollfd(int _fd, short int _ev): fd(_fd), events(_ev), revents(0) { }
