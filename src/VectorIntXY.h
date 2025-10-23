#pragma once

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

        VectorIntXY() = default;
        VectorIntXY(int x, int y) : x{ x }, y{ y } {}

        std::string toString() const;

        friend VectorIntXY operator+(const VectorIntXY& left, const VectorIntXY& right);
        friend VectorIntXY operator-(const VectorIntXY& left, const VectorIntXY& right);
        friend bool operator==(const VectorIntXY& left, const VectorIntXY& right);
        friend bool operator!=(const VectorIntXY& left, const VectorIntXY& right);

        friend std::ostream& operator<<(std::ostream& os, const VectorIntXY& dt);
    };

    void runVectorIntXYTests();
}
