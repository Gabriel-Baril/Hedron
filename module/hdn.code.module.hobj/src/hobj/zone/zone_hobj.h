
#pragma once

#include "core/core.h"
#include "hobj/hobj.h"
#include "zone.h"

namespace hdn
{
    class HZone : public HObject
    {
	public:
        HZone() = default;

		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override;
        virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override;

        virtual ~HZone();
    private:
        Zone m_Zone;
    };
}