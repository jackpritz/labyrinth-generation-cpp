#pragma once

#include <iostream>
#include <string>

namespace LabyrinthGeneration
{
    /// <summary>
    /// Represents a 3d mathematical vector in world space.
    /// Could represent a position or a direction.
    /// </summary>
    struct Vector3
    {
        double x;
        double y;
        double z;

        Vector3(double x, double y, double z) : x{ x }, y{ y }, z{ z } {}

        std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const Vector3& dt);
    };

    void runVector3Tests();
}