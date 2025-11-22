#pragma once

#include "cache.h"

namespace hdn
{
	// Signature must be trivially copyable
	template<typename T>
	struct Signature {};

	bool signature_initialized();
	void signature_init();
	void signature_shutdown();
	void* signature_get(obj_t id);
	void signature_register(obj_t id, const void* data, u64 size, u64 alignment);

	template<typename T, typename... Args>
	constexpr auto make_signature(Args&&... args)
		noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
		-> Signature<T>
	{
		HASSERT(signature_initialized(), "The signature system must be initialized");
		const auto sig = Signature<T>(std::forward<Args>(args)...);
		signature_register(object_get_id(sig), reinterpret_cast<const void*>(&sig), sizeof(decltype(sig)), alignof(decltype(sig)));
		return sig;
	}
}