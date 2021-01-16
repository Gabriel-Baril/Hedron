#include "SceneHierarchyPanel.h"

#include <Hedron/Scene/Components.h>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hedron
{
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		set_context(context);
	}

	void SceneHierarchyPanel::set_context(const Ref<Scene>& context)
	{
		m_context = context;
	}

	void SceneHierarchyPanel::on_imgui_render()
	{
		ImGui::Begin("Scene Hierarchy");
		
		m_context->m_registry.each([=](auto entityID)
		{
			draw_entity_node({ entityID, m_context.get() });
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectionContext = {};
		}

		//ImGui::Text("Entity %i", (uint32_t)m_selectionContext);

		ImGui::End();

		// TODO: Create a new class for the property panel
		ImGui::Begin("Properties");

		if (m_selectionContext)
			draw_components(m_selectionContext);
		
		ImGui::End();
	}

	void SceneHierarchyPanel::draw_entity_node(Entity entity)
	{
		std::string& tagName = entity.get_component<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tagName.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		if(opened)
		{
			ImGui::TreePop();
		}
	}

	// TODO: Create a draw function for each component type
	void SceneHierarchyPanel::draw_components(Entity entity)
	{
		if(entity.has_component<TagComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tag"))
			{
				std::string& tagName = entity.get_component<TagComponent>().tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tagName.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					tagName = std::string(buffer);
				}

				ImGui::Separator();
				ImGui::TreePop();
			}
		}

		if (entity.has_component<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.get_component<TransformComponent>().transform;
				glm::vec3 scaling = { transform[0][0], transform[1][1], transform[2][2] };

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f, -100.0f, 100.0f);
				ImGui::DragFloat3("Scaling", glm::value_ptr(scaling), 0.1f, -100.0f, 100.0f);
				transform[0][0] = scaling.x;
				transform[1][1] = scaling.y;
				transform[2][2] = scaling.z;

				ImGui::Separator();
				ImGui::TreePop();
			}
		}

		if (entity.has_component<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.get_component<CameraComponent>();
				auto& camera = cameraComponent.camera;
				
				ImGui::Checkbox("Primary", &cameraComponent.primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionString = projectionTypeStrings[(uint32_t)camera.get_projection_type()];

				if (ImGui::BeginCombo("Projection", currentProjectionString))
				{
					for (uint32_t i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionString = projectionTypeStrings[i];
							camera.set_projection_type(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.get_projection_type() == SceneCamera::ProjectionType::PERSPECTIVE)
				{
					float perspectiveVerticalFOV = glm::degrees(camera.get_perspective_vertical_fov());
					if (ImGui::DragFloat("FOV", &perspectiveVerticalFOV, 1.0f, 1.0f, 100.0f))
						camera.set_perspective_vertical_fov(glm::radians(perspectiveVerticalFOV));

					float perspectiveNearClip = camera.get_perspective_near_clip();
					if (ImGui::DragFloat("Near", &perspectiveNearClip, 0.05f, 0.01f, 1.0f))
						camera.set_perspective_near_clip(perspectiveNearClip);

					float perspectiveFarClip = camera.get_perspective_far_clip();
					if (ImGui::DragFloat("Far", &perspectiveFarClip, 10.0f, 1000.0f, 10000.0f))
						camera.set_perspective_far_clip(perspectiveFarClip);

				} 
				else if (camera.get_projection_type() == SceneCamera::ProjectionType::ORTHOGRAPHIC)
				{
					float orthoSize = camera.get_orthographic_size();
					if (ImGui::DragFloat("Size", &orthoSize, 1.0f, 1.0f, 100.0f))
						camera.set_orthographic_size(orthoSize);

					float orthoNearClip = camera.get_orthographic_near_clip();
					if (ImGui::DragFloat("Near", &orthoNearClip, 0.1f, -1.0f, 1.0f))
						camera.set_orthographic_near_clip(orthoNearClip);

					float orthoFarClip = camera.get_orthographic_far_clip();
					if (ImGui::DragFloat("Far", &orthoFarClip, 0.1f, -1.0f, 1.0f))
						camera.set_orthographic_far_clip(orthoFarClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}

				ImGui::Separator();
				ImGui::TreePop();
			}
		}
	
		if (entity.has_component<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& color = entity.get_component<SpriteRendererComponent>().color;
				
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
	}

}