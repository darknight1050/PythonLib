#include "PythonInternal.hpp"
#include "Python.hpp"

#include "zip.h"
#include "assets.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "CustomLogger.hpp"

namespace Python {

    bool LoadPythonDirect() {
        auto pythonPath = FileUtils::getPythonPath();
        auto pythonHome = pythonPath + "/usr";
        if(!direxists(pythonHome)) {
            LOG_INFO("Creating PythonPath: %s", pythonPath.c_str());
            mkpath(pythonPath);
            int args = 2;
            std::string_view data = IncludedAssets::python_zip;
            int statusCode = zip_stream_extract(data.data(), data.length(), pythonPath.c_str(), 
                +[](const char* name, void* arg) -> int {
                    return 0;
                }, &args);
        }
        auto libdl = dlopen("libdl.so", RTLD_NOW | RTLD_GLOBAL);
        if(!libdl) {
            LOG_ERROR("Couldn't dlopen: libdl.so");
            return false;
        }
        LOAD_DLSYM(libdl, __loader_android_create_namespace);
        LOAD_DLSYM(libdl, __loader_android_dlopen_ext);

        auto ns = __loader_android_create_namespace(
            "python",
            ("/system/lib64/:/system/product/lib64/:" + pythonHome + "/lib").c_str(),
            "/system/lib64/",
            ANDROID_NAMESPACE_TYPE_SHARED |
            ANDROID_NAMESPACE_TYPE_ISOLATED,
            "/system/:/data/:/vendor/",
            NULL);
        if(!ns) {
            LOG_ERROR("Couldn't create namespace");
            return false;
        }
        const android_dlextinfo dlextinfo = {
                .flags = ANDROID_DLEXT_USE_NAMESPACE,
                .library_namespace = ns,
                };
        auto libpython = __loader_android_dlopen_ext("libpython3.8.so", RTLD_LOCAL | RTLD_NOW, &dlextinfo);
        if(!libpython) {
            LOG_ERROR("Couldn't dlopen: libpython3.8.so");
            return false;
        }
        LOAD_DLSYM(libpython, Py_BytesMain);
        setenv("PYTHONHOME", pythonHome.c_str(), 1);     
        setenv("SSL_CERT_FILE", (pythonHome + "/etc/tls/cert.pem").c_str(), 1); 
        return true;
    }

    bool LoadPython() {
        static std::optional<bool> loaded = std::nullopt;
        if(!loaded.has_value())
            loaded = LoadPythonDirect();
        return loaded.value();
    }

    int RunCommand(const std::string& command) {
        LOG_INFO("Python::RunCommand: \"%s\"", command.c_str());
        if(!LoadPython()) {
            return -1;
        }
        auto args = StringUtils::Split(command, " ");
        auto size = args.size();
        char** argv = new char*[size+1];
        argv[0] = const_cast<char*>(FileUtils::getScriptsPath().c_str());
        for(int i = 0; i < size; i++) {
            argv[i+1] = const_cast<char*>(args[i].c_str());
        }
        return Py_BytesMain(size+1, argv);
    }

}