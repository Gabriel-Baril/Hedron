#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Hedron/Core/Core.h"

namespace Hedron
{
	class Shader 
	{
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& get_name() const = 0;

		virtual void set_int(const std::string& name, int values) = 0;
		virtual void set_int2(const std::string& name, const glm::vec<2, int>& values) = 0;
		virtual void set_int3(const std::string& name, const glm::vec<3, int>& values) = 0;
		virtual void set_int4(const std::string& name, const glm::vec<4, int>& values) = 0;
					 
		virtual void set_float(const std::string& name, float values) = 0;
		virtual void set_float2(const std::string& name, const glm::vec2& values) = 0;
		virtual void set_float3(const std::string& name, const glm::vec3& values) = 0;
		virtual void set_float4(const std::string& name, const glm::vec4& values) = 0;
					 
		virtual void set_mat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void set_mat4(const std::string& name, const glm::mat4& matrix) = 0;

		static Ref<Shader> create(const std::string& filePath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};
}