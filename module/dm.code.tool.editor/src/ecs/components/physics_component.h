#pragma once

#include "core/core.h"
#include <PxPhysicsAPI.h>

namespace dm
{
	struct PhysicsComponent
	{
		physx::PxRigidActor *physicsActor;
	};
}
