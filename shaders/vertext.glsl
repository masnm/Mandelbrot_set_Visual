#version 330 core

layout (location = 0) in vec2 ver_pos;
layout (location = 1) in vec2 ver_uv;
layout (location = 2) in vec4 ver_colour;

uniform float vtime;
uniform vec2 vresolution;
uniform vec2 fix_point;
uniform vec2 scale;
uniform vec2 offset;

out vec4 ourColor;
out float ftime;
out vec2 fresolution;
out vec2 fuv;

void main()
{
	gl_Position = vec4 ( ver_pos, 1.0, 1.0 );

	ourColor = ver_colour;
	ftime = vtime;
	fresolution = vresolution;
	fuv = ver_uv;
}
