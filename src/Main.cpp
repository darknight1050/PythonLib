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

static PyObject *
nativelib_std_write(PyObject *self, PyObject *args)
{
    int type;
    char* data;
    if (PyArg_ParseTuple(args, "is", &type, &data)) {
        switch (type) {
        case 0:
            LOG_INFO("Stdout: %s", data);
            break;
        case 1:
            LOG_ERROR("Stderr: %s", data);
            break;
        }
        PythonWriteEvent.invoke(type, data);
    }
    Py_RETURN_NONE;
}

static PyMethodDef NativeLibMethods[] = {
    {"std_write", nativelib_std_write, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL} 
};

static struct PyModuleDef NativeLibModule = {
    PyModuleDef_HEAD_INIT,
    "nativelib", 
    nullptr,
    -1, 
    NativeLibMethods
};

bool doStuff() {
    if(!LoadPython())
        return false;

    std::string redirectStd = FileUtils::getScriptsPath() + "/redirectStd.py";
    writefile(redirectStd, IncludedAssets::redirectStd_py);
    
    Py_Initialize();
    
    AddNativeModule(NativeLibModule);

    PyObject* pModule = PyImport_ImportModule("redirectStd");
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
    Py_DECREF(pModule);
    //Py_Finalize();
    return true;
}

extern "C" void setup(ModInfo& info) {
    modInfo.id = "PythonLib";
    modInfo.version = VERSION;
    info = modInfo;

    LOG_INFO("Setting up PythonLib...");
    if(doStuff()){
        LOG_INFO("Successfully setup PythonLib!");
    }else {
        LOG_INFO("Error while setting up PythonLib!");
    }
    //freopen("/sdcard/out.txt", "w", stdout);
    //freopen("/sdcard/err.txt", "w", stderr);
}

extern "C" void load() {
    il2cpp_functions::Init();
}