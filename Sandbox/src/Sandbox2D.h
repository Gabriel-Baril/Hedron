#pragma once
#include <Hedron.h>

#include "Random.h"
#include "ParticleSystem.h"

class Sandbox2D : public Hedron::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(Hedron::Timestep ts) override;
	virtual void on_imgui_render() override;
	virtual void on_event(Hedron::Event& event) override;
private:
	float m_time = 0;
	uint32_t m_frame = 0;
	float m_fps;
	ParticleSystem m_particleSystem;
	ParticleProps m_particleProps;

	Hedron::Ref<Hedron::Texture2D> m_achivementsTexture;
	Hedron::Ref<Hedron::Texture2D> m_heartTexture;
	Hedron::Ref<Hedron::SubTexture2D> m_frameTexture;
	glm::vec4 m_backgroundColor;

	Hedron::OrthographicCameraController m_cameraController;
};
