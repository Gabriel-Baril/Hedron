
#pragma once

#include "core/core.h"
#include "hobj.h"
#include "hobj_zone_api.h"

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
        hobj_zone m_Zone;
    };
}