#include "Player.hpp"

using namespace players;

Player::Player(Model* tank) {
    // Reference to the player tank from main.
    this->tank = tank;

    // ----- Player Controls ----- //
    drivingTank = true;        // using 3rd person perspective camera
    usingBinoculars = false;   // using 1st person perspective camera
    usingOverhead = false;
    resetView = false;

    // Tank Controls
    movingForward = false;     // W
    movingBackward = false;    // S
    turningLeft = false;       // A
    turningRight = false;      // D

    // Binocular Controls
    lookingUp = false;         // W
    lookingDown = false;       // S
    lookingLeft = false;       // A
    lookingRight = false;      // D
    zoomingIn = false;         // Q
    zoomingOut = false;        // E

    // Drone Controls
    droningForward = false;    // W
    droningBackward = false;   // S
    droningRight = false;      // D
    droningLeft = false;       // A
    // --------------------------- //

    // Headlights Data
    adjustheadlightsIntensity = false;
    headlightsIntensity = 1;
    lowIntensity = 100.f;
    medIntensity = 500.f;
    highIntensity = 1000.f;

    // Event flag for jumping.
    this->jumping = false;
}

// Very straightforward getters and setters for all the event flags in the Player object.
bool Player::isDrivingTank() {
    return drivingTank;
}

bool Player::isUsingBinoculars() {
    return usingBinoculars;
}

bool Player::isUsingDrone() {
    return usingOverhead;
}

bool Player::isResettingView() {
    return resetView;
}

bool Player::isMovingForward() {
    return movingForward;
}

bool Player::isMovingBackward() {
    return movingBackward;
}

bool Player::isTurningLeft() {
    return turningLeft;
}

bool Player::isTurningRight() {
    return turningRight;
}

bool Player::isLookingUp() {
    return lookingUp;
}

bool Player::isLookingDown() {
    return lookingDown;
}

bool Player::isLookingLeft() {
    return lookingLeft;
}

bool Player::isLookingRight() {
    return lookingRight;
}

bool Player::isZoomingIn() {
    return zoomingIn;
}

bool Player::isZoomingOut() {
    return zoomingOut;
}

bool Player::isDroningForward() {
    return droningForward;
}

bool Player::isDroningBackward() {
    return droningBackward;
}

bool Player::isDroningRight() {
    return droningRight;
}

bool Player::isDroningLeft() {
    return droningLeft;
}

bool Player::isAdjustingHeadlights() {
    return adjustheadlightsIntensity;
}

int Player::getHeadlightsIntensity() {
    return headlightsIntensity;
}

float Player::getLowIntensity() {
    return lowIntensity;
}

float Player::getMedIntensity() {
    return medIntensity;
}

float Player::getHighIntensity() {
    return highIntensity;
}

void Player::setDrivingTank(bool x) {
    this->drivingTank = x;
}

void Player::setUsingBinoculars(bool x) {
    this->usingBinoculars = x;
}

void Player::setUsingDrone(bool x) {
    this->usingOverhead = x;
}

void Player::setResettingView(bool x) {
    this->resetView = x;
}

void Player::setMovingForward(bool x) {
    this->movingForward = x;
}

void Player::setMovingBackward(bool x) {
    this->movingBackward = x;
}

void Player::setTurningLeft(bool x) {
    this->turningLeft = x;
}

void Player::setTurningRight(bool x) {
    this->turningRight = x;
}

void Player::setLookingUp(bool x) {
    this->lookingUp = x;
}

void Player::setLookingDown(bool x) {
    this->lookingDown = x;
}

void Player::setLookingLeft(bool x) {
    this->lookingLeft = x;
}

void Player::setLookingRight(bool x) {
    this->lookingRight = x;
}

void Player::setZoomingIn(bool x) {
    this->zoomingIn = x;
}

void Player::setZoomingOut(bool x) {
    this->zoomingOut = x;
}

void Player::setDroningForward(bool x) {
    this->droningForward = x;
}

void Player::setDroningBackward(bool x) {
    this->droningBackward = x;
}

void Player::setDroningRight(bool x) {
    this->droningRight = x;
}

void Player::setDroningLeft(bool x) {
    this->droningLeft = x;
}

void Player::setAdjustingHeadlights(bool x) {
    this->adjustheadlightsIntensity = x;

    // If adjusting headlights, increment.
    if (x) {
        headlightsIntensity++;

        // if the intensity is past the 3rd level, reset it back to the lowest level.
        if (headlightsIntensity >= 4) headlightsIntensity = 1;

        std::cout << "[F Key Pressed]: Adjusting Point Light Intensity to " << headlightsIntensity << std::endl;
    }
}

void Player::setModel(Model* tank) {
    this->tank = tank;
}

bool Player::isJumping() {
    return this->jumping;
}

void Player::setJumping(bool x) {
    this->jumping = x;
}