#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Object.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++)	{
			// set pixel (x,y) coordinates)
			glm::vec2 pixel{ x, y };
			// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
			glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
			// flip the y value (bottom = 0, top = 1)
			point.y = 1 - point.y;

			// get ray from camera
			ray_t ray = camera.GetRay(point);
			// trace ray
			raycastHit_t raycastHit;
			// 0 = min ray distance, 100 = max ray distance
			color3_t color = Trace(ray, 0, 100, raycastHit);

			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {

	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))	{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// check if ray hit object
	if (rayHit)	{
		// get material color of hit object
		color3_t color = raycastHit.color;
		return color;
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	
	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
