#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Object.h"
#include "glm/glm.hpp"
#include "Random.h"
#include "material.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++) {
			// color will be accumulated with ray trace samples
			color3_t color{ 0 };
			// multi-sample for each pixel
			for (int i = 0; i < numSamples; i++) {
				// set pixel (x,y) coordinates)
				glm::vec2 pixel{ x, y };
				// add random value (0-1) to pixel valie, each sample should be a little different
				pixel += glm::vec2(random::getReal(0.0f, 1.0f), random::getReal(0.0f, 1.0f));
				// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
				glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1 - point.y;

				// get ray from camera
				ray_t ray = camera.GetRay(point);
				// trace ray
				color += Trace(ray, 0, 100);
			}
			// get average color = (color / number samples)
			color /= numSamples;
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

//void Scene::Render(Framebuffer& framebuffer, const Camera& camera) {
//	// trace ray for every framebuffer pixel
//	for (int y = 0; y < framebuffer.height; y++) {
//		for (int x = 0; x < framebuffer.width; x++)	{
//			// set pixel (x,y) coordinates)
//			glm::vec2 pixel{ x, y };
//			// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height }
//			glm::vec2 point = pixel / glm::vec2{ framebuffer.width, framebuffer.height };
//			// flip the y value (bottom = 0, top = 1)
//			point.y = 1 - point.y;
//
//			// get ray from camera
//			ray_t ray = camera.GetRay(point);
//			// trace ray
//			
//			// 0 = min ray distance, 100 = max ray distance
//			color3_t color = Trace(ray, 0, 100);
//
//			framebuffer.DrawPoint(x, y, ColorConvert(color));
//		}
//	}
//}

void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, int maxDepth) {

	if (maxDepth == 0) return glm::vec3({0,0,0});
	bool rayHit = false;
	float closestDistance = maxDistance;
	raycastHit_t raycastHit;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))	{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;

		}
	}

	if (rayHit) {
		color3_t attenuation;
		ray_t scattered;
		// get raycast hit matereial, get material color and scattered ray 
		if (raycastHit.material->Scatter(ray, raycastHit, attenuation, scattered)) {
			// trace scattered ray, final color will be the product of all the material colors
			return attenuation * Trace(scattered, minDistance, maxDistance, maxDepth - 1);
		}
		else {
			return raycastHit.material->GetEmissive();
		}
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	
	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
