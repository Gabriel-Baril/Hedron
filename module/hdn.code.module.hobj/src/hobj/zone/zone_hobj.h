
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

		virtual void Serialize(hostream& stream) override;
        virtual void Deserialize(histream& stream) override;

        virtual ~HZone();
    private:
        Zone m_Zone;
    };
}