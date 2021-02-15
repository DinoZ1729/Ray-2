/*
* 
* RAYTRACING 2
* 
* Autor: Dino1729
* 
* 9. february, 2021.
* 
* Inspired by HillsoftCode:
* https://www.youtube.com/playlist?list=PLAqGIYgEAxrUO6ODA0pnLkM2UOijerFPv
*/


#include <iostream>
#include <cstdlib>
#include <ctime>
#include "screen.h"
#include "camera.h"
#include "shape.h"

const float PI = 3.141592f;

float clamp(float f, float min, float max)
{
	if (f < min)	return min;
	if (f > max)	return max;
	return f;
}

class Light
{
public:
	void push_back(vec3 source, float intensity)
	{
		this->source.push_back(source);
		this->intensity.push_back(intensity);
		n++;
	}

	float enlighten(const Intersection& intersection, Shape* scene) const
	{
		float light = 0.0f;
		for (int i = 0; i < n; i++)
		{
			Ray toLight(intersection.point(), (source[i] - intersection.point()).normalized());
			float diffused = 0.0f;
			if (!scene->doesIntersect(toLight))
				diffused += intensity[i]*intersection.shape->getBrightness() * toLight.direction * intersection.normal;
			if (diffused >= 0.0f)
				light += diffused;
		}

		return light;
	}
private:
	std::vector<vec3> source;
	std::vector<float> intensity;
	int n = 0;
};

float RayTrace(Ray ray, Shape* scene, const Light& lights, int depth = 4)
{
	//escape
	if (depth == 0)
		return 0.0f;

	Intersection intersection(ray);
	if (scene->doesIntersect(ray))
	{
		scene->intersect(intersection);

		//diffusion
		float diffused = lights.enlighten(intersection, scene);

		//reflection
		vec3 reflectedVec = ray.direction - 2 * (ray.direction * intersection.normal) * intersection.normal;
		float reflected = RayTrace(Ray(intersection.point(), reflectedVec), scene, lights, depth-1);

		float k = intersection.shape->getReflection();
		float result = k * reflected + (1 - k) * diffused;
		return  clamp(result, 0.0f, 1.0f);
	}
	else
	{
		return 0.0f;
	}
}

float random(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));;
}

int main()
{
	srand(static_cast <unsigned> (time(0)));

	Screen scr;

	Camera camera
	(
		25.0f * PI / 180.0f,
		(float)WIDTH / (float)HEIGHT
	);
	vec3 position = vec3(-10.0f, 0.0f, 2.5f);
	float alfa = 0.0f, beta = 0.0f;

	camera.setPosition(vec3(0.0f, 0.0f, 0.0f));
	camera.setDirection(alfa, beta);

	float intensity = 1.5f;
	float brightness = 1.5f;
	float reflection = 0.3f;

	//LIGHT

	Light lights;
	lights.push_back(vec3(+0.0f, +1000.0f, 1002.5f), intensity);

	ShapeSet scene;

	//FLOOR

	Plane floor(
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	);
	floor.setBrightness(2.0f);
	floor.setReflection(0.7f);
	scene.push_back(&floor);

	//SPHERES

	const int n = 10;
	Sphere spheres[n];

	for (int i = 0; i < n; i++)
	{
		float x = random(-5.0f, 5.0f);
		float y = random(-5.0f, 5.0f);
		float z = random(+0.4f, 1.5f);
		spheres[i] = Sphere(vec3(x,y,z),z);
		spheres[i].setBrightness(brightness);
		spheres[i].setReflection(reflection);
		scene.push_back(&spheres[i]);
	}

	//MAIN LOOP
	while (true)
	{
		HANDLE hInput;
		DWORD prev_mode;
		hInput = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hInput, &prev_mode);
		SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);

		//KEYBOARD INPUT

		if (GetKeyState(0x57) & 0x8000)		position += 8 * (1.0f / 60.0f) * camera.getForward();		//W
		if (GetKeyState(0x41) & 0x8000)		position -= 8 * (1.0f / 60.0f) * camera.getRight();			//A
		if (GetKeyState(0x53) & 0x8000)		position -= 8 * (1.0f / 60.0f) * camera.getForward();		//S
		if (GetKeyState(0x44) & 0x8000)		position += 8 * (1.0f / 60.0f) * camera.getRight();			//D

		if (GetKeyState(VK_SHIFT) & 0x8000)	position.z -= 8 * (1.0f / 60.0f);
		if (GetKeyState(VK_SPACE) & 0x8000)	position.z += 8 * (1.0f / 60.0f);

		//CAMERA MOVEMENT
		if (GetKeyState(VK_UP) & 0x8000)	beta += 1.0f / 120.0f * (2.0f * PI);
		if (GetKeyState(VK_LEFT) & 0x8000)	alfa -= 1.0f / 120.0f * (2.0f * PI);
		if (GetKeyState(VK_DOWN) & 0x8000)	beta -= 1.0f / 120.0f * (2.0f * PI);
		if (GetKeyState(VK_RIGHT) & 0x8000)	alfa += 1.0f / 120.0f * (2.0f * PI);

		if (GetKeyState(VK_ESCAPE) & 0x8000)
		{
			//EXIT THE PROGRAM
			//...
			return 0;
		}

		camera.setPosition(position);
		camera.setDirection(alfa, beta);

		//RAY TRACING
		for (int x = 0; x < WIDTH; x++)
		{
			for (int y = 0; y < HEIGHT; y++)
			{
				vec2 screenCoord((2.0f * x) / WIDTH - 1.0f, -(2.0f * y) / HEIGHT + 1.0f);
				Ray ray = camera.makeRay(screenCoord);
				float brightness = 0.5;
				brightness = RayTrace(ray, &scene, lights);
				scr.DrawPoint(y, x, (byte)(brightness * 255));
			}
		}

		//DRAWING
		scr.Draw();
	}
	return 0;
}
