#include <iostream>
#include <iomanip>
using namespace std;

#include "HTMLSerializer.hpp"

namespace LogSupervisor::Serializer{

const string HTML::sUserBlock = "border-spacing: 0px;        \
                                 border-collapse: separate;  \
                                 box-shadow: 5px 5px 8px #888888;   \
                                 width: 100%;";
const string HTML::sUserName = "background-color:#00a0ff;               \
                                text-align:center;                      \
                                width:100%;                            \
                                margin:10px 0px 0px 0px;";
const string HTML::sTable = "width:100%; border-spacing:0px;";
const string HTML::sCounter = "color:#b0b0b0;";

const string HTML::sRowStyleOK[] =      { "color:#303030; background-color:#e7ffea;", "color:#303030; background-color:#c6efc6;" };
const string HTML::sRowStyleWarning[] = { "color:#303030; background-color:#fdd079;", "color:#303030; background-color:#ffe7b9;" };
const string HTML::sRowStyleCritical[]= { "color:#303030; background-color:#ffcece;", "color:#303030; background-color:#fdebeb;" };



HTML::HTML(const LogSupervisor::Database::Database& db) :
    db(db)
{}

ostream& HTML::userHeader(ostream& lhs, const User& user, bool login_succeeded){
    string opacity;
    if(login_succeeded == false) opacity = "opacity: 33%;";
    lhs << "<div style='" << sUserBlock << opacity << "'>\n";
    lhs << "<header style='" << sUserName << "'>" << user << "</header>\n";

    lhs << "<table style='" << sTable << "'>";
    return lhs;
}
// I could have made fancier classes, that automatically add a </div> when the are discarded, but heck.
ostream& HTML::userFooter(ostream& lhs){
    return (lhs << "</table></div>");
}

ostream& HTML::authRow(ostream& lhs, const shared_ptr<Authentication> auth, unsigned int* iRow, User* curUser){
    if(iRow == 0 || *curUser != auth->user){
        if(iRow>0){
            userFooter(lhs);
        }
        userHeader(lhs, auth->user, auth->success);
        *curUser = auth->user;
        *iRow = 0;
    }

    string rowStyle;
    if(auth->validity == Authentication::Validity::OK()){
        rowStyle = sRowStyleOK[*iRow % 2];
    }else if(auth->validity == Authentication::Validity::Warning()){
        rowStyle = sRowStyleWarning[*iRow % 2];
    }else{
        rowStyle = sRowStyleCritical[*iRow % 2];
    }

    lhs << "<tr style='" << rowStyle << "'>";
    lhs << "<td>" << *(auth->origin.to_string()) << "</td>";
    lhs << "<td style='" << sCounter << "'>x" << auth->count << "</td>";
    lhs << "<td align='right'>" << auth->description << "</td></tr>\n";
    ++(*iRow);
    return lhs;
}


ostream& HTML::serialize(ostream& lhs){
    // Having the CSS apart is not widely supported in webmails.
    // It is safer to rather have the style hard-coded in every tag

    lhs << "<br><br><b>succeded</b> logins" << endl;
    User curUser;
    unsigned int iRow = 0;
    for(const shared_ptr<Authentication> auth : db.successes()){
        authRow(lhs, auth, &iRow, &curUser);
    }
    userFooter(lhs);


    lhs << "<br><br>The following are <b>failed</b> logins" << endl;
    for(const shared_ptr<Authentication> auth : db.failures()){
        authRow(lhs, auth, &iRow, &curUser);
    }
    userFooter(lhs);
    lhs << "\n";

    return lhs;
}



} // namespace