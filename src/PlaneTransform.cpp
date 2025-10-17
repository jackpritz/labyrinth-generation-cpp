#include "PlaneTransform.h"

#include <cassert>
#include <cmath>

namespace LabyrinthGeneration
{
    const Vector3& PlaneTransform::getPosition() const
    {
        return position;
    }

    void PlaneTransform::setPosition(Vector3 newPosition)
    {
        position = newPosition;
    }

    const VectorXY& PlaneTransform::getForward() const
    {
        return forward;
    }

    void PlaneTransform::setForward(VectorXY newForward)
    {
        forward = newForward;
        normalizeForward();
    }

    void PlaneTransform::normalizeForward()
    {
        if (forward.x == 0 && forward.y == 0)
        {
            throw std::runtime_error("Forward VectorXY cannot equal zero.");
        }

        double magnitude = std::sqrt((forward.x * forward.x) + (forward.y * forward.y));
        forward.x = forward.x / magnitude;
        forward.y = forward.y / magnitude;
    }

    void runPlaneTransformTests()
    {
        PlaneTransform transform{
            Vector3 {1, 2, 3}, 
            VectorXY {1, 1}
        };

        Vector3 position{ transform.getPosition() };
        VectorXY forward{ transform.getForward() };

        assert(
            position.x == 1 &&
            position.y == 2 &&
            position.z == 3
        );

        assert(
            forward.x == (1 / std::sqrt(2)) &&
            forward.y == (1 / std::sqrt(2))
        );

        transform.setPosition(Vector3{ 4, 5, 6 });
        transform.setForward(VectorXY{ -1, -1 });

        position = transform.getPosition();
        forward = transform.getForward();

        assert(
            position.x == 4 &&
            position.y == 5 &&
            position.z == 6
        );

        assert(
            forward.x == -(1 / std::sqrt(2)) &&
            forward.y == -(1 / std::sqrt(2))
        );
    }
}