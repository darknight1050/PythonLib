#pragma once
#include <string>

#define DECLARE_DLSYM(retval, name, ...) \
inline retval(*name)(__VA_ARGS__);

namespace Python {

    class PyObject { };

    typedef ssize_t Py_ssize_t;

    bool LoadPython();

    int RunCommand(const std::string& args);

    DECLARE_DLSYM(int, Py_BytesMain, int argc, char** argv);
    DECLARE_DLSYM(void, Py_Initialize);
    DECLARE_DLSYM(void, Py_Finalize);
    DECLARE_DLSYM(PyObject*, PyImport_ImportModule, const char *name);
    DECLARE_DLSYM(int, PyCallable_Check, PyObject*  o);
    DECLARE_DLSYM(void, PyErr_Print);
    DECLARE_DLSYM(PyObject*, PyModule_GetDict, PyObject* module);
    DECLARE_DLSYM(PyObject*, PyDict_GetItemString, PyObject* p, const char *key);
    DECLARE_DLSYM(PyObject*, Py_BuildValue, const char *format, ...);
    DECLARE_DLSYM(PyObject*, PyObject_CallObject, PyObject* callable, PyObject* args);
    DECLARE_DLSYM(long, PyLong_AsLong, PyObject* obj);
    DECLARE_DLSYM(void, Py_DecRef, PyObject* o);
    DECLARE_DLSYM(const char *, PyUnicode_AsUTF8, PyObject *obj);
}