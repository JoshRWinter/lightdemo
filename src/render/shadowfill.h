#ifndef SHADOWFILL_H
#define SHADOWFILL_H

#include <vector>
#include <algorithm>
#include <cmath>

struct point
{
	float x, y;
};

struct vertex
{
	point p;
	float angle;
};

template <typename Solid, template <typename> typename List> std::vector<vertex> calc_and_sort(const List<Solid> &solids, const float lightx, const float lighty, const float radius)
{
	std::vector<vertex> vertices;

	for(const Solid &solid : solids)
	{
		vertex vert;

		// left
		vert.p.x = solid.x;
		vert.p.y = solid.y;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		vert.p.x = solid.x;
		vert.p.y = solid.y + solid.h;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		// right
		vert.p.x = solid.x + solid.w;
		vert.p.y = solid.y;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		vert.p.x = solid.x + solid.w;
		vert.p.y = solid.y + solid.h;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		// bottom
		vert.p.x = solid.x;
		vert.p.y = solid.y;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		vert.p.x = solid.x + solid.w;
		vert.p.y = solid.y;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		// top
		vert.p.x = solid.x;
		vert.p.y = solid.y + solid.h;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);

		vert.p.x = solid.x + solid.w;
		vert.p.y = solid.y + solid.h;
		vert.angle = atan2(vert.p.y - lighty, vert.p.x - lightx);
		vertices.push_back(vert);
	}

	std::sort(vertices.begin(), vertices.end(), [](const vertex &left, const vertex &right)
	{
		return left.angle < right.angle;
	});

	return vertices;
}

template <typename Solid, template <typename> typename List> bool cast(const List<Solid> &solids, float rayx, float rayy, float radius, point &p)
{
	return false;
}

template <typename Solid, template <typename> typename List> std::vector<float> shadowfill(const List<Solid> &solids, float lightx, float lighty, float radius)
{
	std::vector<vertex> corners = calc_and_sort(solids, lightx, lighty, radius);
	std::vector<float> verts;

	verts.push_back(lightx);
	verts.push_back(lighty);

	for(const vertex &corner : corners)
	{
		verts.push_back(corner.p.x);
		verts.push_back(corner.p.y);
	}

	return verts;

/*
	const float angle = atan2f(lighty, lightx);
	const float mag = 4.0f;
	std::vector<float> verts;

	verts.push_back(-1.0f); verts.push_back(0.0f);
	verts.push_back(1.0f); verts.push_back(0.0f);
	verts.push_back(cos(angle) * mag); verts.push_back(sin(angle) * mag);

	return verts;
*/
}

#endif
