#include <iostream>
#include <iomanip>
using namespace std;

#include "HTMLSerializer.hpp"

namespace LogSupervisor::Serializer{

HTML::HTML(const LogSupervisor::Database::Database& db) :
    db(db)
{}

ostream& HTML::serialize(ostream& lhs){
    // Having the CSS apart is not widely supported in e-mails.
    // It is safer to rather have the style hard-coded in every tag

    lhs << "-----successful logins-------" << endl;

    lhs << "<table border><tr><td>User</td><td>Origin</td><td>Status</td></tr>" << endl;
    for(const shared_ptr<Authentication> auth : db.successes()){
        lhs << "<tr><td>" << auth->user << "</td><td>" << *(auth->origin.to_string()) << "</td><td>" << auth->validity << "</td></tr>" << endl;
    }
    lhs << "</table>" << endl;


    lhs << "-----Failed logins-------" << endl;

    lhs << "<table border><tr><td>User</td><td>Origin</td><td>Status</td></tr>" << endl;
    for(const shared_ptr<Authentication> auth : db.failures()){
        lhs << "<tr><td>" << auth->user << "</td><td>" << *(auth->origin.to_string()) << "</td><td>" << auth->validity << "</td></tr>" << endl;
    }
    lhs << "</table>" << endl;

    // // TODO: only show a count


    return lhs;
}



} // namespace