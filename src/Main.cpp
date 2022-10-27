#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "zip.h"
#include "assets.hpp"
#include "CustomLogger.hpp"

ModInfo modInfo;

Logger& getLogger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, true)); 
    return *logger; 
}

extern "C" void setup(ModInfo& info) {
    modInfo.id = "PythonLib";
    modInfo.version = VERSION;
    info = modInfo;
}

#ifndef DATA_DIR
#define DATA_DIR "/data/data/%s/files/python/"
#endif

std::string getDataHome() {
    static std::string dataHome = string_format(DATA_DIR, Modloader::getApplicationId().c_str());
    if(!direxists(dataHome)) {
        mkpath(dataHome);
        int args = 2;
        std::string_view data = IncludedAssets::python_zip;
        int statusCode = zip_stream_extract(data.data(), data.length(), dataHome.c_str(), 
            +[](const char* name, void* arg) -> int {
                return 0;
            }, &args);
    }
    return dataHome;
}

std::string getModDataHome() {
    static std::string modDataHome = getDataDir(modInfo);
    if(!direxists(modDataHome))
        mkpath(modDataHome);
    static std::string ytdlp = modDataHome + "ytdlp";
    if(!fileexists(ytdlp)) {
        writefile(ytdlp, IncludedAssets::ytdlp_zip);
    }
    return modDataHome;
}

const int ANDROID_NAMESPACE_TYPE_ISOLATED = 1;
const int ANDROID_NAMESPACE_TYPE_SHARED = 2;
const int ANDROID_DLEXT_USE_NAMESPACE = 0x200;
struct android_namespace_t { };
typedef struct {
  uint64_t flags;
  void*   reserved_addr;
  size_t  reserved_size;
  int     relro_fd;
  int     library_fd;
  off64_t library_fd_offset;
  struct android_namespace_t* library_namespace;
} android_dlextinfo;
typedef android_namespace_t*(*android_create_namespace_t)(const char* name, const char* ld_library_path, const char* default_library_path, uint64_t type, const char* permitted_when_isolated_path, struct android_namespace_t* parent);
typedef void*(*android_dlopen_ext_t)(const char* filename, int flag, const android_dlextinfo* extinfo);

typedef int(*Py_BytesMain_t)(int argc, char **argv);

void doStuff() {
    auto libdl = dlopen("libdl.so", RTLD_NOW | RTLD_GLOBAL);
    LOG_INFO("libdl %p", libdl);
    auto android_create_namespace = (android_create_namespace_t)dlsym(libdl, "__loader_android_create_namespace");
    LOG_INFO("android_create_namespace %p", android_create_namespace);
    auto android_dlopen_ext = (android_dlopen_ext_t)dlsym(libdl, "__loader_android_dlopen_ext");
    LOG_INFO("android_dlopen_ext %p", android_dlopen_ext);

    std::string pythonHome = getDataHome() + "usr";
    LOG_INFO("pythonHome %s", pythonHome.c_str());
    std::string ytdlp = getModDataHome() + "ytdlp";
    LOG_INFO("ytdlp %s", ytdlp.c_str());

    auto ns = android_create_namespace(
      "trustme",
      ("/system/lib64/:/system/product/lib64/:" + pythonHome + "/lib").c_str(),
      "/system/lib64/",
      ANDROID_NAMESPACE_TYPE_SHARED |
      ANDROID_NAMESPACE_TYPE_ISOLATED,
      "/system/:/data/:/vendor/",
      NULL);
    LOG_INFO("ns %p", ns);
    const android_dlextinfo dlextinfo = {
        .flags = ANDROID_DLEXT_USE_NAMESPACE,
        .library_namespace = ns,
        };
    auto libpython = android_dlopen_ext("libpython3.8.so", RTLD_LOCAL | RTLD_NOW, &dlextinfo);
    LOG_INFO("libpython %p", libpython);
    if(!libpython) {
        LOG_INFO("dlerror: %s", dlerror());
        return;
    }
    auto Py_BytesMain = (Py_BytesMain_t)dlsym(libpython, "Py_BytesMain");
    LOG_INFO("Py_BytesMain %p", Py_BytesMain);

    setenv("PYTHONHOME", pythonHome.c_str(), 1);
    setenv("SSL_CERT_FILE", (pythonHome+ "/etc/tls/cert.pem").c_str(), 1);
    const int size = 6;
    const char* argv[size] = { "", ytdlp.c_str(), "--no-cache-dir", "-P", "/sdcard", "https://youtu.be/SnP0Nqp455I" };
    LOG_INFO("Py_BytesMain Result: %d", Py_BytesMain(size, (char**)argv));
    /*Py_Initialize();
    PyObject *pModule, *pDict, *pFunc, *pValue, *presult;


   LOG_INFO("Py_Initialize");
   // Initialize the Python Interpreter
   
   // Load the module object
   pModule = PyImport_ImportModule("arbName");
    if (!PyCallable_Check(pModule))
   {
       PyErr_Print();
   }

   LOG_INFO("PyModule_GetDict %p", pModule);
   // pDict is a borrowed reference 
   pDict = PyModule_GetDict(pModule);


   LOG_INFO("PyDict_GetItemString");
   // pFunc is also a borrowed reference 
   pFunc = PyDict_GetItemString(pDict, (char*)"someFunction");

   LOG_INFO("PyCallable_Check");
   if (PyCallable_Check(pFunc))
   {
       pValue=Py_BuildValue("(z)",(char*)"something");
       PyErr_Print();
       LOG_INFO("Let's give this a shot!");
       presult=PyObject_CallObject(pFunc,pValue);
       PyErr_Print();
   } else 
   {
       PyErr_Print();
   }
   LOG_INFO("Result is %ld", PyLong_AsLong(presult));
   Py_DECREF(pValue);

   // Clean up
   Py_DECREF(pModule);

   // Finish the Python Interpreter
   Py_Finalize();*/
}

#include <iostream>
#include <fstream>
extern "C" void load() {
    LOG_INFO("Starting PythonLib installation...");
    il2cpp_functions::Init();
    LOG_INFO("Successfully installed PythonLib!");
    
    //freopen("/sdcard/out.txt", "w", stdout);
    //freopen("/sdcard/err.txt", "w", stderr);
    std::thread t([](){
        doStuff();
    });
    t.detach();
}