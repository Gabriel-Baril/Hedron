#include "core/core.h"
#include "cache.h"
#include "example_compute.h"

int main()
{
	using namespace hdn;
	log_init();
	cache_init();
	
	ExampleCompute compute = example_compute(2, 1);
	HINFO("Example Compute: ({0}, {1})", compute.number, compute.count);

	return 0;
}