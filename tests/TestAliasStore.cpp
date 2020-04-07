#include "catch.hpp"

#include <iostream>
#include <iomanip>
#include "../src/utils/AliasStore.hpp"
using namespace std;
using namespace LogSupervisor;

template<typename T>
static bool compare(const vector<T>& left, const vector<T>& right){
    if(left == right){
        return true;
    }else{
        const int col_width = 10;
        cerr << std::left << std::setw(col_width) << "left" << "right" << endl;
        size_t m = min(left.size(), right.size());
        for(int i=0; i<m; ++i){
            cerr << std::left << std::setw(col_width) << left[i] << right[i] << endl;
        }
        return false;
    }
}

TEST_CASE( "AliasStore<string>" ){
    AliasStore<string> s;
    s.insert("lower-case letters", {"a", "b", "c", "d"});
    s.insert("upper-case letters", {"A", "B", "C", "D"});
    s.insert("nu",                 {"1", "2", "3", "4"});
    s.insert("numbers",            {"nu"});
    s.insert("letters",            {"lower-case letters", "upper-case letters"});
    s.insert("same letters",       {"lower-case letters", "upper-case letters"});
    s.insert("more letters",       {"lower-case letters", "upper-case letters", "z"});
    s.insert("even more letters",  {"lower-case letters", "upper-case letters", "z", "Z"});
    s.insert("letters and numbers",{"letters", "numbers"});

    // Normal operation
    REQUIRE( compare<string>( s.parse("something")           , {"something"}                                                ));
    REQUIRE( compare<string>( s.parse("lower-case letters")  , {"a", "b", "c", "d"}                                         ));
    REQUIRE( compare<string>( s.parse("nu")                  , {"1", "2", "3", "4"}                                         ));
    REQUIRE( compare<string>( s.parse("numbers")             , {"1", "2", "3", "4"}                                         ));
    REQUIRE( compare<string>( s.parse("letters")             , {"a", "b", "c", "d", "A", "B", "C", "D"}                     ));
    REQUIRE( compare<string>( s.parse("same letters")        , {"a", "b", "c", "d", "A", "B", "C", "D"}                     ));
    REQUIRE( compare<string>( s.parse("more letters")        , {"a", "b", "c", "d", "A", "B", "C", "D", "z"}                ));
    REQUIRE( compare<string>( s.parse("even more letters")   , {"a", "b", "c", "d", "A", "B", "C", "D", "z", "Z"}           ));
    REQUIRE( compare<string>( s.parse("letters and numbers") , {"a", "b", "c", "d", "A", "B", "C", "D", "1", "2", "3", "4"} ));

    // Invalid operations
    REQUIRE_THROWS_AS( s.insert("numbers", {"This alias already exists"}) , std::range_error );

    // This is not exactly defined behaviour, but at least, this should not crash
    s.insert("double", {"numbers", "numbers", "numbers"});
}

class Integer{
public:
    Integer(const string& s){
        char* p;
        i = strtol(s.c_str(), &p, 10);
        if (*p) {
            throw std::runtime_error(string("Unable to construct from ") + s);
        }
    }
    Integer(const int value) :
        i(value)
    {}
    bool operator==(const Integer& rhs) const{
        return i == rhs.i;
    }
    friend ostream& operator<<(ostream& os, const Integer& rhs);
private:
    long i;
};
ostream& operator<<(ostream& os, const Integer& rhs){
    os << rhs.i;
    return os;
}

TEST_CASE( "AliasStore<Integer>" ){
    AliasStore<Integer> s;
    s.insert("positive integers",  { "1", "2", "3", "4"}                            );
    s.insert("larger integers",    { "10","20","30","40"}                           );
    s.insert("ni",                 {"-1","-2","-3","-4"}                            );
    s.insert("negative integers",  {"ni"}                                           );
    s.insert("pintegers",          {"positive integers", "larger integers"}         );
    s.insert("same pintegers",     {"positive integers", "larger integers"}         );
    s.insert("more pintegers",     {"positive integers", "larger integers", "100"}  );
    s.insert("even more pintegers",{"positive integers", "larger integers", "100", "200"});
    s.insert("all",                {"pintegers", "negative integers"}               );


    // Normal operation
    REQUIRE( compare<Integer>( s.parse("11")                 ,{11}                                  ));
    REQUIRE( compare<Integer>( s.parse("positive integers")  ,{ 1, 2, 3, 4}                         ));
    REQUIRE( compare<Integer>( s.parse("ni")                 ,{-1,-2,-3,-4}                         ));
    REQUIRE( compare<Integer>( s.parse("negative integers")  ,{-1,-2,-3,-4}                         ));
    REQUIRE( compare<Integer>( s.parse("pintegers")          ,{ 1, 2, 3, 4,10,20,30,40}             ));
    REQUIRE( compare<Integer>( s.parse("same pintegers")     ,{ 1, 2, 3, 4,10,20,30,40}             ));
    REQUIRE( compare<Integer>( s.parse("more pintegers")     ,{ 1, 2, 3, 4,10,20,30,40, 100}        ));
    REQUIRE( compare<Integer>( s.parse("even more pintegers"),{ 1, 2, 3, 4,10,20,30,40, 100, 200}   ));
    REQUIRE( compare<Integer>( s.parse("all")                ,{ 1, 2, 3, 4,10,20,30,40,-1,-2,-3,-4} ));

    // Invalid operations
    REQUIRE_THROWS_AS( s.insert("numbers", {"This alias already exists"}) , std::range_error );
    REQUIRE_THROWS_AS( s.insert("fails", {"this is neither an alias nor a valid constructor parameter"}) , std::range_error );
    REQUIRE_THROWS_AS( s.parse("this is neither an alias nor a valid constructor parameter") , std::range_error );

    // This is not exactly defined behaviour, but at least, this should not crash
    s.insert("double", {"negative integers", "negative integers", "negative integers"});
}
