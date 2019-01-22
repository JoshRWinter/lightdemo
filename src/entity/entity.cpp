#include "../lightdemo.h"

ent::entity::entity(float xp, float yp)
{
	w = mersenne(WIDTH_LOW, WIDTH_HIGH);
	h = mersenne(HEIGHT_LOW, HEIGHT_HIGH);

	press::writeln("width: {}, height: {}", w, h);

	x = xp - (w / 2.0f);
	y = yp - (h / 2.0f);
}

void ent::entity::render(core::renderer &renderer, const std::vector<ent::entity> &entity_list)
{
	for(const ent::entity &entity : entity_list)
		renderer.quad_pass.add(entity);
}
