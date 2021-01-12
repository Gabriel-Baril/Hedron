#pragma once

#include <memory>
#include "Hedron/Debug/Instrumentor.h"


#if defined(_WIN32)
	#if defined(_WIN64)
		#define HDR_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(_MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HDR_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HDR_PLATFORM_MACOS
		#error "MaxOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define HDR_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define HDR_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

#if defined(HDR_PLATFORM_WINDOWS)
	#if HDR_DYNAMIC_LINK
		#if defined(HDR_BUILD_DLL)
			#define HEDRON_API __declspec(dllexport)
		#else
			#define HEDRON_API __declspec(dllimport)
		#endif
	#else
		#define HEDRON_API
	#endif
#else
	#error Hedron only support Windows!
#endif

#if defined(HDR_DEBUG)
	#define HDR_ENABLE_ASSERTS
#endif

#if defined(HDR_ENABLE_ASSERTS)
	#define HDR_ASSERT(x, ...) { if(!(x)) { HDR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HDR_CORE_ASSERT(x, ...) { if(!(x)) { HDR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HDR_ASSERT(x, ...)
	#define HDR_CORE_ASSERT(x, ...)
#endif

#define HDR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)

namespace Hedron
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> create_scope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	
	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}