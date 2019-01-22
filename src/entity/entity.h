#ifndef ENTITY_H
#define ENTITY_H

#include "../lightdemo.h"

namespace core { struct renderer; }

namespace ent
{

	struct entity
	{
		static constexpr float WIDTH_LOW = 0.5f, WIDTH_HIGH = 1.75f;
		static constexpr float HEIGHT_LOW = 0.5f, HEIGHT_HIGH = 1.75f;

		entity(float, float);

		static void render(core::renderer&, const std::vector<ent::entity> &entity_list);

		float x, y, w, h;
	};

}

#endif
