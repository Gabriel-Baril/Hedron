#include <Hedron.h>
#include <Hedron/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hedron
{
	class Poly : public Application
	{
	public:
		Poly()
			: Application("Poly")
		{
			push_layer(new EditorLayer());
		}

		~Poly()
		{
		}
	};

	Application* create_application()
	{
		return new Poly();
	}
}
