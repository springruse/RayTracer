#pragma once
#include "Object.h"
class Sphere : public Object
{
public:
	Sphere() = default;
	Sphere(const glm::vec3& position, float radius, const color3_t& color) :
		//<call Object constructor using color as parameter>,
		Object(color),
		position(position),
		radius(radius)
	{ }

	bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override {
        glm::vec3 oc = ray.origin - position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;

        float sqrtD = sqrt(discriminant);

        // first root (closest)
        float t = (-b - sqrtD) / (2 * a);
        if (t < minDistance || t > maxDistance)
        {
            // try the other root
            t = (-b + sqrtD) / (2 * a);
            if (t < minDistance || t > maxDistance)
                return false;
        }

        raycastHit.distance = t;
        raycastHit.point = ray.origin + t * ray.direction;
        raycastHit.normal = glm::normalize(raycastHit.point - position); // changes the normals of the circles
        raycastHit.color = (raycastHit.normal + glm::vec3{ 1.0f }) * 0.5f; // changes the color of the circles

        return true;
	};

public:
	glm::vec3 position;
	float radius{ 0 };
};