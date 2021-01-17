#include "SceneHierarchyPanel.h"

#include <Hedron/Scene/Components.h>

#include <imgui.h>
#include <imgui_internal.h>

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

		// Right-Click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_context->create_entity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		// TODO: Create a new class for the property panel
		ImGui::Begin("Properties");

		if (m_selectionContext)
		{
			draw_components(m_selectionContext);
		}
		
		ImGui::End();
	}

	void SceneHierarchyPanel::draw_entity_node(Entity entity)
	{
		std::string& tagName = entity.get_component<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tagName.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			
			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_context->destroy_entity(entity);
			if (m_selectionContext == entity)
				m_selectionContext = {};
		}
	}

	static void draw_vec3_control(const char* labelName, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(labelName);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(labelName);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.0f, 0.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		//X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		//Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		//Z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		ImGui::Columns(1); 
		
		ImGui::PopID();
	}

	template<typename Component, typename UIFunc>
	static void draw_component(const char* controlLabel, Entity& entity, UIFunc& uiFunction)
	{
		if (entity.has_component<Component>())
		{
			const ImGuiTreeNodeFlags treeNodeFlags = 
				ImGuiTreeNodeFlags_DefaultOpen | 
				ImGuiTreeNodeFlags_AllowItemOverlap | 
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_Framed | 
				ImGuiTreeNodeFlags_FramePadding;
			auto& component = entity.get_component<Component>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			
			ImGui::Separator();

			bool isOpened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, controlLabel);
			
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (isOpened)
			{
				uiFunction(component);

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.remove_component<SpriteRendererComponent>();
		}
	}

	// TODO: Create a draw function for each component type
	void SceneHierarchyPanel::draw_components(Entity entity)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.has_component<TagComponent>())
		{
			std::string& tagName = entity.get_component<TagComponent>().tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tagName.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tagName = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component..."))
			ImGui::OpenPopup("PropertyPopupAddComponent");

		if (ImGui::BeginPopup("PropertyPopupAddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_selectionContext.add_component<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_selectionContext.add_component<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		draw_component<TransformComponent>("Transform", entity, [](TransformComponent& component)
		{
			draw_vec3_control("Translation", component.translation);
			glm::vec3 rotation = glm::degrees(component.rotation);
			draw_vec3_control("Rotation", rotation);
			component.rotation = glm::radians(rotation);
			draw_vec3_control("Scale", component.scale, 1.0f);
		});

		draw_component<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& camera = component.camera;

			ImGui::Checkbox("Primary", &component.primary);

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

				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
		});

		draw_component<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});
	}

}