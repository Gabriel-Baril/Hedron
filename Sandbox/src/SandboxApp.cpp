#include <Hedron.h>

class Sandbox : public Hedron::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Hedron::Application* Hedron::CreateApplication()
{
	return new Sandbox();
}