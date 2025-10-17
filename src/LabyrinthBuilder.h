#pragma once

#include "CellUnitConverter.h"
#include "Room.h"
#include "VectorIntXY.h"

#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>

namespace LabyrinthGeneration
{
    /// <summary>
    /// Class to build a labyrinth out of rooms, doors, and hallway assets.
    /// </summary>
    class LabyrinthBuilder
    {
        // Sentinel values for the distance field
        static const int DISTANCE_FIELD_ROOM{ std::numeric_limits<int>::max() };
        static const int DISTANCE_FIELD_UNCALCULATED{ std::numeric_limits<int>::max() - 1 };
        static const int DISTANCE_FIELD_POTENTIAL_DOOR{ 0 };
        static const int DISTANCE_FIELD_HALL{ std::numeric_limits<int>::min() };

        std::optional<int> m_randomSeed{};

        VectorIntXY m_labyrinthDimensions{1, 1};
        int m_numRoomsToSpawn{};

        // Each labyrinth distance field cell is a square.
        // This value represents the world space dimension of one side of a cell.
        double m_cellUnit{ 1 };

        int ** m_distanceField;

        Room m_room;

        CellUnitConverter m_converter;

    public:
        LabyrinthBuilder(
            VectorIntXY labyrinthDimensions, 
            int numRoomsToSpawn,
            double cellUnit,
            Room room,
            std::optional<int> randomSeed = {});

        // Rule of 3: not defining copy constructor or assignment override
        // because all we are cleaning up here is internal data that is not worth copying.
        ~LabyrinthBuilder();

        void build();

    private:
        void spawnFirstRoom();
        void spawnRoom(VectorIntXY labyrinthCoordinate);
        void addRoomToDistanceField(VectorIntXY labyrinthCoordinate);
        void debugCoutDistanceField();
    };

    void runLabyrinthBuilderTests();
}
