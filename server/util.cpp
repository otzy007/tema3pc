#include "util.h"
#include "boost/algorithm/string.hpp"

std::vector< std::string > split(std::string stringToSplit, std::string splitBy)
{
   std::vector< std::string > substr;
   boost::split(substr, stringToSplit, boost::is_any_of(splitBy));
    
    return substr;
}
