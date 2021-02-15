#include <cmath>

struct vec2
{
	vec2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}
	float x, y;
	
	vec2 operator + (const vec2& v) const
	{
		return vec2
		(
			x + v.x,
			y + v.y
		);
	}
	
	vec2 operator - (const vec2& v) const
	{
		return vec2
		(
			x - v.x,
			y - v.y
		);
	}
	
	float operator * (const vec2& v) const
	{
		return x*v.x + y*v.y;
	}
	
	vec2 operator * (float k) const
	{
		return vec2
		(
			k*x,
			k*y
		);
	}
	vec2 operator / (float k) const
	{
		return vec2
		(
			x/k,
			y/k
		);
	}
	
	vec2& operator += (const vec2& v)
	{
		x += v.x;
		y += v.y;
	}
	
	vec2& operator -= (const vec2& v)
	{
		x -= v.x;
		y -= v.y;
	}
	
	vec2& operator *= (float k)
	{
		x *= k;
		y *= k;
	}
	
	vec2& operator /= (float k)
	{
		x /= k;
		y /= k;
	}
	
	vec2 operator - ()
	{
		x=-x;
		y=-y;
	}
	
	float magnitute2() const
	{
		return x*x + y*y;
	}
	
	float magnitute() const
	{
		return sqrt(magnitute2());
	}
	
	vec2& normalize()
	{
		float mag=magnitute();
		(*this)/=mag;
	}

	vec2 normalized()
	{
		float mag = magnitute();
		return *this / mag;
	}
};

vec2 operator * (float k, const vec2& v)
{
	return v*k;
}
