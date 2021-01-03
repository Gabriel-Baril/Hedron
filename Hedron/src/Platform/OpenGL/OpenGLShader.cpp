#include "hdrpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hedron
{
	static GLenum shader_type_from_string(const std::string& shaderTypeStr)
	{
		if (shaderTypeStr == "vertex") 
			return GL_VERTEX_SHADER;
		if (shaderTypeStr == "fragment" || shaderTypeStr == "pixel") 
			return GL_FRAGMENT_SHADER;

		HDR_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	static std::string string_from_shader_type(GLenum shaderTypeStr)
	{
		if (shaderTypeStr == GL_VERTEX_SHADER)
			return "vertex";
		if (shaderTypeStr == GL_FRAGMENT_SHADER)
			return "fragment";

		HDR_CORE_ASSERT(false, "Unknown shader type");
		return "unknown";
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
		: m_name(get_file_name(filePath))
	{
		std::string shaderSource = read_file(filePath);
		auto shaderSources = pre_process(shaderSource);

		this->compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;

		this->compile(shaderSources);
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

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram(); // Create a program object
		
		HDR_CORE_ASSERT(shaderSources.size() <= 2, "You need to have less than 2 shader in the rendering pipeline !")
		std::array<GLenum, 2> glShaderIds;
		int glShaderIdIndex = 0;

		for (auto&& [shaderType, shaderSource] : shaderSources)
		{
			GLuint shader = glCreateShader(shaderType);

			// Send the shader source into GL
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0); // Bind our source string with our shader ID

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0; // Store the compilation status
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled); // Put the compilation status in the variable isCompiled
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0; // Length of the log message (error)
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength); // Get the length of the log message 

				std::vector<GLchar> infoLog(maxLength); // Buffer of our log message (The maxLength includes the NULL character)
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]); // Fill the buffer with the error message

				// We delete the shader to not create memory leaks
				glDeleteShader(shader);

				// Log the errors
				HDR_CORE_ERROR("{0}", infoLog.data());
				HDR_CORE_ASSERT(false, string_from_shader_type(shaderType) + " shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);

			glShaderIds[glShaderIdIndex] = shader;
			glShaderIdIndex++;
		}


		// Shaders are now successfully compiled
		// We now need to link them into a program
		glLinkProgram(program); // Link the program and so our shaders together

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Program not successful we need to delete it
			glDeleteProgram(program);
			// We delete the shaders to not create memory leaks

			for (auto id : glShaderIds)
				glDeleteShader(id);

			// Log the errors
			HDR_CORE_ERROR("{0}", infoLog.data());
			HDR_CORE_ASSERT(false, "Program link failure!");
			return;
		}

		// We need to detach the shaders after a successful link
		for (auto id : glShaderIds)
			glDetachShader(program, id);

		m_rendererID = program;
	}

	std::string OpenGLShader::read_file(const std::string filePath)
	{
		std::string result;
		std::ifstream file(filePath, std::ios::in | std::ios::binary);
		if (file)
		{
			file.seekg(0, std::ios::end); // Put the file cursor at the end
			result.resize(file.tellg()); // Resize the string with the size of the file
			file.seekg(0, std::ios::beg); // Put the file cursor at the start
			file.read(&result[0], result.size()); // Read the file and put the data at the start of the string result
			file.close();
		}
		else
		{
			HDR_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	std::string OpenGLShader::get_file_name(const std::string& filePath)
	{
		size_t lastSlashPos = filePath.find_last_of("/\\");
		lastSlashPos = (lastSlashPos == std::string::npos) ? 0 : lastSlashPos + 1;
		size_t lastDotPos = filePath.rfind(".");
		lastDotPos = (lastDotPos == std::string::npos) ? filePath.size() : lastDotPos;

		return filePath.substr(lastSlashPos, lastDotPos - lastSlashPos);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::pre_process(const std::string& shaderSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type"; // The token that we are searching
		size_t typeTokenLength = strlen(typeToken); // The size of the token that we are searching
		size_t pos = shaderSource.find(typeToken, 0); // Return the position of the first token
		while (pos != std::string::npos)
		{
			size_t endOfLine = shaderSource.find_first_of("\r\n", pos); // Find the first end of line (eol)
			HDR_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error"); 
			size_t begin = pos + typeTokenLength + 1; // Return the position of our shader type
			std::string type = shaderSource.substr(begin, endOfLine - begin); // Contains our shader type by sub stringing the string
			HDR_CORE_ASSERT(shader_type_from_string(type) != 0, "Invalid shader type specified");
			
			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", endOfLine);
			pos = shaderSource.find(typeToken, nextLinePos);
			shaderSources[shader_type_from_string(type)] = shaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}
}