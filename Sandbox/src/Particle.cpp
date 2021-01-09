#include "Particle.h"

void Particle::update(Hedron::Timestep ts)
{
	lifeRemaining -= ts;
	position += velocity * (float)ts;
	rotation = 0.01f * ts;
}

void Particle::render()
{
	float life = lifeRemaining / lifeTime;
	float interpolatedSize = glm::lerp(sizeEnd, sizeBegin, life);
	glm::vec4 color = glm::lerp(colorEnd, colorBegin, life);
	color.a *= life;

	Hedron::Renderer2D::fill_rect(position, { interpolatedSize, interpolatedSize }, color);
}
