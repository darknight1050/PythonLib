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

PyObject* Il2CppObjectType = nullptr;

static PyObject* nativelib_std_write(PyObject* self, PyObject* args) {
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

struct InvokeParam {

    void* data = nullptr;
    bool allocated = false;

    template<class T>
    static InvokeParam New(T t) {
        auto size = sizeof(T);
        auto data = malloc(size);
        memcpy(data, &t, size);
        return {data, true};
    }
    
    template<class T>
    static InvokeParam From(T t) {
        return {reinterpret_cast<void*>(t), false};
    }

    void Release() {
        if(allocated)
            free(data);
    }

    static InvokeParam Null() {
        static InvokeParam null = {nullptr, false};
        return null;
    }

};

PyObject* BuildIl2CppObject(Il2CppObject* obj) {
    return PyObject_CallObject(Il2CppObjectType, Py_BuildValue("(O)", PyLong_FromVoidPtr(reinterpret_cast<void*>(obj))));
}

PyObject* ToPyObject(Il2CppObject* obj, const Il2CppType* type) {
    switch (type->type)
    {
    case IL2CPP_TYPE_CLASS:
        return BuildIl2CppObject(obj);
    case IL2CPP_TYPE_VALUETYPE:
        LOG_INFO("instance_size %d", il2cpp_functions::object_get_class(obj)->instance_size);
        LOG_INFO("actualSize %d", il2cpp_functions::object_get_class(obj)->actualSize);
        break;
    case IL2CPP_TYPE_BOOLEAN:
        return PyBool_FromLong(*il2cpp_utils::FromIl2CppObject<bool>(obj));
    case IL2CPP_TYPE_CHAR:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<Il2CppChar>(obj));
    case IL2CPP_TYPE_I1:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<int8_t>(obj));
    case IL2CPP_TYPE_U1:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<uint8_t>(obj));
    case IL2CPP_TYPE_I2:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<int16_t>(obj));
    case IL2CPP_TYPE_U2:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<uint16_t>(obj));
    case IL2CPP_TYPE_I4:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<int32_t>(obj));
    case IL2CPP_TYPE_U4:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<uint32_t>(obj));
    case IL2CPP_TYPE_I8:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<int64_t>(obj));
    case IL2CPP_TYPE_U8:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<uint64_t>(obj));
    case IL2CPP_TYPE_I:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<int64_t>(obj));
    case IL2CPP_TYPE_U:
        return PyLong_FromLong(*il2cpp_utils::FromIl2CppObject<uint64_t>(obj));
    case IL2CPP_TYPE_R4:
        return PyFloat_FromDouble(*il2cpp_utils::FromIl2CppObject<float>(obj));
    case IL2CPP_TYPE_R8:
        return PyFloat_FromDouble(*il2cpp_utils::FromIl2CppObject<double>(obj));
    case IL2CPP_TYPE_STRING:
        if(obj)
            return PyUnicode_FromString(StringW(reinterpret_cast<Il2CppString*>(obj)).operator std::__ndk1::string().c_str());
        break;
    default:
        break;
    }
    Py_RETURN_NONE;
}

InvokeParam ExtractInvokeParameter(PyObject* param, const Il2CppType* type) {
    switch (type->type)
    {
    case IL2CPP_TYPE_CLASS:
        if(PyObject_HasAttrString(param, "ptr")) {
            return InvokeParam::From(PyLong_AsVoidPtr(PyObject_GetAttrString(param, "ptr")));
        }
        break;
    case IL2CPP_TYPE_BOOLEAN:
        if(PyBool_Check(param)) {
            return InvokeParam::New((bool)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_CHAR:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((Il2CppChar)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_I1:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((int8_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_U1:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((uint8_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_I2:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((int16_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_U2:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((uint16_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_I4:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((int32_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_U4:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((uint32_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_I8:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((int64_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_U8:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((uint64_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_I:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((int64_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_U:
        if(PyLong_CheckExact(param)) {
            return InvokeParam::New((uint64_t)PyLong_AsLong(param));
        }
        break;
    case IL2CPP_TYPE_R4:
        if(PyFloat_CheckExact(param)) {
            return InvokeParam::New((float)PyFloat_AsDouble(param));
        }
        break;
    case IL2CPP_TYPE_R8:
        if(PyFloat_CheckExact(param)) {
            return InvokeParam::New(PyFloat_AsDouble(param));
        }
        break;
    case IL2CPP_TYPE_STRING:
        if(PyUnicode_CheckExact(param)) {
            return InvokeParam::From(StringW(PyUnicode_AsUTF8(param)).operator Il2CppString *());
        }
        break;
    default:
        break;
    }
    return InvokeParam::Null();
}

std::vector<InvokeParam> ExtractInvokeParameters(std::vector<PyObject*> params, const MethodInfo* method) {
    auto search = std::find(params.begin(), params.end(), nullptr);
    if (search != params.end()) {
        params.resize(search - params.begin());
    }
    std::vector<InvokeParam> out;
    for(int i = 0; i < params.size(); i++) {
        out.push_back(ExtractInvokeParameter(params[i], method->parameters[i].parameter_type));
    }
    return out;
}

static PyObject* nativelib_get_class(PyObject* self, PyObject* args)
{ 
    PyObject* nameSpace = NULL;
    PyObject* className = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_get_class", 0, 2, &nameSpace, &className)) {
        return BuildIl2CppObject((Il2CppObject*)il2cpp_utils::GetClassFromName(PyUnicode_AsUTF8(nameSpace), PyUnicode_AsUTF8(className)));
    }
    Py_RETURN_NONE;
}


/*
static PyObject* nativelib_get_field_value_static(PyObject* self, PyObject* args)
{
    PyObject* nameSpace = NULL;
    PyObject* className = NULL;
    PyObject* fieldName = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_get_field_value_static", 0, 2, &nameSpace, &className, &fieldName)) {
        auto field = il2cpp_utils::FindField(PyUnicode_AsUTF8(nameSpace), PyUnicode_AsUTF8(className), PyUnicode_AsUTF8(fieldName));
        switch (field->type->type)
            {
            case IL2CPP_TYPE_CLASS:
            {
                Il2CppObject* ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return BuildIl2CppObject(ret);
            }
            case IL2CPP_TYPE_BOOLEAN:
            {
                bool ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_CHAR:
            {
                Il2CppChar ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_I1:
            {
                int8_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_U1:
            {
                uint8_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_I2:
            {
                int16_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_U2:
            {
                uint16_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_I4:
            {
                int32_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_U4:
            {
                uint32_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_I8:
            {
                int64_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_U8:
            {
                uint64_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_I:
            {
                int64_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_U:
            {
                uint64_t ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_R4:
            {
                float ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_R8:
            {
                double ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                return PyBool_FromLong(ret);
            }
            case IL2CPP_TYPE_STRING:
            {
                Il2CppString* ret;
                il2cpp_functions::field_get_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, &ret);
                if(ret)
                    return PyUnicode_FromString(StringW(ret).operator std::__ndk1::string().c_str());
                break;
            }
            default:
                break;
            }
    }
    Py_RETURN_NONE;
}*/

static PyObject* nativelib_set_field_value_static(PyObject* self, PyObject* args)
{
    PyObject* nameSpace = NULL;
    PyObject* className = NULL;
    PyObject* fieldName = NULL;
    PyObject* value = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_set_field_value_static", 0, 4, &nameSpace, &className, &fieldName, &value)) {
        auto field = il2cpp_utils::FindField(PyUnicode_AsUTF8(nameSpace), PyUnicode_AsUTF8(className), PyUnicode_AsUTF8(fieldName));
        auto invokeParam = ExtractInvokeParameter(value, field->type);
        il2cpp_functions::field_static_set_value(field, invokeParam.data);
        invokeParam.Release();
    }
    Py_RETURN_NONE;
}

static PyObject* nativelib_get_field_value_instance(PyObject* self, PyObject* args)
{
    PyObject* instance = NULL;
    PyObject* fieldName = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_get_field_value_instance", 0, 2, &instance, &fieldName)) {
        if(PyObject_HasAttrString(instance, "ptr")) {
            auto instancePtr = PyLong_AsVoidPtr(PyObject_GetAttrString(instance, "ptr"));
            auto field = il2cpp_utils::FindField(il2cpp_functions::object_get_class(reinterpret_cast<Il2CppObject*>(instancePtr)), PyUnicode_AsUTF8(fieldName));
            Il2CppObject* ret = il2cpp_functions::field_get_value_object(field, reinterpret_cast<Il2CppObject*>(instancePtr));
            return ToPyObject(ret, field->type);
        }
    }
    Py_RETURN_NONE;
}

static PyObject* nativelib_set_field_value_instance(PyObject* self, PyObject* args)
{
    PyObject* instance = NULL;
    PyObject* fieldName = NULL;
    PyObject* value = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_set_field_value_instance", 0, 3, &instance, &fieldName, &value)) {
        if(PyObject_HasAttrString(instance, "ptr")) {
            auto instancePtr = PyLong_AsVoidPtr(PyObject_GetAttrString(instance, "ptr"));
            auto field = il2cpp_utils::FindField(il2cpp_functions::object_get_class(reinterpret_cast<Il2CppObject*>(instancePtr)), PyUnicode_AsUTF8(fieldName));
            auto invokeParam = ExtractInvokeParameter(value, field->type);
            il2cpp_functions::field_set_value(reinterpret_cast<Il2CppObject*>(instancePtr), field, invokeParam.data);
            invokeParam.Release();
        }
    }
    Py_RETURN_NONE;
}

static PyObject* RunMethod(const MethodInfo* method, void* instance, PyObject* arg0, PyObject* arg1, PyObject* arg2, PyObject* arg3, PyObject* arg4, PyObject* arg5, PyObject* arg6, PyObject* arg7, PyObject* arg8, PyObject* arg9) {
    Il2CppException* exp = nullptr;
    std::vector<InvokeParam> invokeParams = ExtractInvokeParameters({arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9}, method);
    std::vector<void*> voidInvokeParams;
    std::transform(invokeParams.begin(), invokeParams.end(), std::back_inserter(voidInvokeParams), [](InvokeParam param) { return param.data; });
    auto* ret = il2cpp_functions::runtime_invoke(method, instance, voidInvokeParams.data(), &exp);
    for(auto& param : invokeParams) {
        param.Release();
    }
    return ToPyObject(ret, method->return_type);
}

static PyObject* nativelib_run_method_static(PyObject* self, PyObject* args)
{
    PyObject* nameSpace = NULL;
    PyObject* className = NULL;
    PyObject* methodName = NULL;
    PyObject* argsCount = NULL;
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    PyObject* arg2 = NULL;
    PyObject* arg3 = NULL;
    PyObject* arg4 = NULL;
    PyObject* arg5 = NULL;
    PyObject* arg6 = NULL;
    PyObject* arg7 = NULL;
    PyObject* arg8 = NULL;
    PyObject* arg9 = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_run_method_static", 0, 14, &nameSpace, &className, &methodName, &argsCount, &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9)) {
        auto method = il2cpp_utils::FindMethodUnsafe(PyUnicode_AsUTF8(nameSpace), PyUnicode_AsUTF8(className), PyUnicode_AsUTF8(methodName), (int)PyLong_AsLong(argsCount));
        return RunMethod(method, nullptr, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    Py_RETURN_NONE;
}

static PyObject* nativelib_run_method_instance(PyObject* self, PyObject* args)
{
    PyObject* instance = NULL;
    PyObject* methodName = NULL;
    PyObject* argsCount = NULL;
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    PyObject* arg2 = NULL;
    PyObject* arg3 = NULL;
    PyObject* arg4 = NULL;
    PyObject* arg5 = NULL;
    PyObject* arg6 = NULL;
    PyObject* arg7 = NULL;
    PyObject* arg8 = NULL;
    PyObject* arg9 = NULL;
    if (PyArg_UnpackTuple(args, "nativelib_run_method_instance", 0, 14, &instance, &methodName, &argsCount, &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9)) {
        if(PyObject_HasAttrString(instance, "ptr")) {
            auto instancePtr = PyLong_AsVoidPtr(PyObject_GetAttrString(instance, "ptr"));
            auto method = il2cpp_utils::FindMethodUnsafe(reinterpret_cast<Il2CppObject*>(instancePtr), PyUnicode_AsUTF8(methodName), (int)PyLong_AsLong(argsCount));
            return RunMethod(method, instancePtr, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    }
    Py_RETURN_NONE;
}

static PyMethodDef NativeLibMethods[] = {
    {"std_write", nativelib_std_write, METH_VARARGS, ""},
    {"get_class", nativelib_get_class, METH_VARARGS, ""},
    //{"get_field_value_static", nativelib_get_field_value_static, METH_VARARGS, ""},
    {"set_field_value_static", nativelib_set_field_value_static, METH_VARARGS, ""},
    {"get_field_value_instance", nativelib_get_field_value_instance, METH_VARARGS, ""},
    {"set_field_value_instance", nativelib_set_field_value_instance, METH_VARARGS, ""},
    {"run_method_static", nativelib_run_method_static, METH_VARARGS, ""},
    {"run_method_instance", nativelib_run_method_instance, METH_VARARGS, ""},
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

    writefile(FileUtils::getScriptsPath() + "/redirectStd.py", IncludedAssets::redirectStd_py);
    writefile(FileUtils::getScriptsPath() + "/il2cpp.py", IncludedAssets::il2cpp_py);
    
    Py_Initialize();
    
    AddNativeModule(NativeLibModule);

    PyObject* pModule = PyImport_ImportModule("redirectStd");
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
    Py_DECREF(pModule);

    pModule = PyImport_ImportModule("il2cpp");
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
    Il2CppObjectType = PyObject_GetAttrString(pModule, "Il2CppObject");
    PyObject* pValue = PyObject_CallObject(Il2CppObjectType, Py_BuildValue("(i)", 1337));
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
    PyObject* pDict = PyModule_GetDict(pModule); 
    PyObject* pFunc = PyDict_GetItemString(pDict, (char*)"test"); 
    PyObject* presult = PyObject_CallObject(pFunc, Py_BuildValue("(O)", pValue)); 
    Py_DECREF(pValue); 

    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
    Py_DECREF(pModule);
    //Py_Finalize();
}

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
extern "C" void load() {
    LOG_INFO("Starting PythonLib installation...");
    il2cpp_functions::Init();
    LOG_INFO("Successfully installed PythonLib!");

    //freopen("/sdcard/out.txt", "w", stdout);  
    //freopen("/sdcard/err.txt", "w", stderr);
    QuestUI::Init();
    std::thread([]{ QuestUI::MainThreadScheduler::Schedule([]{ doStuff(); });}).detach();
    
}