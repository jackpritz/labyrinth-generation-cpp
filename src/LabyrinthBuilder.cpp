#include "LabyrinthBuilder.h"

#include <cassert>
#include <format>
#include <iostream>

namespace LabyrinthGeneration
{
    LabyrinthBuilder::LabyrinthBuilder(
        VectorIntXY labyrinthDimensions,
        int numRoomsToSpawn,
        double cellUnit,
        Room room,
        std::optional<int> randomSeed) :
        m_randomSeed{ randomSeed },
        m_numRoomsToSpawn{numRoomsToSpawn},
        m_cellUnit{cellUnit},
        m_room{room},
        m_converter{ m_cellUnit },
        m_labyrinthDimensions{ labyrinthDimensions }
    {
        if (labyrinthDimensions.x < 1 || labyrinthDimensions.y < 1)
        {
            std::string error{ "Labyrinth must have dimensions that are natural numbers. LabyrinthBuilder instantiated with dimensions X = " + std::to_string(labyrinthDimensions.x) + ", Y = " + std::to_string(labyrinthDimensions.y) };
            std::cerr << error;
            throw std::runtime_error((error));
        }

        if (cellUnit == 0)
        {
            std::string error{ "LabyrinthBuilder cell unit cannot be zero!" };
            std::cerr << error;
            throw std::runtime_error((error));
        }

        m_distanceField = new int*[m_labyrinthDimensions.y];
        for (int y = 0; y < m_labyrinthDimensions.y; y++)
        {
            m_distanceField[y] = new int[m_labyrinthDimensions.x];
        }
    }

    // Rule of 3: not defining copy constructor or assignment override
    // because all we are cleaning up here is internal data that is not worth copying.
    LabyrinthBuilder::~LabyrinthBuilder()
    {
        
        for (int y = 0; y < m_labyrinthDimensions.y; y++)
        {
            delete m_distanceField[y];
        }

        delete m_distanceField;
    }

    void LabyrinthBuilder::build()
    {
        // Wipe the distance field
        for (int y = 0; y < m_labyrinthDimensions.y; y++)
        {
            for (int x = 0; x < m_labyrinthDimensions.x; x++)
            {
                m_distanceField[y][x] = DISTANCE_FIELD_UNCALCULATED;
            }
        }

        if (m_numRoomsToSpawn < 1)
        {
            return;
        }

        spawnFirstRoom();

        debugCoutDistanceField();
    }

    void LabyrinthBuilder::spawnFirstRoom()
    {
        Vector3 roomWorldSpaceDimensions{ m_room.getDimensions() };
        VectorIntXY roomCellDimensions{ m_converter.metersToCellRound(roomWorldSpaceDimensions.x), m_converter.metersToCellRound(roomWorldSpaceDimensions.y) };

        int roomSpawnX{ (m_labyrinthDimensions.x / 2) - (roomCellDimensions.x / 2) };
        int roomSpawnY{ (m_labyrinthDimensions.y / 2) - (roomCellDimensions.y / 2) };

        spawnRoom(VectorIntXY{ roomSpawnX, roomSpawnY });

        //addRoomDoorsToDistanceField(room);
    }

    /// <summary>
    /// Spawn a room at the given location, which is the x, y minimum extent of the room.
    /// </summary>
    /// <param name="labyrinthCoordinate"></param>
    void LabyrinthBuilder::spawnRoom(VectorIntXY labyrinthCoordinate)
    {
        addRoomToDistanceField(labyrinthCoordinate);
    }

    void LabyrinthBuilder::addRoomToDistanceField(VectorIntXY labyrinthCoordinate)
    {
        Vector3 roomWorldSpaceDimensions{ m_room.getDimensions() };
        VectorIntXY roomCellDimensions{ m_converter.metersToCellRound(roomWorldSpaceDimensions.x), m_converter.metersToCellRound(roomWorldSpaceDimensions.y) };

        // Make room tiles distance max int. Max int denotes not passable.
        for (int y = 0; y < roomCellDimensions.y; y++)
        {
            for (int x = 0; x < roomCellDimensions.x; x++)
            {
                int currentRoomX = labyrinthCoordinate.x + x;
                int currentRoomY = labyrinthCoordinate.y + y;
                m_distanceField[currentRoomY][currentRoomX] = DISTANCE_FIELD_ROOM;
            }
        }
    }

    void LabyrinthBuilder::debugCoutDistanceField()
    {
        for (int y = 0; y < m_labyrinthDimensions.y; y++)
        {
            // print row number
            std::cout << std::format("{:02}", y);

            for (int x = 0; x < m_labyrinthDimensions.x; x++)
            {
                // give ourselvs a visual indication of every 10 columns
                if (x % 5 == 0)
                {
                    std::cout << " | ";
                }

                int currentCellValue = m_distanceField[y][x];

                if (currentCellValue == DISTANCE_FIELD_ROOM)
                {
                    std::cout << "room  ";
                }
                else if (currentCellValue == DISTANCE_FIELD_UNCALCULATED)
                {
                    std::cout << "....  ";
                }
                else if (currentCellValue == DISTANCE_FIELD_HALL)
                {
                    std::cout << "hall  ";
                }
                else
                {
                    std::cout << std::format("{:04}  ", currentCellValue);
                }
            }

            // on to the next row
            std::cout << "\n";
        }
    }

    void runLabyrinthBuilderTests()
    {
        // todo make for realsies

        // dev testing as we go

        Room room
        {
            // Define a room that is:
            // - 6 x 6 meters footprint
            // - Has a door in the center of each wall
            Room
            {
                Vector3{6, 6, 3}, // room dimensions
                { // doors
                    PlaneTransform
                    {
                        Vector3{3, 0, 0},
                        VectorXY{0, -1}
                    },
                    PlaneTransform
                    {
                        Vector3{3, 6, 0},
                        VectorXY{0, 1}
                    },
                    PlaneTransform
                    {
                        Vector3{0, 3, 0},
                        VectorXY{-1, 0}
                    },
                    PlaneTransform
                    {
                        Vector3{6, 3, 0},
                        VectorXY{1, 0}
                    },
                }
            }
        };

        LabyrinthBuilder builder{
            VectorIntXY{10, 10},
            3,
            2.0,
            room
        };
        
        builder.build();
    }
}
