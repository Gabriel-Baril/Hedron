
#pragma once

#include "HDNDevice.h"
#include "HDNWindow.h"
#include "HDNGameObject.h"
#include "HDNRenderer.h"
#include "HDNDescriptors.h"

#include <memory>
#include <vector>

namespace hdn
{

	class FirstApp
	{
	public:
		static constexpr uint32 WIDTH = 800;
		static constexpr uint32 HEIGHT = 600;
	public:
		FirstApp();
		virtual ~FirstApp();
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void Run();
	private:
		void LoadGameObjects();
	private:
		HDNWindow m_Window{ WIDTH, HEIGHT, "First App"};
		HDNDevice m_Device{ m_Window };
		HDNRenderer m_Renderer{ &m_Window, &m_Device };

		// Order of declarations matters
		Scope<HDNDescriptorPool> globalPool{}; // System that require descriptors only relevant to their work should create their own HDNDescriptorPool

		HDNGameObject::Map m_GameObjects;
	};
}