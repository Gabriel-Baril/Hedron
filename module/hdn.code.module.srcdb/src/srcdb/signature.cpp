#include "signature.h"
#include "core/stl/unordered_map.h"
#include <core/memory/allocator_heap.h>
#include <core/memory/core_memory.h>

namespace hdn
{
	static constexpr u64 SIGNATURE_MEMORY_POOL_SIZE = 50 * MB;

	struct SignatureMetadata
	{
		void* data;
		u64 size;
	};

	struct SignatureGlob
	{
		bool initialized = false;
		unordered_map<obj_t, SignatureMetadata> signatures;
		HeapAllocator sigAllocator;
	};

	static SignatureGlob s_SignatureGlob;

	bool signature_initialized()
	{
		return s_SignatureGlob.initialized;
	}

	void signature_init()
	{
		if (s_SignatureGlob.initialized)
		{
			return;
		}

		u8* sigMemory = new u8[SIGNATURE_MEMORY_POOL_SIZE];
		core_memset(sigMemory, 0, SIGNATURE_MEMORY_POOL_SIZE);
		heap_allocator_init(s_SignatureGlob.sigAllocator, sigMemory, SIGNATURE_MEMORY_POOL_SIZE);
		s_SignatureGlob.initialized = true;
	}

	void signature_shutdown()
	{
		if (!s_SignatureGlob.initialized)
		{
			return;
		}

		heap_allocator_shutdown(s_SignatureGlob.sigAllocator);
		delete[] s_SignatureGlob.sigAllocator.memory;
		s_SignatureGlob.initialized = false;
	}

	void* signature_get(obj_t id)
	{
		HASSERT(signature_initialized(), "The signature system must be initialized");
		if (s_SignatureGlob.signatures.contains(id))
		{
			return s_SignatureGlob.signatures.at(id).data;
		}
		return nullptr;
	}

	void* signature_allocate(u64 size, u64 alignment)
	{
		return heap_allocator_allocate(s_SignatureGlob.sigAllocator, size, alignment);
	}

	void signature_register(obj_t id, void* data, u64 size, u64 alignment)
	{
		HASSERT(signature_initialized(), "The signature system must be initialized");
		void* dst = signature_allocate(size, alignment);
		core_memcpy(dst, data, size);

		SignatureMetadata& meta = s_SignatureGlob.signatures[id];
		meta.data = dst;
		meta.size = size;
	}
}
