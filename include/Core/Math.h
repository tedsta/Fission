/*
Theodore DeRego
Copyright 2012
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <cmath>
#include <vector>

#include <SFML/System/Vector2.hpp>

/// Constant for PI.
const float PI		= 3.14159265359f;

/// Constant for reciprocal of PI.
const float RECIPROCAL_PI	= 1.0f/PI;

/// Constant for half of PI.
const float HALF_PI	= PI/2.0f;

/// 32bit Constant for converting from degrees to radians
const float DEGTORAD = PI / 180.0f;

/// 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
const float RADTODEG   = 180.0f / PI;

/// \brief Utility function to convert a radian value to degrees
/** Provided as it can be clearer to write radToDeg(X) than RADTODEG * X
\param radians	The radians value to convert to degrees.
*/
inline float radToDeg(float radians)
{
    return RADTODEG * radians;
}

/// \brief Utility function to convert a degrees value to radians
/** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
\param degrees	The degrees value to convert to radians.
*/
inline float degToRad(float degrees)
{
    return DEGTORAD * degrees;
}

/// \brief Calculate the length of a 2D vector
inline float length(sf::Vector2f a)
{
    return sqrt(a.x*a.x + a.y*a.y);
}

/// \brief Normalize a 2D vector
inline sf::Vector2f normalize(sf::Vector2f v)
{
    float l = length(v);
    return sf::Vector2f(v.x/l, v.y/l);
}

/// \brief Returns the shortest angle between two angles
inline float getShortestAngle(float a1, float a2)
{
    float shortestAngle = fabs(a1 - a2);
    while (shortestAngle > 360.f) shortestAngle -= 360.f;

    if(shortestAngle > 180)
        shortestAngle = 360 - shortestAngle;

    // Calculate direction
    int moveDir = 0;
    float behindMe = a1 - 180;
    if (behindMe < 0)
        behindMe += 360;

    if (a2 == behindMe)
        moveDir = 1; // or randomly choose
    else if ((a2 > behindMe && a2 < a1) ||
             (a1 < 180 && (a2 > behindMe ||
                                      a2 < a1)))
        moveDir = -1;
    else if ((a2 < behindMe && a2 > a1) ||
             (a1 > 180 && (a2 < behindMe ||
                                      a2 > a1)))
        moveDir= 1;

    return moveDir*shortestAngle;
}

/// \brief Returns whether or not the supplied point is within the polygon
inline bool pointInPolygon(sf::Vector2f point, std::vector <sf::Vector2f> coords)
{
    for (unsigned int i=0;i<coords.size()-2;i+=2)
    {
        if (((point.y-coords[i+1].y)*(coords[i+2].y-coords[i].y) - (point.x-coords[i].x)*(coords[i+3].x-coords[i+1].x))<0)
        {
            return false;
        }
    }
    //The last test is special
    unsigned int j = coords.size();
    if (((point.y-coords[j-1].y)*(coords[0].y-coords[j-2].y) - (point.x-coords[j-2].x)*(coords[1].x-coords[j-1].x))<0)
    {
        return false;
    }
    return true;
}

#endif // MATH_H_INCLUDED
