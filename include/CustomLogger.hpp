#pragma once

//#define LOG_INFO(value...)
#define LOG_INFO(value...) PythonLogger.info(value) 
//#define LOG_DEBUG(value...) 
#define LOG_DEBUG(value...) PythonLogger.debug(value) 
//#define LOG_ERROR(value...)
#define LOG_ERROR(value...) PythonLogger.error(value) 

#include "paper/shared/logger.hpp"
static auto constexpr PythonLogger = Paper::ConstLoggerContext("PythonLib");