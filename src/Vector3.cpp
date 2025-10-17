#include "Vector3.h"

#include <cassert>
#include <iostream>

namespace LabyrinthGeneration
{
    std::string Vector3::toString() const
    {
        return "(" + 
            std::to_string(x) + ", " + 
            std::to_string(y) + ", " + 
            std::to_string(z) + ")";
    }

    Vector3 operator+(const Vector3& left, const Vector3& right)
    {
        return Vector3{
            left.x + right.x,
            left.y + right.y,
            left.z + right.z
        };
    }

    Vector3 operator-(const Vector3& left, const Vector3& right)
    {
        return Vector3{
            left.x + right.x,
            left.y + right.y,
            left.z + right.z
        };
    }

    Vector3 operator*(const Vector3& vec, const double& scalar)
    {
        return Vector3{
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar,
        };
    }

    Vector3 operator/(const Vector3& vec, const double& scalar)
    {
        return Vector3{
            vec.x / scalar,
            vec.y / scalar,
            vec.z / scalar,
        };
    }

    std::ostream& operator<<(std::ostream& os, const Vector3& target)
    {
        os << target.toString();
        return os;
    }

    void runVector3Tests()
    {
        Vector3 vec{ 1, 2, 3 };

        assert("(1.000000, 2.000000, 3.000000)" == vec.toString());
    }
}