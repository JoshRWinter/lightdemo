#include "../lightdemo.h"

core::shadow::shadow(const core::renderer &parent, win::roll &roll)
	: renderer(parent)
{
	program = win::load_shaders(roll["shadowcaster.vert"], roll["shadowcaster.frag"]);
	glUseProgram(program);

	float matrix[16];
	win::init_ortho(matrix, renderer.world.left, renderer.world.right, renderer.world.bottom, renderer.world.top);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, false, matrix);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);
	glEnableVertexAttribArray(0);
}

void core::shadow::add(float x, float y)
{
	buffer_geometry.push_back(x);
	buffer_geometry.push_back(y);

	buffer_geometry.push_back(-3.0f);
	buffer_geometry.push_back(-3.0f);

	buffer_geometry.push_back(3.0f);
	buffer_geometry.push_back(-3.0f);

	buffer_geometry.push_back(3.0f);
	buffer_geometry.push_back(3.0f);

	buffer_geometry.push_back(-3.0f);
	buffer_geometry.push_back(3.0f);

	buffer_geometry.push_back(-3.0f);
	buffer_geometry.push_back(-3.0f);
}

void core::shadow::send()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, geometry);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer_geometry.size(), buffer_geometry.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, buffer_geometry.size() / 2);

	buffer_geometry.clear();
}
