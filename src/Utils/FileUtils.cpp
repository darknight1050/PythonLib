#include "Utils/FileUtils.hpp"

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "CustomLogger.hpp"
#include "ModInfo.hpp"
#include "zip.h"

namespace FileUtils {

    const std::string& getPythonPath() {
        static std::string pythonHome = string_format("%spython", Modloader::getDestinationPath().c_str());
        return pythonHome;
    }

    const std::string& getScriptsPath() {
        static std::string scriptsPath = string_format("%sScripts", getDataDir(modInfo).c_str());
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