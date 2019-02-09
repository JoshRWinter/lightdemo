#include "../lightdemo.h"
#include "shadowfill.h"

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

void core::shadow::add(const std::vector<ent::entity> &solids, float x, float y, float range)
{

	lights.push_back(shadowfill<ent::entity, std::vector>(solids, x, y, range));
}

void core::shadow::send()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, geometry);

	for(const auto &verts : lights)
	{
		const int centerx = (verts.at(0) / (renderer.world.right * 2.0f)) * 1280;
		const int centery = (verts.at(1) / (renderer.world.top * 2.0f)) * 720;
		glUniform2i(glGetUniformLocation(program, "origin"), centerx + 640, centery + 360);
		glUniform1i(glGetUniformLocation(program, "range"), (3.0f / (renderer.world.right * 2.0f)) * 1280);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_DYNAMIC_DRAW);

		glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size() / 2);
	}

	lights.clear();
}
