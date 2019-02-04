#include "lightdemo.h"

int main()
{
	win::system system;
	win::display display = system.make_display("lightdemo", 1280, 720);
	display.vsync(true);

	std::vector<ent::entity> entity_list;

	float x = 0.0f, y = 0.0f;
	const win::area area(-8.0f, 8.0f, -4.5f, 4.5f);
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
	win::font_renderer font_renderer = display.make_font_renderer(display.width(), display.height(), area.left, area.right, area.bottom, area.top);
	win::font font1 = font_renderer.make_font(roll["arial.ttf"], 0.3f);
	core::renderer renderer(display, roll, area);

	while(display.process() && !quit)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		font_renderer.draw(font1, "lightdemo", 0.0f, 4.0f, win::color(1.0f, 1.0f, 0.0f, 0.3f), win::font_renderer::CENTERED);

		ent::entity::render(renderer, entity_list);
		renderer.quad_pass.send();

		renderer.shadow_pass.add(entity_list, x, y, 3.0f);
		const std::vector<float> copy = renderer.shadow_pass.lights.at(0);
		renderer.shadow_pass.send();

		int index = 0;
		for(int i = 2; i < (int)copy.size() - 2; i += 2)
		{
			char buffer[100];
			press::bwrite(buffer, sizeof(buffer), "{}", index);
			font_renderer.draw(font1, buffer, copy.at(i), copy.at(i + 1), win::color(1.0f, 0.0f, 0.0f));
			index++;
		}

		/*
		if(entity_list.size() > 0)
		{
			const ent::entity &entity = entity_list[0];
			float angle = -atan2f(y - entity.y, x - entity.x);
			if(angle < 0.0f)
				angle += M_PI * 2.0f;
			if(angle > M_PI * 2.0f)
				angle -= M_PI * 2.0f;

			press::writeln("angle to upperleft: {}", angle);
		}
		*/

		display.swap();
	}
}
