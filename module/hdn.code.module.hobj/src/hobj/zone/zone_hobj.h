
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

		virtual void serialize(hostream& stream) override;
        virtual void deserialize(histream& stream) override;

		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HZone>(); }

        virtual ~HZone();
    private:
        Zone m_Zone;
    };
}