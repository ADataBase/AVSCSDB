#include "AVSCSDB.hpp"
#include <iostream>
#include <string>
#include <boost/any.hpp>

// sample program for the header

using namespace std;
using namespace MXPSQL:: AVSCSDB;

AVSCSDB db = load("Test.avscsdb");

int main(){
    db.set("coll1");
    AVSCSDBCollection coll1 = db.get("coll1");
    coll1.set("a", 1);
    coll1.set("a2", string("1"));

    int a = boost::any_cast<int>(coll1.get("a"));
    string a2 = boost::any_cast<string>(coll1.get("a2"));



    cout << a << ": a" << endl;
    cout << a2 << ": a2" << endl;
    cout << db.dump() << endl;






    return 0;
}