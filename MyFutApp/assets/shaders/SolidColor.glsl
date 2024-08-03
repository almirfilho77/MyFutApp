#type vertex
#version 430 core
			
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform vec4 u_Color;

out vec3 v_Position;
out vec4 v_Color;

void main()
{
	v_Position = a_Position;
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
	v_Color = u_Color;	
}

#type fragment
#version 430 core
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
	color = v_Color;
}