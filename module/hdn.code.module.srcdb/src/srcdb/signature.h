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
	void signature_register(obj_t id, void* data, u64 size, u64 alignment);

	template<typename T, typename... Ts>
	constexpr auto make_signature(Ts&&... args)
		noexcept(std::is_nothrow_constructible_v<T, Ts&&...>)
		-> Signature<T>
	{
		HASSERT(signature_initialized(), "The signature system must be initialized");
		const auto sig = Signature<T>(std::forward<Ts>(args)...);
		signature_register(object_get_id(sig), &sig, sizeof(decltype(sig)), alignof(decltype(sig)));
		return sig;
	}
}