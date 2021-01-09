#include "ParticleSystem.h"

#include "Random.h"

ParticleSystem::ParticleSystem()
	: m_poolIndex(ParticleSystem::maxParticle - 1)
{
	m_particlePool.resize(ParticleSystem::maxParticle);
}

void ParticleSystem::update(Hedron::Timestep ts)
{
	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.update(ts);
	}
}

void ParticleSystem::render()
{
	for (auto& particle : m_particlePool)
	{
		if (particle.active)
			particle.render();
	}
}

void ParticleSystem::stop()
{
}

void ParticleSystem::emit(const ParticleProps& props)
{
	Particle& particle = m_particlePool[m_poolIndex];

	particle.active = true;
	particle.position = props.position;
	particle.rotation = Random::next_double() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = props.velocity;
	particle.velocity.x = props.velocityVariation.x * (Random::next_double() - 0.5f);
	particle.velocity.y = props.velocityVariation.y * (Random::next_double() - 0.5f);

	// Color
	particle.colorBegin = props.colorBegin;
	particle.colorEnd = props.colorEnd;
	
	// Time span
	particle.lifeRemaining = props.lifeTime;
	particle.lifeTime = props.lifeTime;

	// Size
	particle.sizeBegin = props.sizeBegin + props.sizeVariation * (Random::next_double() - 0.5f);
	particle.sizeEnd = props.sizeEnd;

	m_poolIndex = --m_poolIndex % m_particlePool.size();
}
