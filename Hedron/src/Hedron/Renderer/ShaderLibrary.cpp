#include "hdrpch.h"
#include "ShaderLibrary.h"

namespace Hedron
{

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
	{
		HDR_CORE_ASSERT(!exists(name), "Shader already exists !");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->get_name();
		this->add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath)
	{
		Ref<Shader> shader = Shader::create(filePath);
		this->add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filePath)
	{
		Ref<Shader> shader = Shader::create(filePath);
		this->add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		HDR_CORE_ASSERT(exists(name), "Shader not found !");
		return m_shaders[name];
	}
	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}
}

