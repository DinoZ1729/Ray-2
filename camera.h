#pragma once
#include "Ray.h"
#include "vec2.h"

//perspective camera

class Camera
{
public:
    Camera(float fov, float aspectRatio)
    {
        h = tan(fov);
        w = h * aspectRatio;
    }

    void setPosition(vec3 position) { this->position = position; }
    void setDirection(float alfa, float beta)
    {
        forward = vec3(cos(beta) * cos(alfa), cos(beta) * sin(alfa), sin(beta));
        right = vec3(-sin(alfa), cos(alfa), 0);
        up = cross(forward, right);
    }

    Ray makeRay(vec2 point) const
    {
        vec3 direction = forward + point.x * w * right + point.y * h * up;
        return Ray(position,direction.normalized());
    }
    
    vec3 getForward() { return forward; }
    vec3 getRight() { return right; }
    vec3 getUp() { return up; }

private:
    //UNIT VECTORS
    vec3 forward;
    vec3 up;
    vec3 right;

    vec3 position;

    float h, w;
};