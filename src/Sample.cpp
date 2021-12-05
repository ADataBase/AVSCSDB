#include "AVSCSDB.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include <exception>

// sample program for the header

using namespace std;
using namespace MXPSQL::AVSCSDB;

AVSCSDB<> db;

int main(){
    db.setCollection("a");
    assert(db.CollectionExists("a"));
    db.setKey("a", "a.out", "COFF");
    assert(db.KeyExists("a", "a.out"));

    cout << db.getKey("a", "a.out");

    db.setCollection("b");
    assert(db.CollectionExists("b"));
    db.setKey("b", "bout", "a.off");
    assert(db.KeyExists("b", "bout"));

    db.removeKey("b", "bout");
    assert(!db.KeyExists("b", "bout"));
    db.removeCollection("b");
    assert(!db.CollectionExists("b"));


    return 0;
}