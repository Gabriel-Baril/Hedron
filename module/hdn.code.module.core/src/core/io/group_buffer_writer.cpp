#include "group_buffer_writer.h"

namespace hdn
{
	namespace bin
	{
		void Push(FGroupBufferWriter& writer, const char* semantic)
		{
			writer.Push(semantic);
		}

		void Pop(FGroupBufferWriter& writer)
		{
			writer.Pop();
		}
	}
}
