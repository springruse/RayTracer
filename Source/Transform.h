#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform {
	glm::vec3 position{ 0, 0, 0 };
	glm::quat rotation{ 1, 0, 0, 0 };
	glm::vec3 scale{ 1, 1, 1 };

	Transform() = default;
	Transform(const glm::vec3& position, const glm::quat& rotation = glm::quat{}, glm::vec3 scale = glm::vec3{ 1, 1, 1 }) :
		position{ position },
		rotation{ rotation },
		scale{ scale }
	{
	}

	glm::vec3 right() const { return rotation * glm::vec3{ 1, 0, 0 }; }
	glm::vec3 up() const { return rotation * glm::vec3{ 0, 1, 0 }; }
	glm::vec3 forward() const { return rotation * glm::vec3{ 0, 0, -1 }; }

	glm::mat4 getMatrix() const {
		// create 4x4 transform matrix
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = transform * glm::mat4_cast(rotation);
		transform = glm::scale(transform, scale);

		return transform;
	}
};