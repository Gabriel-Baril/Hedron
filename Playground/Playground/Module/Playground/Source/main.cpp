#include <iostream>

#include "Public/BufferWriter.h"
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
		std::cout << "Semantic: " << group.GetSemantic() << std::endl;
		for (const auto& childIndex : group.GetChildren())
		{
			const auto& child = groups.at(childIndex);
			std::cout << "Children: " << child.GetSemantic() << std::endl;
		}
	}

	delete[] buffer;

	std::cout << "Hello Playground" << std::endl;
	return 0;
}