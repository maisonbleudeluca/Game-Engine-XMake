#pragma once
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <spdlog/spdlog.h>

class Logger
{
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> GetCoreLogger()
    {
        return CoreLogger;
    }

  private:
    static inline std::shared_ptr<spdlog::logger> CoreLogger;
};

#define ENGINE_CORE_TRACE(...) Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...) Logger::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...) Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
