#ifndef _SQLITE_DB_HPP_
#define _SQLITE_DB_HPP_

#include <sqlite3.h>
#include <memory>

#include "SQLite.hpp"
#include "SQLiteStatement.hpp"

namespace SQLite{

//! A wrapper around sqlite3* objects
class SQLiteDB{
public:
    SQLiteDB(const std::string& location);
    SQLiteDB(const SQLiteDB&) = delete;
    SQLiteDB& operator=(const SQLiteDB& rhs) = delete;
    ~SQLiteDB();

    //! This is used for statements where the fetched data must be manipulated
    //! The returned object is no longer valid if the parent database object is destroyed.
    //! For security reasons, never concatenate user-controlled data into statement. You should rather use preparedStatements and bind arguments
    std::shared_ptr<SQLiteStatement> prepare(const std::string& statement) const;

    //! This can be used for statements that do not fetch anything
    void exec(const std::string& statement);

private:
    sqlite3 *db;
};

} // namespace

#endif // _SQLITE_DB_HPP_