#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "VectorXY.h"

namespace LabyrinthGeneration
{
    double VectorXY::distanceSquared(const VectorXY& left, const VectorXY& right)
    {
        return pow(left.x - right.x, 2)
             + pow(left.y - right.y, 2);
    }

    std::string VectorXY::toString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    std::ostream& operator<<(std::ostream& os, const VectorXY& target)
    {
        os << "(" << target.x << ", " << target.y << ")";
        return os;
    }

    void runVectorXYTests()
    {
        VectorXY zero{ 0, 0 };
        VectorXY vec{ 3.0, 4.0 };

        assert(25.0 == vec.distanceSquared(zero, vec));

        std::string vecString{ vec.toString() };
        assert("(3.000000, 4.000000)" == vec.toString());
    }
}
