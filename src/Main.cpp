#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "CustomLogger.hpp"
#include "ModInfo.hpp"
#include "Utils/FileUtils.hpp"
#include "assets.hpp"
#include "PythonInternal.hpp"

using namespace Python;

#define SO_NAME "libpythonlib.so"

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
    if(!LoadPython())
        return;

    std::string redirectStd = FileUtils::getScriptsPath() + "/redirectStd.py";
    if(!fileexists(redirectStd))
        writefile(redirectStd, IncludedAssets::redirectStd_py);
    PyObject *pModule, *pDict, *pFunc, *pValue, *presult;
   
    Py_Initialize();
    pModule = PyImport_ImportModule("redirectStd");
    if (!PyCallable_Check(pModule))
    {
        PyErr_Print();
    }
    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, (char*)"setHandle");
    if (PyCallable_Check(pFunc))
    {
        auto handle = dlopen(SO_NAME, RTLD_LAZY);
        pValue=Py_BuildValue("(K)", handle);
        PyErr_Print();
        presult=PyObject_CallObject(pFunc,pValue);
        PyErr_Print();
    } else 
    {
        PyErr_Print();
    }
    Py_DecRef(pValue);
    Py_DecRef(pModule);
    //Py_Finalize();
}

extern "C" void pythonWrite(int type, char* data) {
    CallPythonWriteEvent(type, data);
    switch (type) {
    case 0:
        LOG_INFO("PythonLib Stdout: %s", data);
        break;
    case 1:
        LOG_ERROR("PythonLib Stderr: %s", data);
        break;
    }
    
}

extern "C" void load() {
    LOG_INFO("Starting PythonLib installation...");
    il2cpp_functions::Init();
    LOG_INFO("Successfully installed PythonLib!");

    doStuff();
}