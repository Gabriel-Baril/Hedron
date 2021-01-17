#include "hdrpch.h"
#include "Hedron/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Hedron/Core/Application.h"


namespace Hedron
{
	std::string FileDialogs::open_file(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR szFile[260] = { 0 };
		// Initialize OPENFILENAME
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().get_window().get_native_window());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::save_file(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR szFile[260] = { 0 };
		// Initialize OPENFILENAME
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().get_window().get_native_window());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}
		return std::string();
	}
}