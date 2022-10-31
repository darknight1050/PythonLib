#pragma once
#include <string>
#include <beatsaber-hook/shared/utils/typedefs-wrappers.hpp>

namespace Python {

    extern UnorderedEventCallback<int, char*> PythonWriteEvent;

    int RunCommand(const std::string& args);

}