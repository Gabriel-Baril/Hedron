#pragma once

#include <string>\

namespace Hedron
{
	class Shader 
	{
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& get_name() const = 0;

		static Ref<Shader> create(const std::string& filePath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};
}