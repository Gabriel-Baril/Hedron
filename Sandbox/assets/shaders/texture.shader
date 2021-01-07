#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_viewProjection;

out vec2 v_texCoord;
out vec4 v_color;


void main()
{
	v_texCoord = a_texCoord;
	v_color = a_color;
	gl_Position = u_viewProjection * vec4(a_position, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec4 v_color;

uniform float u_tilingFactor;
uniform sampler2D u_texture;

void main()
{
	color = v_color;
	//color = texture(u_texture, v_texCoord * u_tilingFactor) * v_color;
}