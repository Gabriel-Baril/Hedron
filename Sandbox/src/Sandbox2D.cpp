#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

static const char* s_mapTiles = 
"TTTTTTTTTTTTTTTTTTTT"
"TUUUUU              "
"TRRRRR RRUUUURRRRRRT"
"T      RRRRURRRRRRRT"
"T UUUUUUUUUUUUURRRRT"
"T RRRRRRRRRRRRURRRRT"
"             RURRRRT"
"TTTTTTTTTTTTTTTTTTTT";

static const glm::vec<2, uint32_t> s_mapSize = {20, 8};

Sandbox2D::Sandbox2D()
	: Hedron::Layer("Sandbox2D layer"), 
	  m_cameraController(1280.f / 720.f, true), 
	  m_backgroundColor(0.2f, 0.2f, 0.2f, 1.0f)
{
	m_particleProps.colorBegin = { 0.4f, 0.7f, 0.2f, 1.0f };
	m_particleProps.colorEnd = { 0.9f, 0.1f, 0.6f, 1.0f };
	m_particleProps.sizeBegin = 0.1f;
	m_particleProps.sizeVariation = 0.3f;
	m_particleProps.sizeEnd = 0.0f;
	m_particleProps.lifeTime = 1.0f;
	m_particleProps.velocity = { 0.0f, 0.0f };
	m_particleProps.velocityVariation = { 3.0f, 1.0f };
	m_particleProps.position = { 0.0f, 0.0f };

}

void Sandbox2D::on_attach()
{
	HDR_PROFILE_FUNCTION();

	m_heartTexture = Hedron::Texture2D::create("assets/textures/heart_pixel_art_64x64.png");
	m_achivementsTexture = Hedron::Texture2D::create("assets/game/textures/isaac_achievements_sprite_sheet.png");
	
	m_tilesTexture = Hedron::Texture2D::create("assets/game/textures/grid/rocks_basement.png");
	m_tiles['R'] = Hedron::SubTexture2D::create_from_coords(m_tilesTexture, { 0.0f, 7.0f }, { 32.0f, 32.0f }, { 1.0f, 1.0f });
	m_tiles['T'] = Hedron::SubTexture2D::create_from_coords(m_tilesTexture, { 1.0f, 7.0f }, { 32.0f, 32.0f }, { 1.0f, 1.0f });
	m_tiles['U'] = Hedron::SubTexture2D::create_from_coords(m_tilesTexture, { 1.0f, 6.0f }, { 32.0f, 32.0f }, { 1.0f, 1.0f });
	m_tiles['Y'] = Hedron::SubTexture2D::create_from_coords(m_tilesTexture, { 0.0f, 6.0f }, { 32.0f, 32.0f }, { 1.0f, 1.0f });
	
	m_frameTexture = Hedron::SubTexture2D::create_from_coords(m_achivementsTexture, { 1.0f, 65.0f }, {64.0f, 64.0f});

	Hedron::FrameBufferSpecification frameBufferSpec;
	frameBufferSpec.width = Hedron::Application::get().get_window().get_width();
	frameBufferSpec.height = Hedron::Application::get().get_window().get_height();
}

void Sandbox2D::on_detach()
{
	HDR_PROFILE_FUNCTION();
}

void Sandbox2D::on_update(Hedron::Timestep ts)
{
	HDR_PROFILE_FUNCTION();
	m_fps = 1000.0f / ts.get_milliseconds();
	
	Hedron::Renderer2D::reset_stats();
	m_cameraController.on_update(ts);
	HDR_INFO("Delta time: [{0} sec] [{1} ms] [{2} fps]", ts.get_seconds(), ts.get_milliseconds(), 1000 / ts.get_milliseconds());

	// Camera position
	//HDR_INFO("[{0}, {1}, {2}]", cam.x, cam.y, cam.z);
	
	Hedron::RenderCommand::set_clear_color(m_backgroundColor);
	Hedron::RenderCommand::clear();


	Hedron::Renderer2D::begin_scene(m_cameraController.get_camera());

	if (Hedron::Input::is_mouse_button_pressed(Hedron::MouseButton::MOUSE_BUTTON_1))
	{
		glm::vec2 cam_pos = m_cameraController.get_camera().get_position();

		uint32_t width = Hedron::Application::get().get_window().get_width();
		uint32_t height = Hedron::Application::get().get_window().get_height();
		auto [x, y] = Hedron::Input::get_mouse_position();
		float cam_width = m_cameraController.get_bounds().get_width();
		float cam_height = m_cameraController.get_bounds().get_height();

		x = x * (cam_width / width) - cam_width * 0.5f;
		y = -y * (cam_height / height) + cam_height * 0.5f;

		//HDR_INFO("[{0}, {1}]", x, y);
		m_particleProps.position = { x + cam_pos.x, y + cam_pos.y };
		for (uint32_t i = 0;i < 5;i++)
			m_particleSystem.emit(m_particleProps);
	}

	m_particleSystem.render();
	m_particleSystem.update(ts);

	for (uint32_t i = 0;i < s_mapSize.x * s_mapSize.y; i++)
	{
		uint32_t x = i % s_mapSize.x;
		uint32_t y = i / s_mapSize.x;

		if(m_tiles.find(s_mapTiles[i]) != m_tiles.end())
			Hedron::Renderer2D::fill_rect({ x, y, 0 }, { 1.0f, 1.0f }, m_tiles[s_mapTiles[i]], { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
	}


	Hedron::Renderer2D::end_scene();

	m_time += 0.001f;
	m_frame++;
}

void Sandbox2D::on_imgui_render()
{
	HDR_PROFILE_FUNCTION();
	auto stat = Hedron::Renderer2D::get_stats();
	auto cam = m_cameraController.get_camera();
	auto cam_pos = cam.get_position();

	ImGui::Begin("Background color control panel");

	ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
	ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
	ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);
	ImGui::ColorEdit4("Starting Color", glm::value_ptr(m_particleProps.colorBegin));
	ImGui::ColorEdit4("Finishing Color", glm::value_ptr(m_particleProps.colorEnd));
	ImGui::DragFloat("Life Span", &m_particleProps.lifeTime, 0.1f, 0.0f, 10.0f);
	ImGui::Text("FPS : %.2f", m_fps);
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("	Draw Call(s) : %i", stat.drawCalls);
	ImGui::Text("	Quad Count : %i", stat.quadCount);
	ImGui::Text("	Vertex Count : %i", stat.get_total_vertex_count());
	ImGui::Text("	Index Count : %i", stat.get_total_index_count());
	ImGui::Text("	Triangle Count : %i", stat.get_total_triangle_count());
	ImGui::Text("Camera Data:");
	ImGui::Text("	Position : [%.2f, %.2f, %.2f]", cam_pos.x, cam_pos.y, cam_pos.z);
	ImGui::Text("	Aspect ratio : %.2f", m_cameraController.get_aspect_ratio());
	ImGui::Text("	Zoom level : %.2f", m_cameraController.get_zoom_level());
	ImGui::Text("	ZL * AR : %.2f", m_cameraController.get_zoom_level() * m_cameraController.get_aspect_ratio());

	ImGui::End();

}

void Sandbox2D::on_event(Hedron::Event& event)
{
	HDR_PROFILE_FUNCTION();
	m_cameraController.on_event(event);
}
