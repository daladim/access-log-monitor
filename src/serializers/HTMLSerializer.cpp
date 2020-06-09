#include <iostream>
#include <iomanip>
using namespace std;

#include "HTMLSerializer.hpp"

namespace LogSupervisor::Serializer{

const string HTML::sUserBlock = "border-spacing: 0px;               \
                                 border-collapse: separate;         \
                                 box-shadow: 5px 5px 8px #888888;   \
                                 width: 100%;";
const string HTML::sUserName = "background-color:#00a0ff;           \
                                text-align:center;                  \
                                font-weight: bold;                  \
                                width:100%;                         \
                                margin:10px 0px 0px 0px;";
const string HTML::sTable = "width:100%; border-spacing:0px;";
const string HTML::sCounter = "color:#909090;";
const string HTML::sFailed = "opacity: 33%;";

const string HTML::sRowStyleOK[] =      { "color:#303030; background-color:#e7ffea;", "color:#303030; background-color:#c6efc6;" };
const string HTML::sRowStyleWarning[] = { "color:#303030; background-color:#fdd079;", "color:#303030; background-color:#ffe7b9;" };
const string HTML::sRowStyleCritical[]= { "color:#303030; background-color:#ffcece;", "color:#303030; background-color:#fdebeb;" };



HTML::HTML(const LogSupervisor::Database::Database& db) :
    db(db)
{}

ostream& HTML::userHeader(ostream& lhs, const User& user, bool login_succeeded){
    string optionalStyle;
    if(login_succeeded == false) optionalStyle = sFailed;
    lhs << "<div style='" << sUserBlock << optionalStyle << "'>\n";
    lhs << "<header style='" << sUserName << "'>" << user << "</header>\n";

    lhs << "<table style='" << sTable << "'>";
    return lhs;
}
// I could have made fancier classes, that automatically add a </div> when they are discarded, but heck.
ostream& HTML::userFooter(ostream& lhs){
    return (lhs << "</table></div>");
}

ostream& HTML::authRow(ostream& lhs, const shared_ptr<Authentication> auth, State& state){
    if(state.iRow == 0 || state.curUser != auth->user){
        if(state.iRow > 0){
            userFooter(lhs);
        }

        state.curUser = auth->user;
        state.iRow = 0;
        state.iUser++;
        if(state.iUser < limitUsers){
            userHeader(lhs, auth->user, auth->success);
        }else{
            if(state.iUser == limitUsers){
                lhs << "... (too many users to show)\n";
                return lhs;
            }
        }
    }

    string rowStyle;
    if(auth->validity == Authentication::Validity::OK()){
        rowStyle = sRowStyleOK[state.iRow % 2];
    }else if(auth->validity == Authentication::Validity::Warning()){
        rowStyle = sRowStyleWarning[state.iRow % 2];
    }else{
        rowStyle = sRowStyleCritical[state.iRow % 2];
    }

    lhs << "<tr style='" << rowStyle << "'>";
    if(state.iRow == limitRowsPerUser){
        lhs << "<td>... (too many results)</td>\n";
    }else if(state.iRow < limitRowsPerUser){
        lhs << "<td><span style='" << sCounter << "'>" << auth->count << "x </span>";
        lhs << *(auth->origin.to_string()) << "</td>";
        lhs << "<td align='right'>" << auth->description << "</td></tr>\n";
    }
    ++(state.iRow);
    return lhs;
}


ostream& HTML::serialize(ostream& lhs){
    // Having the CSS apart is not widely supported in webmails.
    // It is safer to rather have the style hard-coded in every tag

    lhs << "<br><br><b>succeded</b> logins" << endl;
    State state;
    for(const shared_ptr<Authentication> auth : db.successes()){
        authRow(lhs, auth, state);
    }
    userFooter(lhs);


    lhs << "<br><br>The following are <b>failed</b> logins" << endl;
    for(const shared_ptr<Authentication> auth : db.failures()){
        authRow(lhs, auth, state);
    }
    userFooter(lhs);
    lhs << "\n";

    return lhs;
}



} // namespace