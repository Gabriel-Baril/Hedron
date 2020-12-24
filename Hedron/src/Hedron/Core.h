#pragma once



#ifdef HDR_PLATFORM_WINDOWS
	#ifdef HDR_BUILD_DLL
		#define HEDRON_API __declspec(dllexport)
	#else
		#define HEDRON_API __declspec(dllimport)
	#endif
#else
	#error Hedron only support Windows!
#endif

#define BIT(x) (1 << x)