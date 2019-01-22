#ifndef LIGHTDEMO_H
#define LIGHTDEMO_H

#include <random>
#include <time.h>

inline struct mersenne_struct
{
	mersenne_struct()
		: gen(time(NULL)) {}

	float operator()(float low, float high)
	{
		return std::uniform_real_distribution(low, high)(gen);
	}

	std::mt19937 gen;
} mersenne;

#include "press.h"
#include <win.h>

#include "entity/entity.h"
#include "render/render.h"

#endif
