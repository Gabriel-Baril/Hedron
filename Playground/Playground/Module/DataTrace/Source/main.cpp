#include <iostream>

#include "Playground/BufferWriter.h"
#include "Core/Core.h"

#define DEGIN_META() { GroupMetadata meta;
#define META(key, value) meta.Meta(key, value);
#define END_META() }

#define WRITE(x) writer.Write(x);

#define PUSH_WRITE(x) writer.PushGroup(meta); writer.Write(x); writer.PopGroup();

struct Data
{
	float f0 = 2;
	float f1 = 7;
	int n0 = 1;
	char arr[16];
};

int main()
{
	using namespace hdn;
	hdn::Log_Init();

	byte* buffer = new byte[1000];

	Data data;

	BufferWriter writer(buffer);

	writer.PushGroup("Data:data");

	// GroupMetadata meta;
	// meta.Meta(GroupMetadata::Type::STRING, "semantic", "f0");
	// meta.Meta(GroupMetadata::Type::STRING, "type", "float");
	// meta.Meta(GroupMetadata::Type::UINT32, "type.size", sizeof(data.f0));
	writer.PushGroup("float:f0");
	writer.Write(data.f0);
	writer.PopGroup();

	writer.PushGroup("float:f1");
	writer.Write(data.f1);
	writer.PopGroup();

	writer.PushGroup("int:n0");
	writer.Write(data.n0);
	writer.PopGroup();

	writer.PopGroup();

	const auto& groups = writer.GetGroups();
	for (const auto& group : groups)
	{
		HDN_CORE_INFO("Semantic: {0}", group.GetSemantic());
		for (const auto& childIndex : group.GetChildren())
		{
			const auto& child = groups.at(childIndex);
			HDN_CORE_WARNING("Children: {0}", child.GetSemantic());
		}
	}

	delete[] buffer;

	HDN_CORE_INFO("Hello Playground");
	return 0;
}