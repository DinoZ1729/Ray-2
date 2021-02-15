#pragma once

/*
* Inspired by HillsoftCode:
* https://www.youtube.com/playlist?list=PLAqGIYgEAxrUO6ODA0pnLkM2UOijerFPv
*/

#include "Ray.h"
#include <vector>
class Shape;

//intersection info
struct Intersection
{
	Ray ray;
	vec3 normal;
	float t;
	Shape* shape;

	Intersection() : ray(), t(RAY_T_MAX), shape(nullptr) {}

	Intersection(const Ray& ray) : ray(ray), t(RAY_T_MAX), shape(nullptr) {}

	bool intersected() const
	{
		return shape != nullptr;
	}
	vec3 point() const
	{
		return ray.point(t);
	}
};

class Shape
{
public:

	void setReflection(float temp)
	{ reflection = temp; }

	void setBrightness(float temp)
	{ brightness = temp; }

	float getReflection()
	{ return reflection; }

	float getBrightness()
	{ return brightness; }

	virtual bool intersect(Intersection& intersection) = 0;
	virtual bool doesIntersect(const Ray& ray) = 0;
private:
	float brightness = 1.0f;
	float reflection = 0.5f;
};

class ShapeSet : public Shape
{
protected:
	std::vector<Shape*> shapes;

public:
	ShapeSet() {}

	void push_back(Shape* shape) { shapes.push_back(shape); }

	virtual bool intersect(Intersection& intersection)
	{
		bool doesIntersect = false;
		
		for (auto curShape : shapes)
		{
			if (curShape->intersect(intersection))
				doesIntersect = true;
		}
		
		return doesIntersect;
	}
	virtual bool doesIntersect(const Ray& ray)
	{
		for (auto curShape : shapes)
		{
			if (curShape->doesIntersect(ray))
				return true;
		}
		return false;
	}
};

class Plane : public Shape
{
protected:
	vec3 position;
	vec3 normal;
public:

	//horizontal plane
	Plane(float height)
	{
		position = { 0,0,height };
		normal = { 0,0,1 };
	}
	Plane(const vec3& position, const vec3& normal) :position(position), normal(normal) {}
	virtual bool intersect(Intersection& intersection)
	{
		float dot = intersection.ray.direction * normal;
		if (dot == 0.0f)
			return false;
		float t = (position - intersection.ray.origin) * normal / dot;
		if (t < RAY_T_MIN || t >= intersection.t)
			return false;
		intersection.t = t;
		intersection.shape = this;
		if (dot > 0)
			intersection.normal = -normal;
		else
			intersection.normal = normal;
		return true;
	}

	

	virtual bool doesIntersect(const Ray& ray)
	{
		float dot = ray.direction * normal;
		if (dot == 0.0f)
			return false;
		float t = (position - ray.origin) * normal / dot;
		if (t < RAY_T_MIN || t >= ray.tMax)
			return false;
		return true;
	}
};

class Sphere : public Shape
{
protected:
	vec3 centre;
	float radius = 1.0f;
public:
	Sphere() {};
	Sphere(const vec3& centre, float radius) :centre(centre), radius(radius) {}
	
	virtual bool intersect(Intersection& intersection)
	{
		// Transform ray so we can consider origin-centred sphere
		Ray localRay = intersection.ray;
		localRay.origin -= centre;

		// Calculate quadratic coefficients
		float a = localRay.direction.magnitute2();
		float b = 2 * localRay.direction * localRay.origin;
		float c = localRay.origin.magnitute2() - radius * radius;

		// Check whether we intersect
		float discriminant = b * b - 4 * a * c;

		if (discriminant < 0.0f)
			return false;

		float sqrtD = sqrt(discriminant);
		// Find two points of intersection, t1 close and t2 far
		float t1 = (-b - sqrtD) / (2 * a);
		float t2 = (-b + sqrtD) / (2 * a);

		// First check if close intersection is valid
		if (t1 > RAY_T_MIN && t1 < intersection.t)
		{
			intersection.t = t1;
			intersection.normal = (intersection.point() - centre) / radius;
		}
		else if (t2 > RAY_T_MIN && t2 < intersection.t)
		{
			intersection.t = t2;
			intersection.normal = -(intersection.point() - centre) / radius;
		}
		else
			return false;

		intersection.shape = this;

		return true;
	}
	virtual bool doesIntersect(const Ray& ray)
	{
		// Transform ray so we can consider origin-centred sphere
		Ray localRay = ray;
		localRay.origin -= centre;

		// Calculate quadratic coefficients
		float a = localRay.direction.magnitute2();
		float b = 2 * localRay.direction * localRay.origin;
		float c = localRay.origin.magnitute2() - radius * radius;

		// Check whether we intersect
		float discriminant = b * b - 4 * a * c;

		if (discriminant < 0.0f)
			return false;

		float sqrtD = sqrt(discriminant);
		// Find two points of intersection, t1 close and t2 far
		float t1 = (-b - sqrtD) / (2 * a);
		float t2 = (-b + sqrtD) / (2 * a);

		// First check if close intersection is valid
		if (t1 > RAY_T_MIN && t1 < RAY_T_MAX)
			return true;
		if (t2 > RAY_T_MIN && t2 < RAY_T_MAX)
			return true;

		return false;
	}
};
