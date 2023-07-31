#include "Light.hpp"

using namespace lights;

Light::Light(glm::vec3 pos, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong)
	: pos(pos), color(color), ambientColor(ambientColor) {

	this->multiplier = multiplier;
	this->ambientStr = ambientStr;
	this->specStr = specStr;
	this->specPhong = specPhong;
}

void Light::setMultiplier(float multiplier) {
	this->multiplier = multiplier;

	if (this->multiplier <= 0.f) this->multiplier = 0.01f;
}

void Light::setPos(glm::vec3 pos) {
	this->pos = pos;
}

void Light::setColor(glm::vec3 color) {
	this->color = color;
}

glm::vec3 Light::getPos() {
	return this->pos;
}

glm::vec3 Light::getColor() {
	return this->color;
}

float Light::getMultiplier() {
	return this->multiplier;
}

float Light::getAmbientStr() {
	return this->ambientStr;
}

glm::vec3 Light::getAmbientColor() {
	return this->ambientColor;
}

float Light::getSpecStr() {
	return this->specStr;
}

float Light::getSpecPhong() {
	return this->specPhong;
}