
#pragma once

#include "core/core.h"
#include "hobj.h"
#include "hobj_zone.h"

namespace hdn
{
    class HZone : public HObject
    {
	public:
        HZone() = default;

		virtual void Serialize(FBufferWriter& archive, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override;
        virtual void Deserialize(FBufferReader& archive, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override;

        virtual ~HZone();
    private:
		Zone m_Zone;
    };
}