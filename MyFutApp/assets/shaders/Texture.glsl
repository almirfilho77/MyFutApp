#type vertex
#version 430 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec2 v_TextureCoord;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
	v_TextureCoord = a_TextureCoord;
}

#type fragment
#version 430 core
			
layout(location = 0) out vec4 color;

in vec2 v_TextureCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TextureCoord) * u_Color;
}