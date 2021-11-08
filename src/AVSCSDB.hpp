#include <iostream>
#include <map>
#include <string>


#ifndef __cplusplus
#error "AVSCSDB is a C++ library"
#endif

#ifndef AVSCSDB_HPP
#define AVSCSDB_HPP


namespace MXPSQL::AVSCSDB{

    template<typename T>
    using Dictionary = std::map<std::string, T>;

    class AVSCSDBCollection{


    };

    class AVSCSDB{
        private:
        Dictionary<AVSCSDBCollection> kcoll;
    };
}

#endif