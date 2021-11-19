#include "AVSCSDB.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include <boost/any.hpp>

// sample program for the header

using namespace std;
using namespace MXPSQL:: AVSCSDB;

AVSCSDB db = load("Test.avscsdb");

int main(){
    cout << db.getSqliteDBHandleReference() << endl;
    
    assert(db.db == db.getSqliteDBHandleReference());



    return 0;
}