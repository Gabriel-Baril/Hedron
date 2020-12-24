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

Hedron::Application* Hedron::create_application()
{
	return new Sandbox();
}