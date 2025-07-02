#include "core/core.h"
#include "hobj/hobj.h"

#include "tlsf/tlsf.h"

namespace hdn
{
	class HDummyObject : public HObject
	{
	public:
		HDummyObject()
		{
			HINFO("Called HDummyObject constructor");
		}
	};
}

void iterate_pool(void* ptr, size_t size, int used, void* user)
{
	HINFO("size: {0}, Used: {1}", size, used);
}

int main()
{
	using namespace hdn;
	log_init();

	const u64 allocatedSize = 1024 * 100;

	void* mem = new u8[allocatedSize];
	tlsf_t tlsf = tlsf_create_with_pool(mem, allocatedSize);
	void* block1 = tlsf_malloc(tlsf, sizeof(HDummyObject));
	HDummyObject* object = new (block1) HDummyObject();

	tlsf_walk_pool(tlsf_get_pool(tlsf), iterate_pool, nullptr);

	tlsf_free(tlsf, block1);
	tlsf_destroy(tlsf);
	delete[] mem;

	return 0;
}
