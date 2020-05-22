#ifndef _HTML_SERIALIZER_HPP_
#define _HTML_SERIALIZER_HPP_

#include <iostream>

#include "SerializerInterface.hpp"
#include "../database/Database.hpp"
#include "../database/DatabaseObjects.hpp"

namespace LogSupervisor::Serializer{
using Database = LogSupervisor::Database::Database;

//! This classes serializes as formatted HTML
class HTML : public Interface{
public:
    HTML(const Database& db);
    ostream& serialize(ostream& lhs) override;
    virtual ~HTML() {}

private:
    //! The internal state of the authRow() function
    class State{
    public:
        User curUser;
        unsigned int iRow = 0;
        unsigned int iUser = 0;
    };

    const Database& db;

    // various HTML styles
    static const string sUserBlock;
    static const string sUserName;
    static const string sTable;
    static const string sCounter;
    static const string sFailed;
    static const string sRowStyleOK[];
    static const string sRowStyleWarning[];
    static const string sRowStyleCritical[];

    // Some constants
    static const unsigned int limitUsers = 100;
    static const unsigned int limitRowsPerUser = 15;

    //! Call this to print a row. It will automatically call ::userHeader and ::userFooter if needed.
    ostream& authRow(ostream& lhs, const shared_ptr<Authentication> auth, State& state);

    ostream& userHeader(ostream& lhs, const User& user, bool login_succeeded);  //!< Usually called by ::authRow
    ostream& userFooter(ostream& lhs);                                          //!< Usually called by ::authRow
};

} // namespace

#endif // _HTML_SERIALIZER_HPP_