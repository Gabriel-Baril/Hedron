#include <iostream>

#include "Core/Core.h"

#include "DataTrace/BufferWriter.h"
#include "DataTrace/BinaryIO.h"

// #include "octree-cpp/OctreeCpp.h"

struct Data
{
	hdn::float32 f0 = 2;
	hdn::float32 f1 = 7;
	hdn::int32 n0 = 1;
	hdn::byte arr[16];
};

namespace hdn::bin
{
	void Write(const Data& object, FBufferWriter& writer)
	{
		bin::Write(object.f0, writer);
		bin::Write(object.f1, writer);
		bin::Write(object.n0, writer);
	}

	void Write(const Data& object, FGroupBufferWriter& writer, const char* semantic)
	{
		bin::Push(writer, semantic);
		bin::Write(object.f0, writer, "float:f0");
		bin::Write(object.f1, writer, "float:f1");
		bin::Write(object.n0, writer, "int:n0");
		bin::Pop(writer);
	}
}

namespace hdn
{
	void PrintGroupHierarchy(const FGroupBufferWriter& writer, const GroupData& group, int depth = 0)
	{
		for (int i = 0;i < depth; i++)
		{
			std::cout << "  ";
		}
		std::cout << group.GetSemantic() << " [" << group.GetBeginByte() << ";" << group.GetEndByte() << "] [" << group.MetadataByteSize() << "]" << std::endl;
		for (const auto& childIndex : group.GetChildren())
		{
			PrintGroupHierarchy(writer, writer.GetGroups().at(childIndex), depth + 1);
		}
	}
}

int main()
{
	using namespace hdn;
#if USING( HDN_DEBUG )
	hdn::Log_Init();
#endif

	byte* buffer = new byte[4096];
	byte* metaBuffer = new byte[4096];

	Data data;
	Data data2;


	// ImmutableKeyValueStore store;
	// store.Push(hdn::KeyValueType::UINT32, "type.size", (uint32)32);

	FGroupBufferWriter writer(buffer, metaBuffer);

	bin::Push(writer, "DataSection");
	bin::Meta(writer, KeyValueType::INT32, "test", 32);
	bin::Meta(writer, KeyValueType::STRING, "test2", "str");

	bin::Write(data, writer, "Data:data");
	bin::Write(data2, writer, "Data:data2");
	bin::Write(data2, writer, "Data:data3");
	bin::Write(data2, writer, "Data:data4");
	bin::Write(data2, writer, "Data:data5");
	bin::Write(12, writer, "int:constant");
	bin::Write(12, writer, "int:constant2");
	bin::Write(data2, writer, "Data:data6");
	for (int i = 0; i < 20; i++)
	{
		bin::Push(writer, "IntSection");
		bin::Meta(writer, KeyValueType::STRING, "test2", "str");
		for (int j = 0; j < i; j++)
		{
			bin::Write(i*j, writer, "int:constant");
			if ((i * j) % 3 == 0)
			{
				bin::Write(data, writer, "Data:data");
			}
		}
		bin::Pop(writer);
	}
	bin::Pop(writer);

	PrintGroupHierarchy(writer, writer.GetGroups()[0]);

	delete[] buffer;
	delete[] metaBuffer;

	HDN_CORE_INFO("Hello Playground");
	return 0;
}