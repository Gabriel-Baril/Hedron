#pragma once
#include <Hedron.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/compatibility.hpp>


struct Particle
{
	void update(Hedron::Timestep ts);
	void render();
	
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 colorBegin, colorEnd;
	float rotation;
	float sizeBegin, sizeEnd;
	float lifeTime;
	float lifeRemaining;
	bool active;
};