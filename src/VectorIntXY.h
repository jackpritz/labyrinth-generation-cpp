#pragma once

#include <cmath>
#include <iostream>
#include <string>

namespace LabyrinthGeneration
{
    /// <summary>
    /// Utility struct for XY cell coordinates
    /// </summary>
    struct VectorIntXY
    {
        int x{};
        int y{};

        VectorIntXY(int x, int y) : x{ x }, y{ y } {}

        std::string toString() const;

        bool operator==(const VectorIntXY& other) const 
        {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const VectorIntXY& other) const 
        {
            return !(*this == other); // Reuse the overloaded operator==
        }

        friend std::ostream& operator<<(std::ostream& os, const VectorIntXY& dt);
    };

    void runVectorIntXYTests();
}
