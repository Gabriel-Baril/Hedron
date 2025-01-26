#include "buffer_writer.h"

namespace hdn
{

	namespace bin
	{
		void Write(FBufferWriter& writer, const char& object)
		{
			writer.Write(object);
		}

		void Write(FBufferWriter& writer, const string& object)
		{
			const auto size = object.size();
			writer.Write<size_t>(size + 1);
			writer.Write(object.c_str(), size + 1);
		}
	}
}