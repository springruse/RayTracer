#pragma once
#include <glm/glm.hpp>

struct Ray {
public:
	Ray(glm::vec3 origin = { 1,0,0 }, glm::vec3 direction = { 0,90,0 }) {};
	~Ray(){};

	glm::vec3 at(float t) const {
		return origin + t * direction;
	}
	glm::vec3 operator* (float t) const {
		return t* direction;
	}
public:
	glm::vec3 origin = { 0,0,0 };
	glm::vec3 direction = { 0,0,0 };
};