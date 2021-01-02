#pragma once

#include <glm/glm.hpp>
#include "Hedron/Renderer/Shader.h"

// TODO: remove
typedef unsigned int GLenum;

namespace Hedron
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void upload_uniform_int(const std::string& uniformName, int values);
		virtual void upload_uniform_int2(const std::string& uniformName, const glm::vec<2, int>& values);
		virtual void upload_uniform_int3(const std::string& uniformName, const glm::vec<3, int>& values);
		virtual void upload_uniform_int4(const std::string& uniformName, const glm::vec<4, int>& values);

		virtual void upload_uniform_float(const std::string& uniformName, float values);
		virtual void upload_uniform_float2(const std::string& uniformName, const glm::vec2& values);
		virtual void upload_uniform_float3(const std::string& uniformName, const glm::vec3& values);
		virtual void upload_uniform_float4(const std::string& uniformName, const glm::vec4& values);

		virtual void upload_uniform_mat3(const std::string& uniformName, const glm::mat3& matrix);
		virtual void upload_uniform_mat4(const std::string& uniformName, const glm::mat4& matrix);
	private:
		std::string read_file(const std::string filePath);
		std::unordered_map<GLenum, std::string> pre_process(const std::string& shaderSource);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_rendererID;
	};
}