#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <string_view>

#include "VectorXY.h"

namespace LabyrinthGeneration
{
    std::ostream& operator<<(std::ostream& os, const VectorXY& target)
    {
        os << "(" << target.x << ", " << target.y << ")";
        return os;
    }

    int VectorXY::distanceSquared(const VectorXY& left, const VectorXY& right)
    {
        return static_cast<int>(pow(left.x - right.x, 2))
             + static_cast<int>(pow(left.y - right.y, 2));
    }

    std::string VectorXY::toString()
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    void runVectorXYTests()
    {
        VectorXY zero{ 0, 0 };
        VectorXY vec{ 3, 4 };

        assert(25 == vec.distanceSquared(zero, vec));

        std::string vecString{ vec.toString() };
        assert("(3, 4)" == vec.toString());
    }
}
