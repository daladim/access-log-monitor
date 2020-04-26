#ifndef _SQLITE_HPP_
#define _SQLITE_HPP_

#include <iostream>

namespace SQLite{

class SQLError : public std::runtime_error{
public:
    SQLError(const std::string& what) : std::runtime_error(what) {}
};

} // namespace

#endif // _SQLITE_HPP_