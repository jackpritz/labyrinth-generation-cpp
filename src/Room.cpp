#include <cassert>

#include "Room.h"

namespace LabyrinthGeneration
{
    const std::vector<PlaneTransform>& Room::getDoors() const
    {
        return doors;
    }

    const Vector3& Room::getDimensions() const
    {
        return dimensions;
    }

    void runRoomTests()
    {
        Room room{
            // position
            Vector3{1, 2, 3},
            // doors
            { 
                PlaneTransform{Vector3{1.1, 1.2, 1.3}, VectorXY{1.4, 1.5}},
                PlaneTransform{Vector3{2.1, 2.2, 2.3}, VectorXY{2.4, 2.5}}
            }
        };

        Vector3 dimensions{ room.getDimensions()};
        dimensions.x = 10;
        Vector3 originalDimensions{ room.getDimensions() };

        // Verify data encapsulation for position
        assert(dimensions.x == 10);
        assert(originalDimensions.x == 1);

        std::vector<PlaneTransform> doors{ room.getDoors() };
        doors.push_back(PlaneTransform{ Vector3{2.1, 2.2, 2.3}, VectorXY{2.4, 2.5} });
        std::vector<PlaneTransform> originalDoors{ room.getDoors() };

        // Verify data encapsulation for doors
        assert(3 == doors.size());
        assert(2 == originalDoors.size());
    }
}