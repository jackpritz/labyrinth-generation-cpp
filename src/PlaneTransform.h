#pragma once

#include "Vector3.h"
#include "VectorXY.h"

#include <stdexcept>

namespace LabyrinthGeneration
{
    /// <summary>
    /// Represents a position and rotation in world space
    /// Assumes that "up" is the z axis. The forward vector signifies rotation.
    /// </summary>
    class PlaneTransform
    {
        Vector3 position;

        // Cannot equal zero.
        // This will be stored as a normalized vector (magnitude == 1)
        VectorXY forward;

    public:
        PlaneTransform(Vector3 position, VectorXY forward) : 
            position{ position }, 
            forward{ forward } 
        {
            normalizeForward();
        }

        const Vector3& getPosition() const;
        void setPosition(Vector3 newPosition);

        const VectorXY& getForward() const;
        void setForward(VectorXY newForward);

    private:
        void normalizeForward();
    };

    void runPlaneTransformTests();
}