#pragma once
#include "core_define.h"

#define HDN_MODULE_CORE_API

#if USING(HDN_PLATFORM_WINDOWS)
	#if _HDN_MODULE_CORE_DYNAMIC_LINK
		#if defined(_HDN_MODULE_CORE_BUILD_DLL)
			#define HDN_MODULE_CORE_API __declspec(dllexport)
		#else
			#define HDN_MODULE_CORE_API __declspec(dllimport)
		#endif
	#else
		#define HDN_MODULE_CORE_API
	#endif
#endif