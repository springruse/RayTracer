#include "Camera.h"


void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
	this->eye = eye;
	this->forward = glm::normalize(target - eye);
	this->right = glm::normalize(glm::cross(this->forward, up));
	this->up = glm::cross(this->right, this->forward);

	// create camera axis
	//this->forward = normalized direction vector (target <-- eye)
	//this->right = normalized vector from the cross product of the forward and up vector
	//this->up = normalized vector from the cross product of the right and forward vector

	CalculateViewPlane();
}

ray_t Camera::GetRay(const glm::vec2& uv) const {
	ray_t ray;
	ray.origin = eye;
	ray.direction = (lowerLeft + (horizontal * uv.x) + (vertical * uv.y)) - eye;
	//ray.origin = camera eye
	//ray.direction = lower left position + horizontal vector * uv.x + vertical vector * uv.y - camera eye;

	return ray;
}

void Camera::CalculateViewPlane() {
	//float theta = convert fov (degrees) to radians
	float theta = glm::radians(fov);
	float halfheight = glm::tan(theta * 0.5f);
	float halfWidth = halfheight * aspectRatio;

	horizontal = right * (halfWidth * 2.0f);
	vertical = up * (halfheight * 2.0f);
	lowerLeft = eye - (horizontal * 0.5f) - (vertical * 0.5f) + forward;
	//float halfHeight = trig function that is opposite over adjacent, use half theta as parameter
	//float halfWidth = scale halfHeight by aspect ratio

	//horizontal = right vector * width (how do you get full width from half width?)
	//vertical = up vector * height (how do you get full height from half height?)

	//lowerLeft = eye - (half horizontal) - (half vertical) + forward;
}

