#pragma once

#include "core/core.h"
#include <PxPhysicsAPI.h>

namespace hdn
{
	struct PhysicsComponent
	{
		physx::PxRigidActor* physicsActor;
	};
}
