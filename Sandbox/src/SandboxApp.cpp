#include <Hedron.h>

class ExampleLayer : public Hedron::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void on_update() override
	{
		HDR_INFO("ExampleLayer::Update");
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