import nativelib

def GetClass(nameSpace, className):
    return nativelib.get_class(nameSpace, className)

def SetFieldValueStatic(nameSpace, className, fieldName, value):
    nativelib.set_field_value_static(nameSpace, className, fieldName, value)

def GetFieldValueStatic(nameSpace, className, fieldName):
    return nativelib.get_field_value_static(nameSpace, className, fieldName)

def RunMethodStatic(nameSpace, className, methodName, argsCount, *args):
    return nativelib.run_method_static(nameSpace, className, methodName, argsCount, *args)


class Il2CppObject:
    def __init__(self, ptr):
        self.ptr = ptr

    def RunMethod(self, methodName, argsCount = 0, *args):
        return nativelib.run_method_instance(self, methodName, argsCount, *args)

    def SetFieldValue(self, fieldName, value):
        nativelib.set_field_value_instance(self, fieldName, value)

    def GetFieldValue(self, fieldName):
        return nativelib.get_field_value_instance(self, fieldName)

class ValueTypeField:
    def __init__(self, type, data):
        self.type = type
        self.data = data

def test(obj):
    gameObj = RunMethodStatic("Tester", "TestAssetScript", "TestMethod", 1, obj)
    print(gameObj.GetFieldValue("m_CachedPtr"))
    print(gameObj.RunMethod("get_name"))

print("il2cpp Loaded!")