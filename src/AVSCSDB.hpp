#include <iostream>
#include <map>
#include <stdlib.h>
#include "sqlite3.h"
#include <string>
#include <exception>


#ifndef __cplusplus
#error "AVSCSDB is a C++ library"
#endif

#ifndef AVSCSDB_HPP
#define AVSCSDB_HPP

// this is a databae library, who's backend is based of sqlite
namespace MXPSQL::AVSCSDB{

    template<typename T>
    using Dictionary = std::map<std::string, T>;

	// this class should return true on some function and the rc should be 0, if the bool is false, check the rc, it can give you info
    class AVSCSDB{
        private:
		// database
		int rc = 0;

		// status
		bool closed = false;

		public:
		sqlite3 *db;
    	sqlite3_stmt *res;
		// constructor
		AVSCSDB(){
			rc = sqlite3_open(":memory:", &db);
		}

		AVSCSDB(std::string path){
			rc = sqlite3_open(path.c_str(), &db);
		}

		// db ctx functions
		int getRc(){
			return rc;
		}

	 	bool dump(){
		
			try{
				if(closed){
					throw "Database is closed";
				}

				return true;	
			}
			catch(std::exception){
				return false;
			}

		}

		bool close(){
			closed = true;
			sqlite3_close(db);
			return true;
		}

		// sqlite handle and sqlite exec
		bool exec(){
			if(rc != SQLITE_OK){
				return false;
			}
			
			if(rc != SQLITE_OK){
				return false;
			}

			return true;
		}

		sqlite3* getSqliteDBHandleReference(){
			return db;
		}

		sqlite3_stmt* getSqliteStatementHandleReference(){
			return res;
		}
	
    };


	AVSCSDB load(std::string path){
		AVSCSDB ddb(path);
		return ddb;
	}
}

#endif