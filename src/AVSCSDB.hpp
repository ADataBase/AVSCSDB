#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <exception>
#include <cstddef>
#include <filesystem>

// openssl
#include <openssl/pem.h>
#include <openssl/x509.h>

// #include "../extern/cppcodec/cppcodec/base64_rfc4648.hpp" // not used


// check if this is c++ or not
#ifndef __cplusplus
#error "AVSCSDB is a C++ library"
#endif

#ifndef AVSCSDB_CPP_VERSION
	#define AVSCSDB_CPP_VERSION __cplusplus
#endif

#if AVSCSDB_CPP_VERSION < 201703L
#error "You may used this as a C++ library, but your standard is too old! AVSCSDB needs C++ 17 to function!"
#endif

#ifndef AVSCSDB_HPP
#define AVSCSDB_HPP

// codes
#define AVSCSDB_OK 0
#define AVSCSDB_DUMP_ERR 1
#define AVSCSDB_KEY_NOEXST 2
#define AVSCSDB_COLLEC_NOEXST 3
#define AVSCSDB_SERIALPARSE_FAIL 5
#define AVSCSDB_FILE_NOEXST 6
#define AVSCSDB_LOAD_ERR 77778
#define AVSCSDB_GEN_ERR -9
#define AVSCSDB_INTERN_ERR -21 // no extesions or database software with AVSCSDB Backend should use this

// internal things, not meant to be used publicly
namespace MXPSQL::AVSCSDB::Internal{
	inline bool file_exists(const std::string& name) {
	    std::ifstream f(name.c_str());
	    return f.good();
	}
}

// this is a database library
// logic:
namespace MXPSQL::AVSCSDB{

	// include this internal namespace
	using namespace MXPSQL::AVSCSDB::Internal;

	// include more namespace
	// using fs = std::experimental::filesystem;

    template<typename T>
    using Dictionary = std::map<std::string, T>;

	// DB Map Parser interface and class
	// Custom DB Map Parsers should implement this, the database will use this
	// You can implement this to have custom formats and implementation, the main one is JSON
	template<typename T>
	class IDBParser {
		public:
		// abstract methods for you to implement
		virtual std::string mapParse(Dictionary<Dictionary<T>>) = 0;
		virtual Dictionary<Dictionary<T>> parse2map(std::string file) = 0;
	};

	// main implementation
	template<typename T>	
	class DBRapidParser : public IDBParser<T>{
		public:
		std::string mapParse(Dictionary<Dictionary<T>> m){
		}

		Dictionary<Dictionary<T>> parse2map(std::string file){
			Dictionary<Dictionary<T>> d;
			return d;
		}
	};

#ifndef MXPSQL_AVSCSDB_CUSTOM_ILPL // check if custom implementation is used or not

	// My Implementation for the database
	// Memory storage, dumped to file manually
	// probably not thread safe and not atomic, but you can make this atomic
	// You need to change the Parser implementation to use other type and use other file format
	template<typename T = std::string, template<typename> typename P = DBRapidParser> // default type is string, you chnge the parser to use toml or something else
	class AVSCSDB{
		private:
		Dictionary<Dictionary<T>> mapi;
		int rc = AVSCSDB_OK; // should be checked if the return code of some function is false
		std::string path;
		P<T> parser;

		public:
		// constructor
		AVSCSDB(){

		}

		AVSCSDB(std::string pathloc){
			path = pathloc;
			load(path);
		}

		// dump by default
		~AVSCSDB(){
			dump(path);
		}

		// functions
		void throwOnError(){
			if(rc != 0){
				switch(rc){
					case AVSCSDB_DUMP_ERR:
						throw new std::ofstream::failure("Failed to dump to file!");
						break;
					case AVSCSDB_SERIALPARSE_FAIL:
						throw new std::runtime_error("Parser failed");
						break;
					case AVSCSDB_LOAD_ERR:
						throw new std::ifstream::failure("Failed to load database!");
						break;
					case AVSCSDB_FILE_NOEXST:
						throw new std::ifstream::failure("Database does not exists!");
						break;

					case AVSCSDB_KEY_NOEXST:
						throw new std::out_of_range("Key does not exists!");
						break;
					case AVSCSDB_COLLEC_NOEXST:
						throw new std::out_of_range("Collection does not exists!");
						break;
					
					case AVSCSDB_GEN_ERR:
						throw new std::runtime_error("Unknown runtime error occured!");
						break;
				}
			}
		}

		int getRC(){
			return rc;
		}

		void clearRC(){
			rc = AVSCSDB_OK;
		}
		
		// load to the memory
		void strload(std::string db){
		}

		void load(std::string pathloc){
			std::ifstream fs;

			if(!file_exists(pathloc)){
				rc = AVSCSDB_FILE_NOEXST;
				return;
			}

			fs.open(pathloc);
			if(!fs.good()){
				fs.close();
				rc = AVSCSDB_LOAD_ERR;
				return;
			}

			path = pathloc;

			std::string ret;
			fs >> ret;

			strload(ret);

			fs.close();
		}

		void load(){
			load(path);
			throwOnError();
		}

		std::string strdump(){
			return parser.mapParse(mapi);
		}

		bool dump(){
			std::string ret = "";
			std::ofstream ofs;
			bool error = false;

			try{
				ret = strdump();
				throwOnError();
			}
			catch(...){
				rc = AVSCSDB_SERIALPARSE_FAIL;
				return false;
			}

			try{
				ofs.open(path, std::ios::trunc);
				ofs << ret;
				error = false;
			}
			catch(std::exception){
				error = true;
			}

			ofs.close();

			if(error){
				rc = AVSCSDB_DUMP_ERR;
				return false;
			}

			rc = AVSCSDB_OK;
			return true;
		}

		bool setPath(std::string pathloc){
			path = pathloc;
			rc = AVSCSDB_OK;
			return true;
		}

		std::string getPath(){
			rc = AVSCSDB_OK;
			return path;
		}

		bool CollectionExists(std::string collection){
			rc = AVSCSDB_OK;
			return (mapi.count(collection) > 0);
		}

		bool setCollection(std::string collection){
			Dictionary<T> d;
			mapi.insert({collection, d});
			rc = AVSCSDB_OK;
			return true;
		}

		bool removeCollection(std::string collection){
			if(CollectionExists(collection)){
				mapi.erase(collection);
				rc = AVSCSDB_OK;
				return true;
			}
			else{
				rc = AVSCSDB_COLLEC_NOEXST;
				return false;
			}
		}

		bool KeyExists(std::string collection, std::string key){
			rc = AVSCSDB_OK;
			return (mapi.at(collection).count(key) > 0);
		}

		bool setKey(std::string collection, std::string key, T value){
			if(!CollectionExists(collection)){
				rc = AVSCSDB_COLLEC_NOEXST;
				return false;
			}

			mapi.at(collection).insert({key, value});
			rc = AVSCSDB_OK;
			return true;
		}

		T getKey(std::string collection, std::string key){
			if(!CollectionExists(collection)){
				rc = AVSCSDB_KEY_NOEXST;
				return nullptr;
			}

			if(KeyExists(collection, key)){
				T var;
				var = mapi.at(collection).at(key);
				rc = AVSCSDB_OK;
				return var;
			}
			else{
				rc = AVSCSDB_KEY_NOEXST;
				return nullptr;
			}
		}

		bool removeKey(std::string collection, std::string key){
			if(!CollectionExists(collection)){
				rc = AVSCSDB_COLLEC_NOEXST;
				return false;
			}

			if(KeyExists(collection, key)){
				mapi.at(collection).erase(key);
				rc = AVSCSDB_OK;
				return true;
			}
			else{
				rc = AVSCSDB_KEY_NOEXST;
				return false;
			}
		}


		// overloads for operator
		friend std::ostream &operator<<( std::ostream &output, const AVSCSDB &a){

			return output;
		}
	};
#else
	// Implement your own custom implementation
	// You just need to write your own storage container, return code variable, parser and implement the function
	template<typename T, template<typename> typename P>
	class AVSCSDB{
		public:
			virtual AVSCSDB() = 0;
			virtual AVSCSDB(std::string pathloc) = 0;
			virtual ~AVSCSDB() = 0;
			virtual throwOnError() = 0;
			virtual int getRC() = 0;	
			virtual void clearRC()  = 0;
			virtual void strload(std::string db)  = 0;
			virtual void load(std::string pathloc) = 0;
			virtual void load() = 0;
			virtual std::string strdump() = 0;
			virtual bool dump() = 0;
			virtual bool setPath(std::string pathloc) = 0;
			virtual std::string getPath() = 0;
			virtual bool CollectionExists(std::string collection) = 0;
			virtual bool setCollection(std::string collection) = 0;
			virtual bool removeCollection(std::string collection) = 0;
			virtual bool KeyExists(std::string collection, std::string key) = 0;
			virtual bool setKey(std::string collection, std::string key, T value) = 0;
			virtual T getKey(std::string collection, std::string key) = 0;
			virtual bool removeKey(std::string collection, std::string key) = 0;
			virtual friend std::ostream &operator<<( std::ostream &output, const AVSCSDB &a) = 0;
	};

#endif

}
// file format:
// Uses ISO image, el torito, stored inside of it, library will not be developed now, should be based of picklingtools
// Stored in pickle file, stored as key value pair inside the ISO image, not developed now too
// OpenSSL or GnuTLS used foe generating the random filename and making the key file
// Uses XML for indexing the key value, location of the key, expat is used for parsing the XML
// The ISO image containing the files are deflated with zlib, the files are stored in the ISO image
// The ISO image is a file, the database is a folder full of pickle files inside the ISO image, deflated basically
/*
Sturcture:
	ISO image:
		DB: // the database is stored inside the folder
			fjashfajh.pkl // pickle file used to store data
			fafakfj.pkl // one pickle file is one collection
			fdsjjfkladfl.pkl // each pickle file/collection is a key value store
		Key.pem // this is the key
		index.xml // this is the index file
			
*/
/*
Example XML file:
<AVSCSDB>
	<Key>Key.pem</Key>
	<DB>
		<Path id="Coll1">DB/fjashfajh.pkl</Path>
		<Path id="Coll2">DB/fafakfj.pkl</Path>
		<Path id="Coll3">DB/fdsjjfkladfl.pkl</Path>
	</DB>
</AVSCSDB>
*/

#endif