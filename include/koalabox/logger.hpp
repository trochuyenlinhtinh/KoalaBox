#pragma once

#include <koalabox/core.hpp>

#include <spdlog/spdlog.h>

#define LOG_MESSAGE(...)

// Define trace in a special way to avoid excessive logging in release builds
#ifdef _DEBUG
#define LOG_TRACE(fmt, ...) LOG_MESSAGE(trace, fmt, __VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

#define LOG_DEBUG(fmt, ...)     LOG_MESSAGE(debug, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)      LOG_MESSAGE(warn, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)      LOG_MESSAGE(info, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...)     LOG_MESSAGE(error, fmt, __VA_ARGS__)
#define LOG_CRITICAL(fmt, ...)  LOG_MESSAGE(critical, fmt, __VA_ARGS__)
