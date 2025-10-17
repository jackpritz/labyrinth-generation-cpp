
#include <cassert>

#include "Vector3.h"

namespace LabyrinthGeneration
{
    std::string Vector3::toString() const
    {
        return "(" + 
            std::to_string(x) + ", " + 
            std::to_string(y) + ", " + 
            std::to_string(z) + ")";
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