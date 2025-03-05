#pragma once

#include "stream_reader.h"
#include "stream_writer.h"

namespace hdn
{
	class ISerializable
	{
	public:
		virtual void Serialize(StreamWriter& stream) const = 0;
	};

	class IDeserializable
	{
	public:
		virtual void Deserialize(StreamReader& stream) = 0;
	};

	class HObject : public ISerializable, public IDeserializable
	{
	public:
		friend StreamReader &operator>>( StreamReader &stream, HObject &object );
		friend StreamWriter &operator<<( StreamWriter &stream, const HObject &object );

		virtual void Serialize(StreamWriter& stream) const override
		{
			stream << m_Test;
		}

		virtual void Deserialize(StreamReader& stream) override
		{
			stream >> m_Test;
		}

		void SetTest( int test ) { m_Test = test; }
		int m_Test = 0;
	};
}