#version 330 core

uniform ivec2 origin;
uniform int range;

out vec4 color;

void main()
{
	float alpha = 1.0 - (distance(gl_FragCoord.xy, origin) / range);
	color = vec4(0.0, 1.0, 1.0, alpha * 0.4);
}
