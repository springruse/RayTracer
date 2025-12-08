#pragma once
#include "Color.h"
#include "Object.h"
#include <vector>
#include <memory>

class Scene
{
public:
	Scene() = default;

	//void Render(class Framebuffer& framebuffer, const class Camera& camera);
	void Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10);
	void AddObject(std::unique_ptr<Object> object);
	void SetSky(const color3_t& skyBottom, const color3_t& skyTop) {
		this->skyBottom = skyBottom;
		this->skyTop = skyTop;
	}

private:
	// trace the ray into the scene
	color3_t Trace(const struct ray_t& ray, float minDistance, float maxDistance, int maxDepth = 5);

	
private:
	color3_t skyBottom{ 1 };
	color3_t skyTop{ 0.5f, 0.7f, 1.0f };
	std::vector<std::unique_ptr<Object>> objects;
};