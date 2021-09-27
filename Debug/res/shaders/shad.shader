#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout( location = 1) in vec2 textCoord;

uniform mat4 u_MVP;
out vec2 v_TextCoord;

void main()
{
	
	gl_Position = u_MVP * vec4(position, 1.0);
	v_TextCoord = textCoord;

};
#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TextCoord;

uniform vec4 u_Color;
uniform float ambient_value;
uniform vec4 lightColor;
uniform sampler2D u_Texture;

void main()
{
	vec4 textColor = texture(u_Texture, v_TextCoord);
	float ambientIntens = 0.5;
	vec4 ambient = lightColor * ambient_value;
	color = ambient * u_Color;
	

};