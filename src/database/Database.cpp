#include <iostream>
using namespace std;

#include "Database.hpp"

namespace LogSupervisor::Database{

Database::Database() :
    db(":memory:")
{
    // SQLite has no DATETIME format. Let's store everything as string
    db.exec("CREATE TABLE \"auths\"(\"user\" TEXT, \"origin\" TEXT, \"timepoint\" TEXT);");
}

void Database::insert(const Authentication& auth){
    shared_ptr<SQLite::Statement> ps = db.prepare("INSERT INTO auths (user, origin, timepoint)" \
                                                "VALUES (?, ?, ?); ");
    ps->bindText(1, auth.user);
    ps->bindText(2, *(auth.origin.to_string()));
    ps->bindText(3, auth.timestamp.to_string());
    int rc = ps->step();
}


Request Database::all(){
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths;");
    return Request(statement);
}

} // namespace