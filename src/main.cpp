#include "lightdemo.h"

int main()
{
	win::system system;
	win::display display = system.make_display("lightdemo", 1280, 720);
	display.vsync(true);

	std::vector<ent::entity> entity_list;

	float x = 0.0f, y = 0.0f;
	const win::area area(-8.0f, 8.0f, 4.5f, -4.5f);
	const float width = display.width();
	const float height = display.height();

	bool quit = false;
	display.event_button([&quit, &entity_list, &area, &x, &y](const win::button button, const bool press)
	{
		switch(button)
		{
			case win::button::ESC:
				quit = true;
				break;
			case win::button::MOUSE_LEFT:
				if(press)
					entity_list.push_back(ent::entity(x, y));
				break;
		}

	});

	display.event_mouse([&x, &y, &width, &height, &area](const float xp, const float yp)
	{
		x = ((xp / width) * (area.right * 2.0f)) - area.right;
		y = ((yp / height) * (area.bottom * 2.0f)) - area.bottom;
	});

	win::roll roll("assets");
	core::renderer renderer(display, roll, area);

	while(display.process() && !quit)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		ent::entity::render(renderer, entity_list);
		renderer.quad_pass.send();

		renderer.shadow_pass.add(0.0f, 0.0f);
		renderer.shadow_pass.send();

		display.swap();
	}
}
