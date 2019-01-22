#version 330 core

layout (location = 0) in vec2 triangle;
layout (location = 1) in vec2 position;
layout (location = 2) in vec2 size;

uniform mat4 projection;

void main()
{
	mat4 translate = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, position.x + (size.x / 2.0), position.y + (size.y / 2.0), 0.0, 1.0);
	gl_Position = projection * translate * vec4(triangle.x * size.x, triangle.y * size.y, 0.0, 1.0);
}
