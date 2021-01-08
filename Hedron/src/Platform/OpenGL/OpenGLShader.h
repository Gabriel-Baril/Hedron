#pragma once

#include <unordered_map>

#include "Hedron/Renderer/Shader.h"

// TODO: remove
typedef unsigned int GLenum;

namespace Hedron
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const std::string& get_name() const override { return m_name; }

		virtual void set_int(const std::string& name, int values) override;
		virtual void set_int2(const std::string& name, const glm::vec<2, int>& values) override;
		virtual void set_int3(const std::string& name, const glm::vec<3, int>& values) override;
		virtual void set_int4(const std::string& name, const glm::vec<4, int>& values) override;
		virtual void set_int_array(const std::string& name, int* values, uint32_t count) override;

		virtual void set_float(const std::string& name, float values) override;
		virtual void set_float2(const std::string& name,const glm::vec2& values) override;
		virtual void set_float3(const std::string& name,const glm::vec3& values) override;
		virtual void set_float4(const std::string& name,const glm::vec4& values) override;

		virtual void set_mat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void set_mat4(const std::string& name, const glm::mat4& matrix) override;

	private:
		void upload_uniform_int(const std::string& uniformName, int values);
		void upload_uniform_int2(const std::string& uniformName, const glm::vec<2, int>& values);
		void upload_uniform_int3(const std::string& uniformName, const glm::vec<3, int>& values);
		void upload_uniform_int4(const std::string& uniformName, const glm::vec<4, int>& values);
		void upload_uniform_int_array(const std::string& uniformName, int* values, uint32_t count);

		void upload_uniform_float(const std::string& uniformName, float values);
		void upload_uniform_float2(const std::string& uniformName, const glm::vec2& values);
		void upload_uniform_float3(const std::string& uniformName, const glm::vec3& values);
		void upload_uniform_float4(const std::string& uniformName, const glm::vec4& values);

		void upload_uniform_mat3(const std::string& uniformName, const glm::mat3& matrix);
		void upload_uniform_mat4(const std::string& uniformName, const glm::mat4& matrix);

		std::string read_file(const std::string filePath);
		std::string get_file_name(const std::string& filePath);
		std::unordered_map<GLenum, std::string> pre_process(const std::string& shaderSource);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_rendererID;
		std::string m_name;
	};
}