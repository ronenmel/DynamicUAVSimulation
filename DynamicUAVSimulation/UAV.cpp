#include "UAV.h"

// Constructor with parameters
UAV::UAV(int _num, double _x, double _y, double _azimuth, double _z, double _v, double _r)
    : num(_num), x(_x), y(_y), azimuth(_azimuth), z(_z), v(_v), r(_r) {}

// Default constructor
UAV::UAV() : num(0), x(0.0), y(0.0), azimuth(0.0), z(0.0), v(0.0), r(0.0) {}

// Accessors
int UAV::getNum() const {
    return num;
}

double UAV::getX() const {
    return x;
}

double UAV::getY() const {
    return y;
}

double UAV::getAzimuth() const {
    return azimuth;
}

double UAV::getZ() const {
    return z;
}

double UAV::getV() const {
    return v;
}

double UAV::getR() const {
    return r;
}

bool UAV::isAzimuthUpdated() const {
    return azimuthUpdated;
}


bool UAV::isStandbyModeFlag() const {
    return standbyModeFlag;
}


// Mutators
void UAV::setNum(int _num) {
    num = _num;
}

void UAV::setX(double _x) {
    x = _x;
}

void UAV::setY(double _y) {
    y = _y;
}

void UAV::setAzimuth(double _azimuth) {
    azimuth = _azimuth;
}

void UAV::setZ(double _z) {
    z = _z;
}

void UAV::setV(double _v) {
    v = _v;
}

void UAV::setR(double _r) {
    r = _r;
}

void UAV::setAzimuthUpdated(bool updated) {
    azimuthUpdated = updated;
}

void UAV::setStandbyModeFlag(bool flag) {
    standbyModeFlag = flag;
}



//This method Updates UAV's position for straight-line flight over a given duration using current velocity and azimuth.
void UAV::linearFlightUpdate(double duration) {

    // Calculate new position based on velocity and duration
    double newX = this->getX() + this->getV() * cos(this->getAzimuth()) * duration;
    double newY = this->getY() + this->getV() * sin(this->getAzimuth()) * duration;

    // Update UAV position
    this->setX(newX);
    this->setY(newY);
}

//Places the UAV in standby mode, circling around a specified target location.
void UAV::standbyMode(double destX, double destY, double duration) {
    // Define pi
    const double pi = 3.14159265358979323846;

    // Calculate the angular speed for circular movement
    double angularSpeed = getV() / getR(); // Angular speed (radians per second) = linear speed / radius

    // Calculate the change in azimuth after the given duration (clockwise direction)
    double azimuthChange = -angularSpeed * duration;

    // Calculate the new azimuth for circular movement
    double newAzimuth = getAzimuth() + azimuthChange;

    // Adjust the new azimuth to ensure it stays within the range [0, 2 * pi)
    while (newAzimuth < 0)
        newAzimuth += 2 * pi;
    while (newAzimuth >= 2 * pi)
        newAzimuth -= 2 * pi;


    // Calculate the new position along the circular path
    double newX = destX - getR() * cos(newAzimuth);
    double newY = destY - getR() * sin(newAzimuth);

    // Update UAV position and azimuth
    setX(newX);
    setY(newY);
    setAzimuth(newAzimuth);
}

// Navigates the UAV towards a specified target location.
void UAV::navigateToTarget(double duration, double destX, double destY) {
    // Calculate distance to target
    double deltaX = destX - getX();
    double deltaY = destY - getY();
    double distanceToDest = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Calculate the azimuth towards the destination
    double azimuthToDest = std::atan2(deltaY, deltaX);

    // Check if we are currently at a distance equal to the radius from the destination
    if (distanceToDest == getR()|| standbyModeFlag) {
        // If so, move in a circle
        standbyModeFlag = true;
        standbyMode(destX, destY, duration);
    }
    // Check if we are inside the circle (a small distance from the radius to the center)
    else if (distanceToDest < getR()) {

        // Calculate time to reach the destination
        double timeToDest = distanceToDest / getV();
        double timeToRadius =getR()/ getV();

        // Update UAV azimuth towards the destination if it hasn't been updated before
        if (!azimuthUpdated) {
            setAzimuth(azimuthToDest);
            azimuthUpdated = true;
        }
        if ((timeToDest+ timeToRadius) < duration) {
            azimuthUpdated = false;
            // Move towards the radius linearly
            linearFlightUpdate(timeToDest);

            // Calculate remaining duration for circling around the center
            double remainingDuration = duration - (timeToDest+ timeToRadius);

            // Perform circular movement around the center with the remaining duration
            standbyMode(destX, destY, remainingDuration);
        }
        else{
            linearFlightUpdate(duration);
        }
    }
    else {
        double finalX = getX() + getV() * cos(atan2(deltaY, deltaX)) * duration;
        double finalY = getY() + getV() * sin(atan2(deltaY, deltaX)) * duration;
        double finalDistanceToDest = sqrt(pow(destX - finalX, 2) + pow(destY - finalY, 2));

        // Check if the UAV is skipping the destination Or if the UAV is inside the radius of destination
        if (finalDistanceToDest < getR() || finalDistanceToDest - getR() > distanceToDest) {
            // Calculate the time to reach the radius from the current position
            double timeToRadiusFromCenter = (abs(getR() - distanceToDest)) / getV();

            // Update UAV azimuth
            setAzimuth(azimuthToDest);

            // Move towards the radius linearly
            linearFlightUpdate(timeToRadiusFromCenter);

            // Calculate remaining duration for circling around the center
            double remainingDuration = duration - timeToRadiusFromCenter;

            standbyModeFlag = true;

            // Perform circular movement around the center with the remaining duration
            standbyMode(destX, destY, remainingDuration);
        }
        else {
            // Update UAV azimuth
            setAzimuth(azimuthToDest);

            // Move towards the radius linearly
            linearFlightUpdate(duration);
        }
    }
}







