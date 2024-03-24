#pragma once
#ifndef UAV_H
#define UAV_H

#include <cmath>

class UAV {
public:
    // Constructors
    UAV(int _num, double _x, double _y, double _azimuth, double _z, double _v, double _r);
    UAV();

    // Accessors
    int getNum() const;
    double getX() const;
    double getY() const;
    double getAzimuth() const;
    double getZ() const;
    double getV() const;
    double getR() const;
    bool isAzimuthUpdated() const; // Getter for azimuthUpdated
    bool isStandbyModeFlag() const; // Getter for standbyModeFlag

    // Mutators
    void setNum(int _num);
    void setX(double _x);
    void setY(double _y);
    void setAzimuth(double _azimuth);
    void setZ(double _z);
    void setV(double _v);
    void setR(double _r);
    void setAzimuthUpdated(bool updated); // Setter for azimuthUpdated
    void setStandbyModeFlag(bool flag); // Setter for standbyModeFlag


    /**
    * This method updates the UAV's position after flying in a straight line for a specified duration.
    * The flight direction is determined by the UAV's current azimuth (orientation angle),
    * and the flight's distance is calculated from its velocity and the flight duration.
    *
    * @param duration The time in seconds for which the UAV moves towards the target.
    */
    void linearFlightUpdate(double duration);

    /**
    * Navigates the UAV towards a specified target location.
    *
    * This method adjusts the UAV's position and azimuth to either move directly towards the target
    * or circle around it if within a certain radius, based on its current position, velocity, and turning radius.
    *
    * @param duration The time in seconds for which the UAV moves towards the target.
    * @param destX The X coordinate of the target location.
    * @param destY The Y coordinate of the target location.
     */
    void navigateToTarget(double duration, double destX, double destY);

    /**
    *Places the UAV in standby mode, circling around a specified target location.
    *
    * This method calculates the UAV's position and azimuth to circle around the specified target location
    * for the given duration, based on its current position, velocity, and turning radius.
    *
    * @param destX The X coordinate of the target location around which the UAV will circle.
    * @param destY The Y coordinate of the target location around which the UAV will circle.
    * @param duration The time in seconds for which the UAV will circle around the target location.
    */
    void standbyMode(double destX, double destY, double duration);

private:
    int num;
    double x;
    double y;
    double azimuth;
    double z;
    double v;
    double r;

    bool azimuthUpdated = false; 
    bool standbyModeFlag = false;

};

#endif // UAV_H

