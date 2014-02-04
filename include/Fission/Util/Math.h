/*
Theodore DeRego
Copyright 2012
*/

#ifndef FISSION_MATH_H_INCLUDED
#define FISSION_MATH_H_INCLUDED

#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace fsn
{
    // Constant for PI.
    const float PI = 3.14159265359f;

    // Constant for reciprocal of PI.
    const float RECIPROCAL_PI = 1.0f/PI;

    // Constant for half of PI.
    const float HALF_PI	= PI/2.0f;

    // 32bit Constant for converting from degrees to radians
    const float DEGTORAD = PI / 180.0f;

    // 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
    const float RADTODEG = 180.0f / PI;

    // Smallest float
    const float EPSILON = 0.0001f;

    /// \brief Comparison with tolerance of EPSILON
    inline bool equal(float a, float b )
    {
        // <= instead of < for NaN comparison safety
        return std::abs(a - b) <= EPSILON;
    }

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

    /// \brief 2D vector dot product
    inline float dot(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x*b.x + a.y*b.y;
    }

    inline sf::Vector2f cross(const sf::Vector2f& v, float a)
    {
      return sf::Vector2f( a * v.y, -a * v.x );
    }

    inline sf::Vector2f cross(float a, const sf::Vector2f& v)
    {
      return sf::Vector2f( -a * v.y, a * v.x );
    }

    /// \brief 2D vector cross product between two vectors
    inline float cross(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x*b.y - a.y*b.x;
    }

    /// \brief Square a number
    inline float sqr(float f)
    {
        return f*f;
    }

    /// \brief Calculate the squared length of a 2D vector
    inline float lengthSqr(const sf::Vector2f& v)
    {
        return v.x*v.x + v.y*v.y;
    }

    /// \brief Calculate the length of a 2D vector
    inline float length(const sf::Vector2f& v)
    {
        return sqrt(v.x*v.x + v.y*v.y);
    }

    /// \brief Normalize a 2D vector
    inline sf::Vector2f normalize(const sf::Vector2f& v)
    {
        float l = length(v);
        return !equal(l, 0.f) ? sf::Vector2f(v.x/l, v.y/l) : sf::Vector2f(0, 0);
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
        else if ((a2 > behindMe && a2 < a1) || (a1 < 180 && (a2 > behindMe || a2 < a1)))
            moveDir = -1;
        else if ((a2 < behindMe && a2 > a1) || (a1 > 180 && (a2 < behindMe || a2 > a1)))
            moveDir= 1;

        return moveDir*shortestAngle;
    }

    inline bool biasGreaterThan( float a, float b )
    {
        const float k_biasRelative = 0.95f;
        const float k_biasAbsolute = 0.01f;
        return a >= b * k_biasRelative + a * k_biasAbsolute;
    }
}


#endif // FISSION_MATH_H_INCLUDED
