#pragma once
#include <string>
#include <vector>

namespace StringUtils {

    std::vector<std::string> Split(const std::string& s, std::string_view delimiter);

}