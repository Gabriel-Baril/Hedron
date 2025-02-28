#pragma once

#include "core/application/application_layer.h"

namespace hdn
{
	class EditorLayer : public IApplicationLayer
	{
	public:
		// template<typename T, typename... Args>
		// IEditorPanel* Register(Args&&... args)
		// {
		// 	m_Panels.push_back(CreateRef<T>(std::forward<Args>(args)...));
		// 	return m_Panels.back();
		// }
		// 
		// template<typename T>
		// void Unregister()
		// {
		// 	// TODO: Unregister layer of the same type (The layer type is defined by the class type)
		// }
		// 
		// void Unregister(int id)
		// {
		// 	// TODO: Unregister layer by it's id
		// }

		void OnUpdate(float dt)
		{
			// for (const auto& panel : m_Panels)
			// {
			// 	if (!panel->Enabled())
			// 	{
			// 		continue;
			// 	}
			// 	panel->Begin();
			// 	panel->OnUpdate(dt);
			// 	panel->End();
			// }
		}

	private:
		// vector<Ref<IPanel>> m_Panels;
	};
}