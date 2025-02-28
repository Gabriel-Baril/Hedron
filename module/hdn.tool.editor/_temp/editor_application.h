#pragma once

#include "../application/application.h"
#include "editor_layer.h"

namespace hdn
{
	class EditorApplication : public IApplication
	{
	public:
		EditorApplication()
		{
			auto edLayer = Register<EditorLayer>();
		}
	};

}