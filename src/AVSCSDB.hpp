#include <iostream>
#include <map>
#include <string>
#include <exception>
#include <boost::any.hpp>


#ifndef __cplusplus
#error "AVSCSDB is a C++ library"
#endif

#ifndef AVSCSDB_HPP
#define AVSCSDB_HPP


namespace MXPSQL::AVSCSDB{

    template<typename T>
    using Dictionary = std::map<std::string, T>;

    class AVSCSDBCollection{
	private:
	Dictionary<boost::any> kvp;

    };

    class AVSCSDB{
        private:
        Dictionary<AVSCSDBCollection> kcoll;

	public bool dump(){
		
		try{
			return true;	
		}
		catch(std::exception){
			return false;
		}
	}
	
    };
}

#endif