#pragma once

#include "Hedron/Renderer/OrthographicCamera.h"
#include "Hedron/Renderer/Texture2D.h"


namespace Hedron
{

	// super static class !
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin_scene(const OrthographicCamera& camera);
		static void end_scene();
		static void flush();


		// Primitives
		static void draw_rect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_rect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad();
		static void draw_trigon();
		static void draw_polygon();
		static void draw_line();
		static void draw_square();
		static void draw_circle(const glm::vec2& position, float radius, uint32_t sideCount, const glm::vec4 color);
		static void draw_oval();
		static void draw_triangle();

		static void fill_rect(const glm::vec2& position, const glm::vec2& size);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);

		static void fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const glm::vec4& color);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const glm::vec4& color);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void fill_rect(const glm::vec2& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);
		static void fill_rect(const glm::vec3& position, const glm::vec2& size, float radRotation, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor);

		static void fill_quad();
		static void fill_trigon();
		static void fill_polygon();
		static void fill_line();
		static void fill_square();
		static void fill_circle();
		static void fill_oval();
		static void fill_triangle();


	private:
		static void build_rect_data();
		static void build_circle_data();

		Renderer2D() = delete;
	};

	struct Transform2D
	{
		glm::vec3 position;
		glm::vec2 size;
		float radRotation;
	};
}