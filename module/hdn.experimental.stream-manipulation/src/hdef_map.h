#pragma once

#include "stream_reader.h"
#include "stream_writer.h"
#include "entity.h"
#include "hobject.h"

namespace hdn
{
	class HDefMap : public HObject
	{
	public:
		friend StreamReader &operator>>( StreamReader &stream, HDefMap &object );
		friend StreamWriter &operator<<( StreamWriter &stream, const HDefMap &object );

		virtual void Serialize(StreamWriter& stream) const override
		{
			HObject::Serialize(stream);
			stream << m_Version;
			stream << m_Entities.size();
			for (const auto& entity : m_Entities) {
				stream << entity;
			}
		}

		virtual void Deserialize(StreamReader& stream) override
		{
			HObject::Deserialize(stream);
			stream >> m_Version;
			size_t entityCount = 0;
			stream >> entityCount;
			m_Entities.reserve(entityCount);
			for (int i = 0; i < entityCount; i++) {
				HEntity entity;
				stream >> entity;
				m_Entities.emplace_back(entity);
			}
		}

		void SetVersion( int version ) {
			m_Version = version;
		}

		void AddEntity( int numComponent ) {
			HEntity ent;
			ent.m_NumComponent = numComponent;
			m_Entities.emplace_back( ent );
		}

	private:
		int m_Version = 0;
		std::vector<HEntity> m_Entities;
	};
}