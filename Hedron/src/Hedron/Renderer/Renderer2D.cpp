#include "hdrpch.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hedron/Renderer/Renderer2D.h"
#include "Hedron/Renderer/OrthographicCamera.h"
#include "Hedron/Renderer/Shader.h"
#include "Hedron/Renderer/VertexArray.h"
#include "Hedron/Renderer/VertexBuffer.h"
#include "Hedron/Renderer/IndexBuffer.h"
#include "Hedron/Renderer/RenderCommand.h"



namespace Hedron
{

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 10000; // Max quads per draw calls
		static const uint32_t maxVertices = maxQuads * 4; // Max vertices per draw calls
		static const uint32_t maxIndices = maxQuads * 6; // Max quads per draw calls
		static const uint32_t maxTextureSlot = 32; // TODO: Render Capabilities

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlot> textureSlots;
		uint32_t textureSlotIndex = 1; // SLOT 0 = white texture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_renderer2DData;


	void Renderer2D::init()
	{
		HDR_PROFILE_FUNCTION();
		
		Renderer2D::build_rect_data();
		//Renderer2D::build_circle_data();

	}


	void Renderer2D::build_rect_data()
	{
		s_renderer2DData.quadVertexArray = VertexArray::create();

		s_renderer2DData.quadVertexBuffer = VertexBuffer::create(Renderer2DData::maxVertices * sizeof(QuadVertex));
		// TODO: Construct a layout from a vertex struct
		s_renderer2DData.quadVertexBuffer->set_layout({
			{ Hedron::ShaderDataType::FLOAT3, "a_position" },
			{ Hedron::ShaderDataType::FLOAT4, "a_color" },
			{ Hedron::ShaderDataType::FLOAT2, "a_texCoord" },
			{ Hedron::ShaderDataType::FLOAT, "a_texIndex" },
			{ Hedron::ShaderDataType::FLOAT, "a_tilingFactor" }
			});
		s_renderer2DData.quadVertexArray->add_vertex_buffer(s_renderer2DData.quadVertexBuffer);

		s_renderer2DData.quadVertexBufferBase = new QuadVertex[Renderer2DData::maxVertices];

		// Unsafe in a multi-threaded environment
		uint32_t* quadIndices = new uint32_t[Renderer2DData::maxIndices]; // To be safe we heap allocate to prevent a memory leak ( 0, 1, 2, 2, 3, 0)

		uint32_t offset = 0;
		for (uint32_t i = 0;i < Renderer2DData::maxIndices; i += 6)
		{
			quadIndices[0 + i] = offset + 0;
			quadIndices[1 + i] = offset + 1;
			quadIndices[2 + i] = offset + 2;
						
			quadIndices[3 + i] = offset + 2;
			quadIndices[4 + i] = offset + 3;
			quadIndices[5 + i] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::create(quadIndices, Renderer2DData::maxIndices);
		s_renderer2DData.quadVertexArray->set_index_buffer(quadIndexBuffer);
		delete[] quadIndices;
		
		// Create a 1x1 white texture
		s_renderer2DData.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xFFFFFFFF;
		s_renderer2DData.whiteTexture->set_data(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[Renderer2DData::maxTextureSlot];
		for (int32_t i = 0; i < Renderer2DData::maxTextureSlot; i++)
			samplers[i] = i;

		// Setup texture shader
		s_renderer2DData.textureShader = Shader::create("assets/shaders/texture.shader");

		s_renderer2DData.textureShader->bind();
		s_renderer2DData.textureShader->set_int_array("u_textures", samplers, Renderer2DData::maxTextureSlot);

		s_renderer2DData.textureSlots[0] = s_renderer2DData.whiteTexture;

		s_renderer2DData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_renderer2DData.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_renderer2DData.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_renderer2DData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		HDR_PROFILE_FUNCTION();

		delete[] s_renderer2DData.quadVertexBufferBase;
	}

	void Renderer2D::begin_scene(const Camera& camera, const glm::mat4& transform)
	{
		HDR_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.get_projection() * glm::inverse(transform);

		s_renderer2DData.textureShader->bind();
		s_renderer2DData.textureShader->set_mat4("u_viewProjection", viewProj);

		s_renderer2DData.quadIndexCount = 0;
		s_renderer2DData.quadVertexBufferPtr = s_renderer2DData.quadVertexBufferBase;
		s_renderer2DData.textureSlotIndex = 1;
	}

	void Renderer2D::begin_scene(const OrthographicCamera& camera)
	{
		HDR_PROFILE_FUNCTION();

		s_renderer2DData.textureShader->bind();
		s_renderer2DData.textureShader->set_mat4("u_viewProjection", camera.get_view_projection_matrix());

		s_renderer2DData.quadIndexCount = 0;
		s_renderer2DData.quadVertexBufferPtr = s_renderer2DData.quadVertexBufferBase;
		s_renderer2DData.textureSlotIndex = 1;
	}

	void Renderer2D::end_scene()
	{
		HDR_PROFILE_FUNCTION();

		uint32_t quadVertexBufferSize = (uint8_t*)s_renderer2DData.quadVertexBufferPtr - (uint8_t*)s_renderer2DData.quadVertexBufferBase;
		s_renderer2DData.quadVertexBuffer->set_data(s_renderer2DData.quadVertexBufferBase, quadVertexBufferSize);

		Renderer2D::flush();
	}

	void Renderer2D::flush()
	{
		HDR_PROFILE_FUNCTION();
		// Bind textures
		for (uint32_t i = 0;i < s_renderer2DData.textureSlotIndex;i++)
			s_renderer2DData.textureSlots[i]->bind(i);

		// TODO: Change the signature of the draw_indexed function to prevent those kind of if statement
		if (s_renderer2DData.quadIndexCount > 0)
			RenderCommand::draw_indexed(s_renderer2DData.quadVertexArray, s_renderer2DData.quadIndexCount);

		s_renderer2DData.stats.drawCalls++;
	}

	void Renderer2D::start_new_batch()
	{
		Renderer2D::end_scene(); // TODO: Rename that function

		// TODO: Create a reset_flush_data function
		s_renderer2DData.quadIndexCount = 0;
		s_renderer2DData.quadVertexBufferPtr = s_renderer2DData.quadVertexBufferBase;
		s_renderer2DData.textureSlotIndex = 1;
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
		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices)
			Renderer2D::start_new_batch();

		const float textureIndex = 0.0f; // White texture index
		const float tilingFactor = 1.0f;

		// TODO: Create a function that fill vertex data
		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
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

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();
		
		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices || s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot - 1)
			Renderer2D::start_new_batch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_renderer2DData.textureSlotIndex; i++)
		{
			if (*s_renderer2DData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_renderer2DData.textureSlotIndex;
			s_renderer2DData.textureSlots[s_renderer2DData.textureSlotIndex] = texture;
			s_renderer2DData.textureSlotIndex++;
		}

		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
	}

	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		fill_rect({position.x, position.y, 0.0f}, size, subTexture, tintColor, tilingFactor);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		const Ref<Texture2D> texture = subTexture->get_texture();
		const glm::vec2* textureCoords = subTexture->get_tex_coords();

		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices || s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot - 1)
			Renderer2D::start_new_batch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_renderer2DData.textureSlotIndex; i++)
		{
			if (*s_renderer2DData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_renderer2DData.textureSlotIndex;
			s_renderer2DData.textureSlots[s_renderer2DData.textureSlotIndex] = texture;
			s_renderer2DData.textureSlotIndex++;
		}

		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[0];
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[1];
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[2];
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[3];
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
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

		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices || s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot - 1)
			Renderer2D::start_new_batch();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), radRotation, { 0.0f, 0.0f, 1.0f });

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[0];
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[1];
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[2];
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[3];
		s_renderer2DData.quadVertexBufferPtr->color = color;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
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

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices || s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot - 1)
			Renderer2D::start_new_batch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_renderer2DData.textureSlotIndex; i++)
		{
			if (*s_renderer2DData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_renderer2DData.textureSlotIndex;
			s_renderer2DData.textureSlots[s_renderer2DData.textureSlotIndex] = texture;
			s_renderer2DData.textureSlotIndex++;
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), radRotation, { 0.0f, 0.0f, 1.0f });

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[0];
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[1];
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[2];
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[3];
		s_renderer2DData.quadVertexBufferPtr->color = tintColor;
		s_renderer2DData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
		s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_renderer2DData.quadVertexBufferPtr++;

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
	}


	void Renderer2D::fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		fill_rect({position.x, position.y, 0.0f}, size, radRotation, subTexture, tintColor, tilingFactor);
	}

	void Renderer2D::fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		const Ref<Texture2D> texture = subTexture->get_texture();
		const glm::vec2* textureCoords = subTexture->get_tex_coords();

		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices || s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot - 1)
			Renderer2D::start_new_batch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_renderer2DData.textureSlotIndex; i++)
		{
			if (*s_renderer2DData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_renderer2DData.textureSlotIndex;
			s_renderer2DData.textureSlots[s_renderer2DData.textureSlotIndex] = texture;
			s_renderer2DData.textureSlotIndex++;
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), radRotation, { 0.0f, 0.0f, 1.0f });

		// TODO: Refactor the other functions like this
		for (uint32_t i = 0;i < 4;i++)
		{
			s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[i];
			s_renderer2DData.quadVertexBufferPtr->color = tintColor;
			s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
			s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_renderer2DData.quadVertexBufferPtr++;
		}

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
	}

	void Renderer2D::fill_rect(const glm::mat4& transform, const glm::vec4& color)
	{
		HDR_PROFILE_FUNCTION();
		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices)
			Renderer2D::start_new_batch();

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f; // White texture index
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[i];
			s_renderer2DData.quadVertexBufferPtr->color = color;
			s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
			s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_renderer2DData.quadVertexBufferPtr++;
		}

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
	}

	void Renderer2D::fill_rect(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		HDR_PROFILE_FUNCTION();

		if (s_renderer2DData.quadIndexCount >= Renderer2DData::maxIndices)
			Renderer2D::start_new_batch();

		float textureIndex = 0.0f; // White texture index

		for (uint32_t i = 1; i < s_renderer2DData.textureSlotIndex; i++)
		{
			if (*s_renderer2DData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_renderer2DData.textureSlotIndex >= Renderer2DData::maxTextureSlot)
				Renderer2D::start_new_batch();

			textureIndex = (float)s_renderer2DData.textureSlotIndex;
			s_renderer2DData.textureSlots[s_renderer2DData.textureSlotIndex] = texture;
			s_renderer2DData.textureSlotIndex++;
		}

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_renderer2DData.quadVertexBufferPtr->position = transform * s_renderer2DData.quadVertexPositions[i];
			s_renderer2DData.quadVertexBufferPtr->color = tintColor;
			s_renderer2DData.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_renderer2DData.quadVertexBufferPtr->texIndex = textureIndex;
			s_renderer2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_renderer2DData.quadVertexBufferPtr++;
		}

		s_renderer2DData.quadIndexCount += 6;

		s_renderer2DData.stats.quadCount++;
	}

#pragma endregion


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


	Renderer2D::Statistics Renderer2D::get_stats()
	{
		return s_renderer2DData.stats;
	}

	void Renderer2D::reset_stats()
	{
		memset(&s_renderer2DData.stats, 0, sizeof(Statistics));
	}
}