#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <string_view>

namespace LabyrinthGeneration
{
    struct VectorXY
    {
        int x{};
        int y{};
    public:
        VectorXY(int x, int y) : x{ x }, y{ y } {}

        static int distanceSquared(const VectorXY& left, const VectorXY& right);
        std::string toString();

        friend std::ostream& operator<<(std::ostream& os, const VectorXY& dt);
    };

    void runVectorXYTests();
}
