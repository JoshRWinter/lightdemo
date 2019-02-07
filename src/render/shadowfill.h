#ifndef SHADOWFILL_H
#define SHADOWFILL_H

#include <vector>
#include <algorithm>
#include <cmath>

struct point
{
	point() = default;
	point(const float px, const float py)
		: x(px), y(py) {}

	float x, y;
};

struct segment
{
	segment() = default;
	segment(const point x, const point y)
		: a(x), b(y) {}

	point a, b;
};

struct intersection
{
	point p;
	float distance;
};

intersection get_intersection(float lightx, float lighty, float dx, float dy, const point &sa, const point &sb, bool &collision)
{
	intersection intersect;
	intersect.p.x = intersect.p.y = 0.0f;
	intersect.distance = 0.0f;
	/*
	{
		intersect.p.x = lightx + dx * 5.0f;
		intersect.p.y = lighty + dy * 5.0f;
		intersect.distance = 0.0f;
		collision = true;
		return intersect;
	}
	*/

	const float r_px = lightx;
	const float r_py = lighty;
	const float r_dx = dx - lightx;
	const float r_dy = dy - lighty;

	const float s_px = sa.x;
	const float s_py = sa.y;
	const float s_dx = sb.x - sa.x;
	const float s_dy = sb.y - sa.y;

	// check for parellel
	const float r_mag = sqrtf((r_dx * r_dx) + (r_dy * r_dy));
	const float s_mag = sqrtf((s_dx * s_dx) + (s_dy * s_dy));
	if(r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag)
	{
		// the lines are parellel
		collision = false;
		return intersect;
	}

	const float t2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
	const float t1 = (s_px + s_dx * t2 - r_px) / r_dx;

	if(t1 < 0.0f || (t2 < 0.0f || t2 > 1.0f))
	{
		collision = false;
		return intersect;
	}

	collision = true;
	intersect.p.x = r_px + r_dx * t1;
	intersect.p.y = r_py + r_dy * t1;
	intersect.distance = t1;

	return intersect
	;
}

template <typename Solid, template <typename> typename List> std::vector<float> shadowfill(const List<Solid> &solids, const float lightx, const float lighty, const float radius)
{
	std::vector<float> angles;
	std::vector<segment> segments;

	for(const Solid &solid : solids)
	{
		const float offset = 0.0001f;
		float angle;

		// left
		angle = atan2f(solid.y - lighty, solid.x - lightx);
		angles.push_back(angle - offset);
		angles.push_back(angle + offset);

		angle = atan2f((solid.y + solid.h) - lighty, solid.x - lightx);
		angles.push_back(angle - offset);
		angles.push_back(angle + offset);

		segments.push_back(segment(point(solid.x, solid.y), point(solid.x, solid.y + solid.h)));

		// right
		angle = atan2f(solid.y - lighty, (solid.x + solid.w) - lightx);
		angles.push_back(angle - offset);
		angles.push_back(angle + offset);

		angle = atan2f((solid.y + solid.h) - lighty, (solid.x + solid.w) - lightx);
		angles.push_back(angle - offset);
		angles.push_back(angle + offset);

		segments.push_back(segment(point(solid.x + solid.w, solid.y), point(solid.x + solid.w, solid.y + solid.h)));

		// bottom
		segments.push_back(segment(point(solid.x, solid.y), point(solid.x + solid.w, solid.y)));

		// top
		segments.push_back(segment(point(solid.x, solid.y + solid.h), point(solid.x + solid.w, solid.y + solid.h)));
	}

	// four corners
	angles.push_back(atan2f((lighty - radius) - lighty, (lightx - radius) - lightx)); // bottom left
	angles.push_back(atan2f((lighty - radius) - lighty, (lightx + radius) - lightx)); // bottom right
	angles.push_back(atan2f((lighty + radius) - lighty, (lightx + radius) - lightx)); // top right
	angles.push_back(atan2f((lighty + radius) - lighty, (lightx - radius) - lightx)); // top left

	segments.push_back(segment(point(lightx - radius, lighty - radius), point(lightx - radius, lighty + radius)));
	segments.push_back(segment(point(lightx + radius, lighty - radius), point(lightx + radius, lighty + radius)));
	segments.push_back(segment(point(lightx - radius, lighty - radius), point(lightx + radius, lighty - radius)));
	segments.push_back(segment(point(lightx - radius, lighty + radius), point(lightx + radius, lighty + radius)));

	std::sort(angles.begin(), angles.end());

	std::vector<float> verts;
	verts.push_back(lightx);
	verts.push_back(lighty);

	// plot points
	for(const float angle : angles)
	{
		const float dx = cosf(angle);
		const float dy = sinf(angle);

		const float stepx = lightx + dx;
		const float stepy = lighty + dy;

		float x = 0.0f, y = 0.0f;
		float closest = 100000.0f;
		for(const segment &seg : segments)
		{
			bool collision = false;

			const intersection intersect = get_intersection(lightx, lighty, stepx, stepy, seg.a, seg.b, collision);
			if(!collision)
				continue;

			if(intersect.distance < closest)
			{
				closest = intersect.distance;
				x = intersect.p.x;
				y = intersect.p.y;
			}
		}

		verts.push_back(x);
		verts.push_back(y);
	}

	verts.push_back(verts.at(2));
	verts.push_back(verts.at(3));

	return verts;
}

#endif
