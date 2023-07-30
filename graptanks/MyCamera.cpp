#include "MyCamera.hpp"

using namespace cameras;

MyCamera::MyCamera() {
	this->persPos = glm::vec3(0, 0, -75);
	this->worldUp = glm::vec3(0, 1, 0);
	this->cameraCenter = glm::vec3(0, 0, 0);
    this->orthoPos = glm::vec3(0, 10, 0);
    this->activePos = this->persPos;
    this->orthoActive = false;

	this->orthographic = this->initializeOrthographic();
	this->perspective = this->initializePerspective();

	this->activeProjection = this->perspective;
}

void MyCamera::move(glm::vec3 movement) {}

glm::mat4 MyCamera::initializeOrthographic() {

	glm::mat4 orthographic = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, -50.0f, 200.0f);
	return orthographic;
}

glm::mat4 MyCamera::initializePerspective() {

	glm::mat4 perspective = glm::perspective(glm::radians(90.0f), 200.0f / 200.0f, -1.0f, 1.0f);
	return perspective;
}

void MyCamera::enableOrthographic() {
	this->activeProjection = this->orthographic;
    this->orthoActive = true;
}

void MyCamera::enablePerspective() {
	this->activeProjection = this->perspective;
    this->orthoActive = false;
}

void MyCamera::run(GLuint* shaderProgram, double x_cursor_pos, double y_cursor_pos) {
    float radius = 30.f; // Distance of the spawned object to the camera.=
    float theta_tot = -90.f; // Max angle on the left or bottom side of the screen.
    glm::vec2 mousePos = glm::vec2(x_cursor_pos - (1280.0f / 2), y_cursor_pos - (720.0f / 2));
    float yaw = glm::radians((mousePos.x / (1280.0f / 2)) * theta_tot);
    float pitch = glm::radians((mousePos.y / (720.0f / 2)) * theta_tot);

    float xAxisRot = radius * sin(yaw) * cos(pitch);
    float yAxisRot = radius * sin(pitch);
    float zAxisRot = radius * cos(yaw) * cos(pitch);

    if (orthoActive) {
        this->activePos = this->orthoPos;
        this->worldUp = glm::vec3(0, 0, 1);
    } else {
        this->activePos = this->persPos;
        this->worldUp = glm::vec3(0, 1, 0);
        this->cameraCenter = glm::vec3(xAxisRot, yAxisRot, zAxisRot);
    }

    glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), this->activePos * -1.0f);

    glm::vec3 F = (this->cameraCenter - this->activePos);
    F = glm::normalize(F);

    glm::vec3 R = glm::normalize(glm::cross(F, this->worldUp));
    glm::vec3 U = glm::cross(R, F);

    glm::mat4 cameraOrientation = glm::mat4(1.0f);

    cameraOrientation[0][0] = R.x;
    cameraOrientation[1][0] = R.y;
    cameraOrientation[2][0] = R.z;

    cameraOrientation[0][1] = U.x;
    cameraOrientation[1][1] = U.y;
    cameraOrientation[2][1] = U.z;

    cameraOrientation[0][2] = -F.x;
    cameraOrientation[1][2] = -F.y;
    cameraOrientation[2][2] = -F.z;

    if (orthoActive) {
        this->viewMatrix = glm::lookAt(this->activePos, this->cameraCenter, this->worldUp);
    }
    else {
       this->viewMatrix = cameraOrientation * cameraPosMatrix; 
    }
    
    unsigned int viewLoc = glGetUniformLocation(*shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(*shaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(this->activeProjection));
}

glm::mat4 MyCamera::getViewMatrix() {
    return this->viewMatrix;
}
glm::vec3 MyCamera::getCameraPos() {
    return this->activePos;
}