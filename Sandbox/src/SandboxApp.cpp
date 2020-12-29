#include <Hedron.h> 

#include "imgui.h"

class ExampleLayer : public Hedron::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void on_update() override
	{
		if (Hedron::Input::is_key_pressed(HDR_KEY_E) && Hedron::Input::is_key_pressed(HDR_KEY_E))
			HDR_WARNING("E and A key are being pressed");
	}

	virtual void on_imgui_render() override
	{
		ImGui::Begin("TEST");
		ImGui::Text("HELLO WORLD");
		ImGui::Text("HELLO WORLD 2");
		ImGui::Button("BUTTON");
		ImGui::End();
	}

	void on_event(Hedron::Event& event) override
	{
		HDR_TRACE("{0}", event);
	}
};

class Sandbox : public Hedron::Application
{
public:
	Sandbox()
	{
		push_layer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Hedron::Application* Hedron::create_application()
{
	return new Sandbox();
}