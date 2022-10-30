#include "Utils/FileUtils.hpp"

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "CustomLogger.hpp"
#include "ModInfo.hpp"

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

}