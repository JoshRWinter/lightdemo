#ifndef RENDER_H
#define RENDER_H

#include <vector>

namespace core
{
	struct renderer;

	struct quad
	{
		quad(const core::renderer&, win::roll&);

		void add(const ent::entity&);
		void send();

		win::program program;
		win::vao vao;

		win::vbo triangle;
		win::vbo position_size;
		win::ebo ebo;

		std::vector<float> buffer_position_size;

		const core::renderer &renderer;
	};

	struct shadow
	{
		shadow(const core::renderer&, win::roll&);
		void add(float, float);
		void send();

		win::program program;
		win::vao vao;

		win::vbo geometry;

		std::vector<float> buffer_geometry;

		const core::renderer &renderer;
	};

	struct renderer
	{
		renderer(const win::display&, win::roll&, const win::area&);

		win::area world;

		quad quad_pass;
		shadow shadow_pass;
	};
}

#endif
