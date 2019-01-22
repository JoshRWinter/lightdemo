#include "../lightdemo.h"

core::renderer::renderer(const win::display &display, win::roll &roll, const win::area &area)
	: world(area)
	, quad_pass(*this, roll)
	, shadow_pass(*this, roll)
{
	glClearColor(0.125f, 0.125f, 0.125f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
