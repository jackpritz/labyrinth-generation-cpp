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
        Vector3 m_position;

        // Cannot equal zero.
        // This will be stored as a normalized vector (magnitude == 1)
        VectorXY m_forward;

    public:
        PlaneTransform(Vector3 position, VectorXY forward);

        const Vector3& getPosition() const;
        void setPosition(Vector3 newPosition);

        const VectorXY& getForward() const;
        void setForward(VectorXY newForward);

    private:
        void normalizeForward();
    };

    void runPlaneTransformTests();
}