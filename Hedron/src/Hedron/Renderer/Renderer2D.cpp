#include "hdrpch.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hedron/Renderer/Renderer2D.h"
#include "Hedron/Renderer/Shader.h"
#include "Hedron/Renderer/VertexArray.h"
#include "Hedron/Renderer/VertexBuffer.h"
#include "Hedron/Renderer/IndexBuffer.h"
#include "Hedron/Renderer/RenderCommand.h"



namespace Hedron
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		Ref<VertexArray> circleVertexArray;
	};

	static Renderer2DStorage* s_renderer2DStorage;

	void Renderer2D::init()
	{
		HDR_PROFILE_FUNCTION();
		s_renderer2DStorage = new Renderer2DStorage();

		Renderer2D::build_rect_data();
		//Renderer2D::build_circle_data();
	}

	void Renderer2D::shutdown()
	{
		HDR_PROFILE_FUNCTION();

		delete s_renderer2DStorage;
	}

	void Renderer2D::begin_scene(const OrthographicCamera& camera)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DStorage->textureShader->bind();
		s_renderer2DStorage->textureShader->set_mat4("u_viewProjection", camera.get_view_projection_matrix());
	}

	void Renderer2D::end_scene()
	{
		HDR_PROFILE_FUNCTION();


	}

#pragma region fill_rect_impl

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size)
	{
		Renderer2D::fill_rect({ position.x, position.y, 0.0f }, size, { 1.0f, 1.0f , 1.0f, 1.0f });
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size)
	{
		Renderer2D::fill_rect(position, size, { 1.0f, 1.0f , 1.0f, 1.0f });
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		Renderer2D::fill_rect({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DStorage->textureShader->set_float("u_tilingFactor", 1.0f);
		s_renderer2DStorage->textureShader->set_float4("u_color", color);
		s_renderer2DStorage->whiteTexture->bind();
		s_renderer2DStorage->quadVertexArray->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size, 0 });
		s_renderer2DStorage->textureShader->set_mat4("u_transform", transform);

		RenderCommand::draw_indexed(s_renderer2DStorage->quadVertexArray);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, texture, { 1.0f, 1.0f , 1.0f, 1.0f }, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		fill_rect(position, size, texture, {1.0f, 1.0f , 1.0f, 1.0f }, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, texture, color, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, texture, color, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, texture, color, tilingFactor);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DStorage->textureShader->set_float("u_tilingFactor", tilingFactor);
		s_renderer2DStorage->textureShader->set_float4("u_color", color);
		texture->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size, 0 }); // translate  * scale
		s_renderer2DStorage->textureShader->set_mat4("u_transform", transform);

		s_renderer2DStorage->quadVertexArray->bind();
		RenderCommand::draw_indexed(s_renderer2DStorage->quadVertexArray);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation)
	{
		fill_rect({ position.x, position.y, 0 }, size, radRotation, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation)
	{
		fill_rect(position, size, radRotation, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const glm::vec4& color)
	{
		fill_rect({ position.x, position.y, 0 }, size, radRotation, color);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const glm::vec4& color)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DStorage->textureShader->set_float("u_tilingFactor", 1.0f);
		s_renderer2DStorage->textureShader->set_float4("u_color", color);
		s_renderer2DStorage->whiteTexture->bind();
		s_renderer2DStorage->quadVertexArray->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), radRotation, { 0, 0, 1 }) *
			glm::scale(glm::mat4(1.0f), { size, 0 });
		s_renderer2DStorage->textureShader->set_mat4("u_transform", transform);

		RenderCommand::draw_indexed(s_renderer2DStorage->quadVertexArray);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, radRotation, texture, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture)
	{
		fill_rect(position, size, radRotation, texture, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		fill_rect({ position.x, position.y, 0.0f }, size, radRotation, texture, color, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		fill_rect(position, size, radRotation, texture, color, 1.0f);
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		fill_rect({position.x, position.y, 0.0f}, size, radRotation, texture, color, tilingFactor);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DStorage->textureShader->set_float("u_tilingFactor", tilingFactor);
		s_renderer2DStorage->textureShader->set_float4("u_color", color);
		texture->bind();
		s_renderer2DStorage->quadVertexArray->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { position.x, position.y, 0 }) *
			glm::rotate(glm::mat4(1.0f), radRotation, { 0, 0, 1 }) *
			glm::scale(glm::mat4(1.0f), { size, 0 }); // translate * rotate * scale
		s_renderer2DStorage->textureShader->set_mat4("u_transform", transform);

		RenderCommand::draw_indexed(s_renderer2DStorage->quadVertexArray);
	}

#pragma endregion

	void Renderer2D::build_rect_data()
	{
		///------------------------------------------------------------
		s_renderer2DStorage->quadVertexArray = VertexArray::create();

		float squareVertices[] =
		{
			 -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
			  0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
			  0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			 -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::create(squareVertices, sizeof(squareVertices));

		quadVertexBuffer->set_layout({
			{ Hedron::ShaderDataType::FLOAT3, "a_position" },
			{ Hedron::ShaderDataType::FLOAT2, "a_texCoord" }
			});

		s_renderer2DStorage->quadVertexArray->add_vertex_buffer(quadVertexBuffer);
		///------------------------------------------------------------

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

		s_renderer2DStorage->quadVertexArray->set_index_buffer(indexBuffer);

		s_renderer2DStorage->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xFFFFFFFF;
		s_renderer2DStorage->whiteTexture->set_data(&whiteTextureData, sizeof(uint32_t));

		s_renderer2DStorage->textureShader = Shader::create("assets/shaders/texture.shader");
		s_renderer2DStorage->textureShader->bind();
		s_renderer2DStorage->textureShader->set_int("u_texture", 0);
	}

	void Renderer2D::build_circle_data()
	{
	}

	// https://github.com/SonarSystems/OpenGL-Tutorials/blob/master/Drawing%20A%20Hollow%20Circle/main.cpp
	void Renderer2D::draw_circle(const glm::vec2& position, float radius, uint32_t sideCount, const glm::vec4 color)
	{
		std::vector<float> vertices;
		vertices.reserve((360 / sideCount) * 3);

		for (int theta = 0; theta < 360; theta += 360 / sideCount)
		{
			vertices.push_back(radius * cos(theta)); // x component
			vertices.push_back(radius * sin(theta)); // y component
			vertices.push_back(0); // z component
		}

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertices[0], vertices.size() * sizeof(float));

		vertexBuffer->set_layout({
			{ShaderDataType::FLOAT3, "a_position"}
		});

		vertexBuffer->bind();
		//RenderCommand::draw_line_loop(0, vertexBuffer->);
	}
}