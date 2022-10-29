#pragma once
#include <string>

#define DECLARE_DLSYM(retval, name, ...) \
extern retval(*name)(__VA_ARGS__);

namespace Python {

    int RunCommand(const std::string& args);

}