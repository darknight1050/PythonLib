#pragma once
#include <optional>
#include "Python.hpp"



#define DECLARE_DLSYM_INLINE(retval, name, ...) \
inline retval(*name)(__VA_ARGS__);

#define DEFINE_DLSYM(retval, name, ...) \
retval(*name)(__VA_ARGS__) = NULL;

#define LOAD_DLSYM(handle, name) \
dlerror(); \
*reinterpret_cast<void**>(&name) = dlsym(handle, #name); \
auto name##Error = dlerror(); \
if(name##Error) { \
    LOG_ERROR("Couldn't dlsym %s: %s", #name, name##Error); \
    return false; \
}

#define DEFINE_DLSYM_TYPE(name) \
PyTypeObject* name;

#define LOAD_DLSYM_TYPE(handle, name) \
dlerror(); \
name = reinterpret_cast<PyTypeObject*>(dlsym(handle, #name)); \
auto name##Error = dlerror(); \
if(name##Error) { \
    LOG_ERROR("Couldn't dlsym %s: %s", #name, name##Error); \
    return false; \
}

namespace Python {
    
    const int ANDROID_NAMESPACE_TYPE_ISOLATED = 1;
    const int ANDROID_NAMESPACE_TYPE_SHARED = 2;
    const int ANDROID_DLEXT_USE_NAMESPACE = 0x200;

    struct android_namespace_t {

    };

    struct android_dlextinfo {
        uint64_t flags;
        void*   reserved_addr;
        size_t  reserved_size;
        int     relro_fd;
        int     library_fd;
        off64_t library_fd_offset;
        struct android_namespace_t* library_namespace;
    };
    
    DECLARE_DLSYM_INLINE(android_namespace_t*, __loader_android_create_namespace, const char* name, const char* ld_library_path, const char* default_library_path, uint64_t type, const char* permitted_when_isolated_path, struct android_namespace_t* parent);
    DECLARE_DLSYM_INLINE(void*, __loader_android_dlopen_ext, const char* filename, int flag, const android_dlextinfo* extinfo);
    
    
    bool LoadPython();

    bool Load_Dlsym(void* libpython);
    
}