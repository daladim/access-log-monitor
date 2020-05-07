#include <iostream>
#include <iomanip>
using namespace std;

#include "TextSerializer.hpp"

namespace LogSupervisor::Serializer{

Text::Text(const LogSupervisor::Database::Database& db) :
    db(db)
{}

#define COL 15
// I'm too lazy to define a class Word that has an operator<< method, so:
#define printWord(w) left << setw(COL) << string(w).substr(0,COL)
#define printBool(w) left << setw(COL) << boolalpha << w
#define printNative(w) left << setw(COL) << w

ostream& Text::serialize(ostream& lhs){

    lhs << "-----successful logins-------" << endl;
    lhs << printWord("User") << printWord("Origin") << printWord("Succcess?") << printWord("Status") << endl;
    for(const shared_ptr<Authentication> auth : db.successes()){
        lhs << printWord(auth->user) << printWord(*(auth->origin.to_string())) << printBool(auth->success) << printNative(auth->validity) << endl;
    }

    lhs << endl;
    lhs << "-----failed logins-------" << endl;
    lhs << printWord("User") << printWord("Count") << printWord("Succcess?") << printWord("Status") << endl;
    for(const shared_ptr<Authentication> auth : db.failures()){
        lhs << printWord(auth->user) << printWord(*(auth->origin.to_string())) << printBool(auth->success) << printNative(auth->validity) << endl;
    }

    return lhs;
}



} // namespace