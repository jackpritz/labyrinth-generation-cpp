#include <cassert>

#include "VectorIntXY.h"

namespace LabyrinthGeneration
{
    std::string VectorIntXY::toString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    std::ostream& operator<<(std::ostream& os, const VectorIntXY& target)
    {
        os << "(" << target.x << ", " << target.y << ")";
        return os;
    }

    void runVectorIntXYTests()
    {
        VectorIntXY zero{ 0, 0 };
        VectorIntXY vec{ 3, 4 };
        VectorIntXY vec2{ 3, 4 };

        std::string vecString{ vec.toString() };
        assert("(3, 4)" == vec.toString());
        assert(zero != vec);
        assert(vec == vec2);
    }
}