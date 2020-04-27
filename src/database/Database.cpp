#include <iostream>
using namespace std;

#include "Database.hpp"

namespace LogSupervisor{

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

vector<shared_ptr<const Authentication>> Database::all() const{
    vector<shared_ptr<const Authentication>> v;
    shared_ptr<SQLite::Statement> ps = db.prepare("SELECT * from auths;");
    while(ps->step() == SQLITE_ROW){
        User user =      (const char*) ps->textValue(0);
        Address origin = (const char*) ps->textValue(1);
        Timestamp ts =   (const char*) ps->textValue(2);
        v.push_back( make_shared<Authentication>(user, origin, ts) );
    }
    return v;
};



} // namespace