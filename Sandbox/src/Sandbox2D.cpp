#include "Sandbox2D.h"
#include "imgui.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

Sandbox2D::Sandbox2D()
	: Hedron::Layer("Sandbox2D layer"), 
	  m_cameraController(1280.f / 720.f, true), 
	  m_backgroundColor(1.0f, 1.0f, 1.0f, 1.0f)
{
}

void Sandbox2D::on_attach()
{
	HDR_PROFILE_FUNCTION();

	m_heartTexture = Hedron::Texture2D::create("assets/textures/heart_pixel_art_64x64.png");
	m_cricketsHead = Hedron::Texture2D::create("assets/textures/collectibles_004_cricketshead.png");
}

void Sandbox2D::on_detach()
{
	HDR_PROFILE_FUNCTION();
}

void Sandbox2D::on_update(Hedron::Timestep ts)
{
	HDR_PROFILE_FUNCTION();
	
	Hedron::Renderer2D::reset_stats();

	//HDR_INFO("Delta time: [{0} sec] [{1} ms] [{2} fps]", ts.get_seconds(), ts.get_milliseconds(), );
	
	m_fps = 1000.0f / ts.get_milliseconds();
	m_quadCount = 0;

	m_cameraController.on_update(ts);

	// Camera position
	//glm::vec3 cam = m_cameraController.get_camera().get_position();
	//HDR_INFO("[{0}, {1}, {2}]", cam.x, cam.y, cam.z);
	
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

	float xBound = 200;
	float yBound = 200;

	for (int x = 0;x < xBound; x++)
	{
		for (int y = 0; y < yBound; y++)
		{
			float xColor = sin(x + m_time);
			float yColor = cos(y + m_time);
			if( (x + y) % 2 == 0)
				Hedron::Renderer2D::fill_rect({ x, y, 0.0f }, { 0.8f, 0.8f }, m_heartTexture, { xColor, yColor, xColor * yColor, 1.0 }, 1.0f);
			else
				Hedron::Renderer2D::fill_rect({ x, y, 0.0f }, { 1.0f, 1.0f }, m_cricketsHead, { xColor, yColor, xColor * yColor, 1.0 }, 1.0f);

			m_quadCount++;
		}
	}
	Hedron::Renderer2D::fill_rect({ 0.5f, 0.3f, 0.0f }, { 0.8f, 0.8f }, glm::radians(m_time), { 0.2f, 0.8f, 0.3f, 1.0 });
	//Hedron::Renderer2D::fill_rect({ m_squarePosition.x, m_squarePosition.y, 0.0f }, { 0.4f, 0.8f }, m_heartTexture, {0.0f, 1.0f, 0.3f, 1.0f}, 1);
	//Hedron::Renderer2D::fill_rect({ m_squarePosition.x + 0.5f, m_squarePosition.y + 0.5f, -0.1f }, { 1.0f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });

	Hedron::Renderer2D::end_scene();

	m_time += 0.001;
	m_frame++;
	m_rotation += m_rotationSpeed;
}

void Sandbox2D::on_imgui_render()
{
	HDR_PROFILE_FUNCTION();

	auto stat = Hedron::Renderer2D::get_stats();

	ImGui::Begin("Background color control panel");
	ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
	ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
	ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);
	ImGui::Text("FPS : %.2f", m_fps);
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("	Draw Call(s) : %i", stat.drawCalls);
	ImGui::Text("	Quad Count : %i", stat.quadCount);
	ImGui::Text("	Vertex Count : %i", stat.get_total_vertex_count());
	ImGui::Text("	Index Count : %i", stat.get_total_index_count());
	ImGui::Text("	Triangle Count : %i", stat.get_total_triangle_count());
	ImGui::End();
}

void Sandbox2D::on_event(Hedron::Event& event)
{
	HDR_PROFILE_FUNCTION();
	m_cameraController.on_event(event);
}
