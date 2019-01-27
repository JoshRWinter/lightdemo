#ifndef SHADOWFILL_H
#define SHADOWFILL_H

#include <vector>

struct vertex
{
	vertex() : propagate(false) {}

	float x, y;
	bool propagate;
};

struct triangle
{
	vertex v[3];
};

template <typename Solid> std::vector<triangle> get_visible_sides(const Solid &solid, float x, float y)
{
	std::vector<triangle> visibles;

	triangle tri;
	tri.v[0].x = x;
	tri.v[0].y = y;

	bool left = false;
	bool right = false;

	if(x < solid.x) // left side
	{
		fprintf(stderr, "LEFT SIDE\n");
		left = true;

		tri.v[1].x = solid.x;
		tri.v[1].y = solid.y;

		tri.v[2].x = solid.x;
		tri.v[2].y = solid.y + solid.h;

		visibles.push_back(tri);
	}
	else if(x > solid.x + solid.w) // right side
	{
		fprintf(stderr, "RIGHT SIDE\n");
		right = true;

		tri.v[1].x = solid.x + solid.w;
		tri.v[1].y = solid.y + solid.h;

		tri.v[2].x = solid.x + solid.w;
		tri.v[2].y = solid.y;

		visibles.push_back(tri);
	}

	if(y < solid.y) // top side
	{
		fprintf(stderr, "TOP SIDE\n");
		tri.v[1].x = solid.x + solid.w;
		tri.v[1].y = solid.y;

		tri.v[2].x = solid.x;
		tri.v[2].y = solid.y;

		visibles.push_back(tri);

/*
		if(right)
		{
			visibles[0].v[1].propagate = true;
			visibles[1].v[2].propagate = true;
		}
		else if(left)
		{
			visibles[0].v[2].propagate = true;
			visibles[1].v[1].propagate = true;
		}
*/
	}
	else if(y > solid.y + solid.h) // bottom side
	{
		fprintf(stderr, "BOTTOM SIDE\n");
		tri.v[1].x = solid.x;
		tri.v[1].y = solid.y + solid.h;

		tri.v[2].x = solid.x + solid.w;
		tri.v[2].y = solid.y + solid.h;

		visibles.push_back(tri);

/*
		if(right)
		{
			visibles[0].v[2].propagate = true;
			visibles[1].v[1].propagate = true;
		}
		else if(left)
		{
			visibles[0].v[1].propagate = true;
			visibles[1].v[2].propagate = true;
		}
*/
	}

	return visibles;
}

template <typename Solid, template <typename c> typename List> std::vector<float> shadowfill(const List<Solid> &solids, float x, float y, float range)
{
	std::vector<float> verts;

	for(const auto &solid : solids)
	{
		const auto visibles = get_visible_sides(solid, x, y);

		fprintf(stderr, "{\n");
		for(const triangle &tri : visibles)
		{
			verts.push_back(tri.v[0].x);
			verts.push_back(tri.v[0].y);

			verts.push_back(tri.v[1].x);
			verts.push_back(tri.v[1].y);

			verts.push_back(tri.v[2].x);
			verts.push_back(tri.v[2].y);
		}
		fprintf(stderr, "}\n");
	}

	return verts;
}

#endif
