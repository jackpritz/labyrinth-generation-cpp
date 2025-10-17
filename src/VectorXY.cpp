#include "VectorXY.h"

#include <cassert>
#include <iostream>

namespace LabyrinthGeneration
{
    double VectorXY::distanceSquared(const VectorXY& left, const VectorXY& right)
    {
        return ((left.x - right.x) * (left.x - right.x))
             + ((left.y - right.y) * (left.y - right.y));
    }

    std::string VectorXY::toString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    std::ostream& operator<<(std::ostream& os, const VectorXY& target)
    {
        os << target.toString();
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
