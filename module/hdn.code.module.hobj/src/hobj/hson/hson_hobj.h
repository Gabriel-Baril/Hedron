#pragma once

#include "hson.h"
#include "hson_build.h"
#include "hobj/hobj.h"

namespace hdn
{
	// Standalone serializable hson
	class HHson : public HObject
	{
	public:
		HHson() = default;
		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override;
		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override;
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HHson>(); }
		virtual ~HHson() = default;

		hson_t& GetHson() { return m_Hson; };
		const hson_t& GetHson() const { return m_Hson; };
	private:
		hson_t m_Hson;
	};
}