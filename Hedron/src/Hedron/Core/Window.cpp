#include "hdrpch.h"
#include "Hedron/Core/Window.h"

#ifdef HDR_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif


namespace Hedron
{
	Scope<Window> Window::create(const WindowProps& props)
	{
#ifdef HDR_PLATFORM_WINDOWS
		return create_scope<WindowsWindow>(props);
#else
		HDR_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}