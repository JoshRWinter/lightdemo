#include "../lightdemo.h"

core::quad::quad(const core::renderer &parent, win::roll &roll)
	: renderer(parent)
{
	program = win::load_shaders(roll["quad.vert"], roll["quad.frag"]);
	glUseProgram(program);

	float matrix[16];
	win::init_ortho(matrix, renderer.world.left, renderer.world.right, renderer.world.bottom, renderer.world.top);

	const float verts[] =
	{
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f
	};

	const unsigned int element_indices[] =
	{
		0, 1, 3, 1, 2, 3
	};

	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, false, matrix);

	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), element_indices, GL_STATIC_DRAW);

	// triangle data
	glBindBuffer(GL_ARRAY_BUFFER, triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);
	glEnableVertexAttribArray(0);

	// position and size attributes
	glBindBuffer(GL_ARRAY_BUFFER, position_size);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, NULL);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
}

void core::quad::add(const ent::entity &entity)
{
	buffer_position_size.push_back(entity.x);
	buffer_position_size.push_back(entity.y);
	buffer_position_size.push_back(entity.w);
	buffer_position_size.push_back(entity.h);
}

void core::quad::send()
{
	glBindVertexArray(vao);
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, position_size);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer_position_size.size(), buffer_position_size.data(), GL_DYNAMIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL, buffer_position_size.size() / 4);

	buffer_position_size.clear();
}
