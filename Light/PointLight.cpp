#include "PointLight.hpp"

using namespace lights;

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong)
	: Light(pos, color, multiplier, ambientStr, ambientColor, specStr, specPhong) {

	this->ambientColor = color;
}