
#pragma once

#include "core/core.h"
#include "core/hobj/hobj.h"
#include "zone.h"

namespace hdn
{
    class HZone : public HObject
    {
	public:
        HZone() = default;

        void Deserialize(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override;
        void Serialize(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override;

        virtual ~HZone();
    private:
		Zone m_Zone;
    };
}