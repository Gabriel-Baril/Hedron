#pragma once

#include <string>

namespace Hedron
{
	class Shader 
	{
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static Ref<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}