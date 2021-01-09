#pragma once
#include <Hedron.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"

struct ParticleProps
{
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void update(Hedron::Timestep ts);
	void render();
	void stop();

	void emit(const ParticleProps& props);
private:
	std::vector<Particle> m_particlePool;
	uint32_t m_poolIndex;

	inline static uint32_t maxParticle = 5000;
};