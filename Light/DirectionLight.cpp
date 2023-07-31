#include "DirectionLight.hpp"

using namespace lights;

DirectionLight::DirectionLight(glm::vec3 pos, glm::vec3 pointsAt, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong) 
	: Light(pos, color, multiplier, ambientStr,
		ambientColor, specStr, specPhong)
{
	// Compute Direction 
	this->direction = this->pos - this->pointsAt;
}

glm::vec3 DirectionLight::getDirection() {
	return this->direction;
}