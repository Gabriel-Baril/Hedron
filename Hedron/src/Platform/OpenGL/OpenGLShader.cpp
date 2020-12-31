#include "hdrpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hedron
{
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source into GL
		const GLchar* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0); // Bind our source string with our vertexShader ID

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0; // Store the compilation status
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled); // Put the compilation status in the variable isCompiled
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0; // Length of the log message (error)
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength); // Get the length of the log message 

			std::vector<GLchar> infoLog(maxLength); // Buffer of our log message (The maxLength includes the NULL character)
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]); // Fill the buffer with the error message

			// We delete the shader to not create memory leaks
			glDeleteShader(vertexShader);

			// Log the errors
			HDR_CORE_ERROR("{0}", infoLog.data());
			HDR_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code into GL
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We delete the shaders to not create memory leaks
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			// Log the errors
			HDR_CORE_ERROR("{0}", infoLog.data());
			HDR_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shader are now successfully compiled
		// We now need to link them into a program
		m_rendererID = glCreateProgram(); // Create a program object

		glAttachShader(m_rendererID, vertexShader); // Bind our program with our vertexShader
		glAttachShader(m_rendererID, fragmentShader); // Bind our program with our vertexShader

		glLinkProgram(m_rendererID); // Link the program and so our shaders together

		GLint isLinked = 0;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

			// Program not successful we need to delete it
			glDeleteProgram(m_rendererID);
			// We delete the shaders to not create memory leaks
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Log the errors
			HDR_CORE_ERROR("{0}", infoLog.data());
			HDR_CORE_ASSERT(false, "Program link failure!");
			return;
		}

		// We need to detach the shader after a successful link
		glDetachShader(m_rendererID, vertexShader);
		glDetachShader(m_rendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID); // Delete the program when we don't need it anymore
	}

	void OpenGLShader::bind() const
	{
		glUseProgram(m_rendererID); // Use the program for the future draw calls
	}

	void OpenGLShader::unbind() const
	{
		glUseProgram(0); // Unbind the program for the future draw calls
	}


	void OpenGLShader::upload_uniform_int(const std::string& uniformName, int values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform1i(location, values);
	}

	void OpenGLShader::upload_uniform_int2(const std::string& uniformName, const glm::vec<2, int>& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform2i(location, values.x, values.y);
	}

	void OpenGLShader::upload_uniform_int3(const std::string& uniformName, const glm::vec<3, int>& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform3i(location, values.x, values.y, values.z);
	}

	void OpenGLShader::upload_uniform_int4(const std::string& uniformName, const glm::vec<4, int>& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform4i(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::upload_uniform_float(const std::string& uniformName, float values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform1f(location, values);
	}

	void OpenGLShader::upload_uniform_float2(const std::string& uniformName, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::upload_uniform_float3(const std::string& uniformName, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::upload_uniform_float4(const std::string& uniformName, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::upload_uniform_mat4(const std::string& uniformName, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void OpenGLShader::upload_uniform_mat3(const std::string& uniformName, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, uniformName.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}