#include "core/core.h"
#include "core/core_filesystem.h"

#include "hdef_model.h"

int main()
{
	using namespace hdn;
	Log_Init();
	
	vector<byte> out;
	byte vertices[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	hdef_model_builder builder;
	builder.set_version(5);
	builder.set_vertices(vertices, sizeof(vertices));
	builder.builder.write_out(12, out);

	hdef_model model{ out };

	HINFO("Version: {0}", model.get_version());
	const byte* data = model.get_vertices();
	for (int i = 0;i < 9; i++)
	{
		HINFO("data[{0}] = {1}", i, data[i]);
	}

	return 0;
}