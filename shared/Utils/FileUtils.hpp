#pragma once
#include <string>

namespace FileUtils {

    const std::string& getPythonPath();
    const std::string& getScriptsPath();
    int ExtractZip(std::string_view data, std::string_view path);
    
}