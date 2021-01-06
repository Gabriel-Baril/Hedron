#pragma once
#include <Hedron.h>

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

	// TEMP
	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };
	float m_squareSpeed = 4.0f;

	float m_rotation = 0;
	float m_rotationSpeed = 2;

	Hedron::Ref<Hedron::Shader> m_shader;
	Hedron::Ref<Hedron::Texture2D> m_texture;
	Hedron::Ref<Hedron::VertexArray> m_vertexArray;
	Hedron::Ref<Hedron::Texture2D> m_heartTexture;
	glm::vec4 m_backgroundColor;

	Hedron::OrthographicCameraController m_cameraController;
};
