#include "Utils/FileUtils.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "CustomLogger.hpp"
#include "ModInfo.hpp"
#include "zip.h"

#include <filesystem>

namespace FileUtils {

    const std::string& getPythonPath() {
        static std::string pythonHome = std::filesystem::path{modloader::get_files_dir()} / "python";
        return pythonHome;
    }

    const std::string& getScriptsPath() {
        static std::string scriptsPath = std::filesystem::path{getDataDir(modInfo)} / "Scripts";
        if(!direxists(scriptsPath))
            mkpath(scriptsPath);
        return scriptsPath;
    }

    int ExtractZip(std::string_view data, std::string_view path) {
        int args = 2;
        int statusCode = zip_stream_extract(data.data(), data.length(), path.data(), 
            +[](const char* name, void* arg) -> int {
                return 0;
            }, &args);
        return statusCode;
    }

}