#pragma once

#include <vector>

#include "PlaneTransform.h"
#include "Vector3.h"
#include "VectorXY.h"

namespace LabyrinthGeneration
{
    /// <summary>
    /// This class represents a room that can be spawned in the labyrinth
    /// 
    /// The dimensions represent the world space dimensions of the room.
    /// The doors array represents 
    /// 
    /// A room is immutable. Once it is created we cannot change its data.
    /// </summary>
    class Room
    {
    private:
        const Vector3 m_dimensions;
        const std::vector<PlaneTransform> m_doors;

    public:
        Room(Vector3 dimensions, std::vector<PlaneTransform> doors) : 
            m_dimensions{ dimensions }, 
            m_doors{doors}
        { }

        const Vector3& getDimensions() const;

        const std::vector<PlaneTransform>& getDoors() const;
    };

    void runRoomTests();
}