#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <string_view>

namespace LabyrinthGeneration
{
    // Utility struct for XY coordinates
    struct VectorXY
    {
        double x{};
        double y{};

        VectorXY(double x, double y) : x{ x }, y{ y } {}

        static double distanceSquared(const VectorXY& left, const VectorXY& right);
        std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const VectorXY& dt);
    };

    void runVectorXYTests();
}
