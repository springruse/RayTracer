#pragma once
#include "Object.h"
#include "Material.h"
#include <memory>
class Sphere : public Object
{
public:
	Sphere() = default;
	Sphere(const Transform& transform,float radius, std::shared_ptr<Material> material) :
		Object(transform, material),
		radius(radius)
	{ }

	bool Hit(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override {
        glm::vec3 oc = ray.origin - transform.position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(ray.direction, oc);
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
        raycastHit.normal = glm::normalize(raycastHit.point - transform.position); // changes the normals of the circles
        raycastHit.material = material.get();
        //raycastHit.color = (raycastHit.normal + glm::vec3{ 1.0f }) * 0.5f; // changes the color of the circles

        return true;
	};

public:
	float radius{ 0 };
};