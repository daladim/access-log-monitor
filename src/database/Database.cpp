#include <iostream>
using namespace std;

#include "Database.hpp"

namespace LogSupervisor::Database{

Database::Database() :
    db(":memory:")
{
    // SQLite has no DATETIME format. Let's store everything as string
    db.exec("CREATE TABLE 'auths'('id' INTEGER PRIMARY KEY AUTOINCREMENT,                          "
            "                     'user' TEXT, 'origin' TEXT, 'timepoint' TEXT, 'success' INTEGER, "
            "                     'validity' TEXT, 'description' TEXT );");
}

void Database::insert(const Authentication& auth){
    shared_ptr<SQLite::Statement> ps = db.prepare("INSERT INTO auths (user, origin, timepoint, success, validity, description) " \
                                                  "VALUES (?, ?, ?, ?, ?, ?); ");
    ps->bindText(1, auth.user);
    ps->bindText(2, *(auth.origin.to_string()));
    ps->bindText(3, auth.timestamp.to_string());
    ps->bindInt(4, auth.success);
    ps->bindText(5, auth.validity.to_string());
    ps->bindText(6, auth.description);
    int rc = ps->step();
}


Request Database::all() const{
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths;");
    return Request(statement);
}

Request Database::successes() const{
    return Request(db.prepare("SELECT * from auths WHERE success=1 ORDER BY user, validity;"));
}

Request Database::failures() const{
    return Request(db.prepare("SELECT * from auths WHERE success=0 ORDER BY user, validity;"));
}

void Database::updateValidity(int id, Authentication::Validity v){
    shared_ptr<SQLite::Statement> statement = db.prepare("UPDATE 'auths' SET validity = ? WHERE id = ?");
    statement->bindText(1, v.to_string());
    statement->bindInt(2, id);
    statement->step();
}

std::shared_ptr<Authentication> Database::fetch(int id){
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths WHERE id = ?;");
    statement->bindInt(1, id);
    return Request(statement).firstRow();
}


} // namespace