#type vertex
#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec3 v_color;

void main()
{
	v_color = a_color;
	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec3 color;

in vec3 v_color;

void main()
{
	color = v_color;
}