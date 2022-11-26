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

static PyObject *
nativelib_stdWrite(PyObject *self, PyObject *args)
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

void doStuff() {
    if(!LoadPython())
        return;

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
}

extern "C" void load() {
    LOG_INFO("Starting PythonLib installation...");
    il2cpp_functions::Init();
    LOG_INFO("Successfully installed PythonLib!");

    //freopen("/sdcard/out.txt", "w", stdout);  
    //freopen("/sdcard/err.txt", "w", stderr);

    doStuff();
}