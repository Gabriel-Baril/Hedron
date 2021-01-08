#pragma once

#include <memory>
#include "Hedron/Debug/Instrumentor.h"

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