#pragma once

#include "CellUnitConverter.h"
#include "Room.h"
#include "VectorIntXY.h"

#include <iostream>
#include <limits>
#include <optional>
#include <vector>

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

        std::vector<std::vector<int>> m_distanceField;

        Room m_room;

        CellUnitConverter m_converter;

        std::vector<VectorIntXY> zeroDistanceCoordinates{};

        std::vector<VectorIntXY> m_traversalDirections{ {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    public:
        LabyrinthBuilder(
            VectorIntXY labyrinthDimensions, 
            int numRoomsToSpawn,
            double cellUnit,
            Room room,
            std::optional<int> randomSeed = {});

        void build();

    private:
        void spawnFirstRoom ();
        void spawnRoom      (VectorIntXY cell);

        void addRoomToDistanceField      (VectorIntXY cell);
        void addRoomDoorsToDistanceField (VectorIntXY cell);

        VectorIntXY findDoorCoordinate (VectorIntXY roomCell, PlaneTransform door);
        bool        isInDistanceField  (VectorIntXY cell);

        void setPotentialDoorCell(VectorIntXY labyrinthCoordinate);

        void recalculateDistanceField();

        void debugCoutDistanceField();
    };

    void runLabyrinthBuilderTests();
}
