#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "CustomLogger.hpp"
#include "ModInfo.hpp"
#include "Utils/FileUtils.hpp"
#include "assets.hpp"
#include "Python.hpp"

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


void doStuff() {
    std::string ytdlp = FileUtils::getScriptsPath() + "/ytdlp";
    if(!fileexists(ytdlp)) {
        writefile(ytdlp, IncludedAssets::ytdlp_zip);
    }
    LOG_INFO("Python::RunCommand %d", Python::RunCommand(ytdlp + " --no-cache-dir -P /sdcard https://youtu.be/SnP0Nqp455I"));
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