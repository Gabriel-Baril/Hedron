#pragma once

namespace hdn
{
	// void editor_f32_field( float *v );
	// void editor_vec2f32_field( vec2f32 *v );
	// void editor_vec3f32_field( vec3f32 *v );
	// void editor_vec4f32_field( vec4f32 *v );
	// void editor_checkbox( bool *checked );
	void editor_begin_panel();
	void editor_end_panel();

	class IEditorPanel
	{
	public:
		virtual void Begin()
		{
			editor_begin_panel();
		}
		virtual void Update( float dt ) = 0;
		virtual void End()
		{
			editor_end_panel();
		}

		void Enable()
		{
			m_Enabled = true;
		}

		void Disable()
		{
			m_Enabled = false;
		}

		bool Enabled()
		{
			return m_Enabled;
		}
	private:
		bool m_Enabled = true;
	};
}