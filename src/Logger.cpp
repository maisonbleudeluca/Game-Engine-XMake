#include "Logger.hpp"

void Logger::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    CoreLogger = spdlog::stdout_color_mt("Engine");
};