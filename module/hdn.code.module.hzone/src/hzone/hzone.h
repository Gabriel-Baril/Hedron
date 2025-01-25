
#pragma once

#include "core/core.h"
#include "core/hobj/hobj.h"

namespace hdn
{
    class HZone : public HObject
    {
	public:
        HZone() = default;

        void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override;
        void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override;

        virtual ~HZone();
    private:
    };
}