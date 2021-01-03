#pragma once

#include <string>
#include <unordered_map>

#include "Hedron/Core.h"
#include "Shader.h"

namespace Hedron
{
	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const Ref<Shader>& shader);
		void add(const Ref<Shader>& shader);
		Ref<Shader> load(const std::string& filePath);
		Ref<Shader> load(const std::string& name, const std::string& filePath);

		Ref<Shader> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}