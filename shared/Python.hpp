#pragma once
#include <string>

#define DECLARE_DLSYM(retval, name, ...) \
inline retval(*name)(__VA_ARGS__);

namespace Python {

    int RunCommand(const std::string& args);

}