#include "../lightdemo.h"

core::shadow::shadow(const core::renderer &parent, win::roll &roll)
	: renderer(parent)
{
	program = win::load_shaders(roll["shadowcaster.vert"], roll["shadowcaster.frag"]);
}
