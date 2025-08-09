#pragma once
#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/random.h"

namespace hdn
{
	class HObject
	{
	public:
		virtual ~HObject() = default;
		uuid64 id()
		{
			return m_Id;
		}
	private:
		uuid64 m_Id;
	};
}