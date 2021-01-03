#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Hedron::Layer("Sandbox2D layer"), 
	  m_cameraController(1280.f / 720.f, true), 
	  m_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Sandbox2D::on_attach()
{
}

void Sandbox2D::on_detach()
{

}

void Sandbox2D::on_update(Hedron::Timestep ts)
{
	m_cameraController.on_update(ts);
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_I))
		m_squarePosition.y += m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_K))
		m_squarePosition.y -= m_squareSpeed * ts;
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_J))
		m_squarePosition.x -= m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_L))
		m_squarePosition.x += m_squareSpeed * ts;

	Hedron::RenderCommand::set_clear_color(m_backgroundColor);
	Hedron::RenderCommand::clear();

	Hedron::Renderer2D::begin_scene(m_cameraController.get_camera());
	
	Hedron::Renderer2D::fill_rect({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 0.8f, 0.5f, 1.0f });
	Hedron::Renderer2D::fill_rect(m_squarePosition, { 1.0f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hedron::Renderer2D::fill_rect({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.5f }, m_rotation, { 0.2f, 0.4f, 0.8f, 1.0f });

	Hedron::Renderer2D::end_scene();

	m_rotation += m_rotationSpeed;
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("Background color control panel");
	ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
	ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
	ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);
	ImGui::End();
}

void Sandbox2D::on_event(Hedron::Event& event)
{
	m_cameraController.on_event(event);
}
