#pragma once
#include "core/core_define.h"

#define HDN_MODULE_CONFIG_API

#if USING(HDN_PLATFORM_WINDOWS)
	#if _HDN_MODULE_CONFIG_DYNAMIC_LINK
		#if defined(_HDN_MODULE_CONFIG_BUILD_DLL)
			#define HDN_MODULE_CONFIG_API __declspec(dllexport)
		#else
			#define HDN_MODULE_CONFIG_API __declspec(dllimport)
		#endif
	#else
		#define HDN_MODULE_CONFIG_API
	#endif
#endif