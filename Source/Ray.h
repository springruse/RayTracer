#pragma once
#include <glm/glm.hpp>

struct ray_t {

	ray_t(glm::vec3 origin = { 1,0,0 }, glm::vec3 direction = { 0,90,0 }) : origin(origin), direction(direction) {};
	~ray_t() = default;

	glm::vec3 at(float t) const {
		return origin + t * direction;
	}
	glm::vec3 operator* (float t) const {
		return t* direction;
	}
	glm::vec3 origin;
	glm::vec3 direction;
};

struct raycastHit_t {
	glm::vec3 point;
	glm::vec3 normal;
	float direction;

};