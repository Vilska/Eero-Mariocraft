#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace Eero {

	class Log
	{
	public:
		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
		static void Init();
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

#define LOG_INFO(...)    ::Eero::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)    ::Eero::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)   ::Eero::Log::GetLogger()->error(__VA_ARGS__)
