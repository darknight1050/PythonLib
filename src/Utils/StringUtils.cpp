#include "Utils/StringUtils.hpp"

namespace StringUtils {

    std::vector<std::string> Split(const std::string& s, std::string_view delimiter) {
        std::vector<std::string> result;
        size_t last = 0; 
        size_t next = 0; 
        while ((next = s.find(delimiter, last)) != std::string::npos) { 
            result.push_back(s.substr(last, next-last));
            last = next + 1; 
        } 
        result.push_back(s.substr(last));
        return result;
    }

}