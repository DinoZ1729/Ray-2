#pragma once
#include "vec3.h"

#define RAY_T_MIN 0.0001f
#define RAY_T_MAX 10000000.0f

struct Ray
{
	vec3 origin;
	vec3 direction;
	float tMax;

	Ray() :tMax(RAY_T_MAX) {};

	Ray(vec3 origin, vec3 direction, float tMax = RAY_T_MAX) :origin(origin), direction(direction), tMax(tMax) {}

	vec3 point(float t) const { return origin + t * direction; }
};