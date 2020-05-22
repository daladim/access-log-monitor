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
            "                     'count' INTEGER, 'validity' TEXT, 'description' TEXT );");
}

void Database::insert(const Authentication& auth){
    // Does this record exist already?
    // Note that we do not check Timestamps, since their support is planned for a future (if any) version
    shared_ptr<SQLite::Statement> check = db.prepare("SELECT count,id from auths WHERE user=? AND origin=? AND success=?");
    check->bindText(1, auth.user);
    check->bindText(2, *(auth.origin.to_string()));
    check->bindInt(3, auth.success);

    for(const SQLite::Statement& record : *check){
        // There is such a record already. We just want to update it
        shared_ptr<SQLite::Statement> update = db.prepare("UPDATE auths SET count=? WHERE id=?");
        update->bindInt(1, record.intValue(0) + 1);
        update->bindInt(2, record.intValue(1));
        update->step();
        return;
    }


    // Insert a new record if needed
    shared_ptr<SQLite::Statement> ps = db.prepare("INSERT INTO auths (user, origin, timepoint, success, count, validity, description) " \
                                                  "VALUES (?, ?, ?, ?, ?, ?, ?); ");
    ps->bindText(1, auth.user);
    ps->bindText(2, *(auth.origin.to_string()));
    ps->bindText(3, auth.timestamp.to_string());
    ps->bindInt(4, auth.success);
    ps->bindInt(5, 1);
    ps->bindText(6, auth.validity.to_string());
    ps->bindText(7, auth.description);
    ps->step();
}


Request Database::all() const{
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths;");
    return Request(statement);
}

Request Database::successes() const{
    return Request(db.prepare("SELECT * from auths WHERE success=1 ORDER BY user ASC, validity ASC, count DESC;"));
}

Request Database::failures() const{
    return Request(db.prepare("SELECT * from auths WHERE success=0 ORDER BY user ASC, validity ASC, count DESC;"));
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