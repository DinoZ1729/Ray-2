#pragma once
#include <cmath>

struct vec3
{
	float x, y, z;

	vec3()
	{ x = y = z = 0; }

	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	vec3 operator + (const vec3& v) const
	{
		return vec3
		(
			x + v.x,
			y + v.y,
			z + v.z
		);
	}
	
	vec3 operator - (const vec3& v) const
	{
		return vec3
		(
			x - v.x,
			y - v.y,
			z - v.z
		);
	}
	
	float operator * (const vec3& v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}
	
	vec3 operator * (float k) const
	{
		return vec3
		(
			k*x,
			k*y,
			k*z
		);
	}
	vec3 operator / (float k) const
	{
		return vec3
		(
			x/k,
			y/k,
			z/k
		);
	}
	
	vec3& operator += (const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	vec3& operator -= (const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	vec3& operator *= (float k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	
	vec3& operator /= (float k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}
	
	vec3 operator - ()
	{
		return vec3(-x,-y,-z);
	}
	
	float magnitute2() const
	{
		return x*x + y*y + z*z;
	}
	
	float magnitute() const
	{
		return sqrt(magnitute2());
	}
	
	vec3 normalize()
	{
		float mag=magnitute();
		(*this)/=mag;
		return *this;
	}

	//doesn't change vector, just returns normalized version, hence the name
	vec3 normalized()
	{
		float mag = magnitute();
		return *this / mag;
	}
};

vec3 cross(const vec3& u, const vec3& v)
{
	return vec3
	(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

vec3 operator * (float k, const vec3& v)
{
	return v*k;
}