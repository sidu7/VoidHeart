#pragma once

#include <memory>

#include "Hollow/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Hollow {

	/// <summary>
	/// Class Log.
	/// </summary>
	class Log
	{
	public:
		/// <summary>
		/// Initializes this Logger.
		/// </summary>
		HOLLOW_API static void Init();
		/// <summary>
		/// Gets the core logger.
		/// </summary>
		/// <returns>std.shared_ptr of type logger.</returns>
		HOLLOW_API inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		/// <summary>
		/// Gets the client logger.
		/// </summary>
		/// <returns>std.shared_ptr of type logger.</returns>
		HOLLOW_API inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define HW_CORE_TRACE(...)  ::Hollow::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HW_CORE_INFO(...)   ::Hollow::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HW_CORE_WARN(...)   ::Hollow::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HW_CORE_ERROR(...)  ::Hollow::Log::GetCoreLogger()->error(__VA_ARGS__); __debugbreak()
#define HW_CORE_FATAL(...)  ::Hollow::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define HW_TRACE(...)       ::Hollow::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HW_INFO(...)        ::Hollow::Log::GetClientLogger()->info(__VA_ARGS__)
#define HW_WARN(...)        ::Hollow::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HW_ERROR(...)       ::Hollow::Log::GetClientLogger()->error(__VA_ARGS__); __debugbreak()
#define HW_FATAL(...)       ::Hollow::Log::GetClientLogger()->fatal(__VA_ARGS__)

