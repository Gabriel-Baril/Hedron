#pragma once

#include <type_traits>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define IN_USE		&&
#define NOT_IN_USE	&&!
#define USE_IF( x )	&&((x) ? 1 : 0)&&
#define USING( x )	(1 x 1)

#define HDN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)
#define ARRLEN(arr) (sizeof(arr) / sizeof(*arr))

#ifdef _HDN_DEBUG
#define HDN_DEBUG IN_USE
#else
#define HDN_DEBUG NOT_IN_USE
#endif

#ifdef _HDN_RELEASE
#define HDN_RELEASE IN_USE
#else
#define HDN_RELEASE NOT_IN_USE
#endif

#ifdef _HDN_RETAIL
#define HDN_RETAIL IN_USE
#else
#define HDN_RETAIL NOT_IN_USE
#endif

#define HDN_PLATFORM_WINDOWS		NOT_IN_USE
#define HDN_PLATFORM_IOS			NOT_IN_USE
#define HDN_PLATFORM_MACOS			NOT_IN_USE
#define HDN_PLATFORM_ANDROID		NOT_IN_USE
#define HDN_PLATFORM_LINUX			NOT_IN_USE

#if defined(_WIN32)
	#if defined(_WIN64)
		#undef HDN_PLATFORM_WINDOWS
		#define HDN_PLATFORM_WINDOWS IN_USE
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(_MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#undef HDN_PLATFORM_IOS
		#define HDN_PLATFORM_IOS IN_USE
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#undef HDN_PLATFORM_MACOS
		#define HDN_PLATFORM_MACOS IN_USE
		#error "MaxOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#undef HDN_PLATFORM_ANDROID
	#define HDN_PLATFORM_ANDROID IN_USE
	#error "Android is not supported!"
#elif defined(__linux__)
	#undef HDN_PLATFORM_LINUX
	#define HDN_PLATFORM_LINUX IN_USE
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

#define DEV					USE_IF( USING(HDN_DEBUG) )
#define ENABLE_LOG			USE_IF( USING(DEV) )
#define ENABLE_ASSERT		USE_IF( USING(DEV) )
#define ENABLE_THROW		NOT_IN_USE //USE_IF( USING(DEV) )
