#pragma once

#ifdef HW_PLATFORM_WINDOWS
	#ifdef HW_BUILD_DLL
		#define HOLLOW_API __declspec(dllexport)
	#else
		#define HOLLOW_API __declspec(dllimport)
	#endif
#else
	#error Hollow supports windows
#endif

#define BIT(x) (1<<x)