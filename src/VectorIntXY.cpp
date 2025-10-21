#include "VectorIntXY.h"

#include <cassert>
#include <iostream>

namespace LabyrinthGeneration
{
    std::string VectorIntXY::toString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    VectorIntXY operator+(const VectorIntXY& left, const VectorIntXY& right)
    {
        return VectorIntXY{
            left.x + right.x,
            left.y + right.y
        };
    }

    VectorIntXY operator-(const VectorIntXY& left, const VectorIntXY& right)
    {
        return VectorIntXY{
            left.x + right.x,
            left.y + right.y
        };
    }

    bool operator==(const VectorIntXY& left, const VectorIntXY& right)
    {
        return (left.x == right.x && left.y == right.y);
    }

    bool operator!=(const VectorIntXY& left, const VectorIntXY& right)
    {
        return !(left == right); // Reuse the overloaded operator==
    }

    std::ostream& operator<<(std::ostream& os, const VectorIntXY& target)
    {
        os << target.toString();
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