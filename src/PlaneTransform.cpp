#include "PlaneTransform.h"

#include <cassert>
#include <cmath>

namespace LabyrinthGeneration
{
    PlaneTransform::PlaneTransform(Vector3 position, VectorXY forward) :
        m_position{ position },
        m_forward{ forward }
    {
        normalizeForward();
    }

    const Vector3& PlaneTransform::getPosition() const
    {
        return m_position;
    }

    void PlaneTransform::setPosition(Vector3 newPosition)
    {
        m_position = newPosition;
    }

    const VectorXY& PlaneTransform::getForward() const
    {
        return m_forward;
    }

    void PlaneTransform::setForward(VectorXY newForward)
    {
        m_forward = newForward;
        normalizeForward();
    }

    void PlaneTransform::normalizeForward()
    {
        if (m_forward.x == 0 && m_forward.y == 0)
        {
            throw std::runtime_error("Forward VectorXY cannot equal zero.");
        }

        double magnitude = std::sqrt((m_forward.x * m_forward.x) + (m_forward.y * m_forward.y));
        m_forward.x = m_forward.x / magnitude;
        m_forward.y = m_forward.y / magnitude;
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