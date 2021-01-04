#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_name(name), m_stopped(false), m_func(std::move(func))
	{
		m_startPointTime = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_stopped)
			this->stop();
	}

	void stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_startPointTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();

		m_stopped = true;

		float duration = (end - start) * 0.000001f;

		m_func({ m_name, duration});
	}
private:
	const char* m_name;
	std::chrono::time_point<std::chrono::steady_clock> m_startPointTime;
	bool m_stopped;
	Fn m_func;
};


Sandbox2D::Sandbox2D()
	: Hedron::Layer("Sandbox2D layer"), 
	  m_cameraController(1280.f / 720.f, true), 
	  m_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Sandbox2D::on_attach()
{
	m_heartTexture = Hedron::Texture2D::create("assets/textures/heart_pixel_art_254x254.png");
}

void Sandbox2D::on_detach()
{

}

#define W(x) x
#define CONCAT(x, y) W(x)W(y)
#define PROFILE_SCOPE(name) Timer timer(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); });
void Sandbox2D::on_update(Hedron::Timestep ts)
{
	//HDR_INFO("Delta time: [{0} sec] [{1} ms] [{2} fps]", ts.get_seconds(), ts.get_milliseconds(), 1000.0f / ts.get_milliseconds());
	PROFILE_SCOPE("Sandbox2D::on_update");

	{
		PROFILE_SCOPE("CameraController::on_update");
		m_cameraController.on_update(ts);
	}
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_I))
		m_squarePosition.y += m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_K))
		m_squarePosition.y -= m_squareSpeed * ts;
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_J))
		m_squarePosition.x -= m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_L))
		m_squarePosition.x += m_squareSpeed * ts;

	{
		PROFILE_SCOPE("Sandbox2D::on_update::Rendering");
		Hedron::RenderCommand::set_clear_color(m_backgroundColor);
		Hedron::RenderCommand::clear();

		Hedron::Renderer2D::begin_scene(m_cameraController.get_camera());

		Hedron::Renderer2D::fill_rect({ 0.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, 0, { 0.2f, 0.4f, 0.8f, 1.0f });

		Hedron::Renderer2D::fill_rect({ m_squarePosition.x, m_squarePosition.y, 0.0f }, { 1.0f, 1.0f }, m_heartTexture);
		Hedron::Renderer2D::fill_rect({ m_squarePosition.x + 0.5f, m_squarePosition.y + 0.5f, 0.0f }, { 1.0f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });

		Hedron::Renderer2D::end_scene();
	}

	m_rotation += m_rotationSpeed;
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("Background color control panel");
	//ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
	//ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
	//ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);

	for (auto& result : m_profileResults)
	{
		char label[50];
		strcpy_s(label, "%.4f ms : ");
		strcat_s(label, result.name);
		ImGui::Text(label, result.time);
	}
	m_profileResults.clear();

	ImGui::End();


}

void Sandbox2D::on_event(Hedron::Event& event)
{
	m_cameraController.on_event(event);
}
