#include <Hedron.h>

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

	void on_event(Hedron::Event& event) override
	{
		//HDR_TRACE("{0}", event);
	}
};

class Sandbox : public Hedron::Application
{
public:
	Sandbox()
	{
		push_layer(new ExampleLayer());
		push_overlay(new Hedron::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Hedron::Application* Hedron::create_application()
{
	return new Sandbox();
}