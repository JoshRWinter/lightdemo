#ifndef SHADOWFILL_H
#define SHADOWFILL_H

#include <vector>
#include <algorithm>
#include <cmath>

struct vertex
{
	vertex() : overtake(overtake_style::NONE) {}
	void set_angle(float xp, float yp)
	{
		angle = -atan2(yp - y, xp - x);
		if(angle < 0.0f)
			angle += M_PI * 2.0f;
		else if(angle > M_PI * 2.0f)
			angle -= M_PI * 2.0f;
	}

	enum class overtake_style { NONE, PRE, POST } overtake;

	float x, y;
	float angle;
};

template <typename Solid, template <typename> typename List> std::vector<vertex> get_all_corners(const List<Solid> &solids, float x, float y, float range)
{
	std::vector<vertex> corners;

	// add the four corners
	vertex v;
	v.x = x + range;
	v.y = y - range;
	v.set_angle(x, y);
	corners.push_back(v);

	v.x = x + range;
	v.y = y + range;
	v.set_angle(x, y);
	corners.push_back(v);

	v.x = x - range;
	v.y = y + range;
	v.set_angle(x, y);
	corners.push_back(v);

	v.x = x - range;
	v.y = y - range;
	v.set_angle(x, y);
	corners.push_back(v);

	for(const Solid &solid : solids)
	{
		const bool left = x < solid.x;
		const bool right = x > solid.x + solid.w;
		const bool bottom = y > solid.y + solid.h;
		const bool top = y < solid.y;

		if(left)
		{
			vertex corner1;
			corner1.x = solid.x;
			corner1.y = solid.y + solid.h;
			corner1.set_angle(x, y);

			vertex corner2;
			corner2.x = solid.x;
			corner2.y = solid.y;
			corner2.set_angle(x, y);

			if(top)
				corner1.overtake = vertex::overtake_style::PRE;
			else if(bottom)
				corner2.overtake = vertex::overtake_style::POST;
			else
			{
				corner1.overtake = vertex::overtake_style::PRE;
				corner2.overtake = vertex::overtake_style::POST;
			}

			corners.push_back(corner1);
			corners.push_back(corner2);
		}
		else if(right)
		{
			vertex corner1;
			corner1.x = solid.x + solid.w;
			corner1.y = solid.y;
			corner1.set_angle(x, y);

			vertex corner2;
			corner2.x = solid.x + solid.w;
			corner2.y = solid.y + solid.h;
			corner2.set_angle(x, y);

			if(top)
				corner2.overtake = vertex::overtake_style::POST;
			else if(bottom)
				corner1.overtake = vertex::overtake_style::PRE;
			else
			{
				corner1.overtake = vertex::overtake_style::PRE;
				corner2.overtake = vertex::overtake_style::POST;
			}

			corners.push_back(corner1);
			corners.push_back(corner2);
		}
		if(bottom)
		{
			vertex corner1;
			corner1.x = solid.x + solid.w;
			corner1.y = solid.y + solid.h;
			corner1.set_angle(x, y);

			vertex corner2;
			corner2.x = solid.x;
			corner2.y = solid.y + solid.h;
			corner2.set_angle(x, y);

			if(left)
				corner2.overtake = vertex::overtake_style::POST;
			else if(right)
				corner1.overtake = vertex::overtake_style::PRE;
			else
			{
				corner1.overtake = vertex::overtake_style::PRE;
				corner2.overtake = vertex::overtake_style::POST;
			}

			corners.push_back(corner1);
			corners.push_back(corner2);
		}
		else if(top)
		{
			vertex corner1;
			corner1.x = solid.x;
			corner1.y = solid.y;
			corner1.set_angle(x, y);

			vertex corner2;
			corner2.x = solid.x + solid.w;
			corner2.y = solid.y;
			corner2.set_angle(x, y);

			if(left)
				corner2.overtake = vertex::overtake_style::POST;
			else if(right)
				corner1.overtake = vertex::overtake_style::PRE;
			else
			{
				corner1.overtake = vertex::overtake_style::PRE;
				corner2.overtake = vertex::overtake_style::POST;
			}

			corners.push_back(corner1);
			corners.push_back(corner2);
		}
	}

	std::sort(corners.begin(), corners.end(), [](const vertex &corner1, const vertex &corner2)
	{
		return corner1.angle < corner2.angle;
	});

	return corners;
}

template <typename Solid, template <typename> typename List> vertex cast_ray(const List<Solid> &solids, float sourcex, float sourcey, float targetx, float targety, float angle)
{
	return {};
}

template <typename Solid, template <typename> typename List> std::vector<float> shadowfill(const List<Solid> &solids, float x, float y, float range)
{
	std::vector<float> verts;

	const std::vector<vertex> corners = get_all_corners(solids, x, y, range);

	verts.push_back(x);
	verts.push_back(y);

	for(const auto &corner : corners)
	{
		if(corner.overtake == vertex::overtake_style::PRE)
		{
			verts.push_back(-cos(corner.angle) * range);
			verts.push_back(sin(corner.angle) * range);
		}

		verts.push_back(corner.x);
		verts.push_back(corner.y);

		if(corner.overtake == vertex::overtake_style::POST)
		{
			verts.push_back(-cos(corner.angle) * range);
			verts.push_back(sin(corner.angle) * range);
		}
	}

	verts.push_back(verts.at(2));
	verts.push_back(verts.at(3));

	return verts;
}

#endif
