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
        auto scriptsPath = FileUtils::getScriptsPath();
        auto pythonHome = pythonPath + "/usr";
        LOG_INFO("PythonPath: %s", pythonPath.c_str());
        if(!direxists(pythonHome)) {
            mkpath(pythonPath);
            int args = 2;
            std::string_view data = IncludedAssets::python_zip;
            int statusCode = zip_stream_extract(data.data(), data.length(), pythonPath.c_str(), 
                +[](const char* name, void* arg) -> int {
                    return 0;
                }, &args);
        }
        dlerror();
        auto libdl = dlopen("libdl.so", RTLD_NOW | RTLD_GLOBAL);
        auto libdlError = dlerror();
        if(libdlError) {
            LOG_ERROR("Couldn't dlopen libdl.so: %s", libdlError);
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
        dlerror();
        auto libpython = __loader_android_dlopen_ext("libpython3.8.so", RTLD_LOCAL | RTLD_NOW, &dlextinfo);
        auto libpythonError = dlerror();
        if(libpythonError) {
            LOG_ERROR("Couldn't dlopen libpython3.8.so: %s", libpythonError);
            return false;
        }
        LOAD_DLSYM(libpython, Py_BytesMain);
        LOAD_DLSYM(libpython, Py_Initialize);
        LOAD_DLSYM(libpython, Py_Finalize);
        LOAD_DLSYM(libpython, PyImport_ImportModule);
        LOAD_DLSYM(libpython, PyCallable_Check);
        LOAD_DLSYM(libpython, PyErr_Print);
        LOAD_DLSYM(libpython, PyModule_GetDict);
        LOAD_DLSYM(libpython, PyDict_GetItemString);
        LOAD_DLSYM(libpython, Py_BuildValue);
        LOAD_DLSYM(libpython, PyObject_CallObject);
        LOAD_DLSYM(libpython, PyLong_AsLong);
        LOAD_DLSYM(libpython, Py_DecRef);
        LOAD_DLSYM(libpython, PyUnicode_AsUTF8);
        setenv("PYTHONHOME", pythonHome.c_str(), 1);     
        setenv("PYTHONPATH", scriptsPath.c_str(), 1);     
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