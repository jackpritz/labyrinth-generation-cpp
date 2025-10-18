#include "LabyrinthBuilder.h"

#include <algorithm>
#include <cassert>
#include <format>

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

        m_distanceField = std::vector(m_labyrinthDimensions.y, std::vector<int>(m_labyrinthDimensions.x));
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

        VectorIntXY roomSpawnCell
        {
            (m_labyrinthDimensions.x / 2) - (roomCellDimensions.x / 2),
            (m_labyrinthDimensions.y / 2) - (roomCellDimensions.y / 2)
        };

        spawnRoom(roomSpawnCell);

        addRoomDoorsToDistanceField(roomSpawnCell);
    }

    /// <summary>
    /// Spawn a room at the given location, which is the x, y minimum extent of the room.
    /// </summary>
    /// <param name="labyrinthCoordinate"></param>
    void LabyrinthBuilder::spawnRoom(VectorIntXY cell)
    {
        addRoomToDistanceField(cell);
    }

    void LabyrinthBuilder::addRoomToDistanceField(VectorIntXY cell)
    {
        Vector3 roomWorldSpaceDimensions{ m_room.getDimensions() };
        VectorIntXY roomCellDimensions{ m_converter.metersToCellRound(roomWorldSpaceDimensions.x), m_converter.metersToCellRound(roomWorldSpaceDimensions.y) };

        // Make room tiles distance max int. Max int denotes not passable.
        for (int y = 0; y < roomCellDimensions.y; y++)
        {
            for (int x = 0; x < roomCellDimensions.x; x++)
            {
                int currentRoomX = cell.x + x;
                int currentRoomY = cell.y + y;
                m_distanceField[currentRoomY][currentRoomX] = DISTANCE_FIELD_ROOM;
            }
        }
    }

    void LabyrinthBuilder::addRoomDoorsToDistanceField(VectorIntXY cell)
    {
        // Mark the space outside each door as a potential door.
        for (PlaneTransform door : m_room.getDoors())
        {
            VectorIntXY doorCoordinate{ findDoorCoordinate(cell, door) };

            // Make sure we are still in the array and not overriding a room
            if (!isInDistanceField(doorCoordinate) || m_distanceField[doorCoordinate.y][doorCoordinate.x] == DISTANCE_FIELD_ROOM)
            {
                continue;
            }

            // Make that coordinate 0 in the distance field
            setPotentialDoorCell(doorCoordinate);
        }
    }

    /// <summary>
    /// Spawn the specified door
    /// from the perspective of the local space of the provided cell.
    /// </summary>
    /// <param name="cell"></param>
    /// <param name="door"></param>
    VectorIntXY LabyrinthBuilder::findDoorCoordinate(VectorIntXY roomCell, PlaneTransform door)
    {
        // Move door position forward into an adjoining cell.
        // The provided position is the door prefab spawn position.
        // The forward direction of the provided transform indicates "out of the room"
        // Adding half of a unit also accounts for float precision.
        Vector3 doorPosition = door.getPosition();
        Vector3 hallPosition = Vector3{ doorPosition.x, doorPosition.y, doorPosition.z };
        VectorXY doorForward{ door.getForward().x, door.getForward().y};
        hallPosition = hallPosition + ((Vector3{ doorForward.x, doorForward.y, 0 } * 0.5f));

        // Translate to coordinate in grid
        int doorX{ roomCell.x + m_converter.metersToCellFloor(hallPosition.x) };

        int doorY{ roomCell.y + m_converter.metersToCellFloor(hallPosition.y) };

        return VectorIntXY{ doorX, doorY };
    }

    bool LabyrinthBuilder::isInDistanceField(VectorIntXY cell)
    {
        return 
            (cell.x >= 0) && 
            (cell.y >= 0) && 
            (cell.x < m_labyrinthDimensions.x) && 
            (cell.y < m_labyrinthDimensions.y);
    }

    void LabyrinthBuilder::setPotentialDoorCell(VectorIntXY cell)
    {
        // If cell not found in zeroDistanceCoordinates cache
        if (zeroDistanceCoordinates.end() ==
            std::find(zeroDistanceCoordinates.begin(), zeroDistanceCoordinates.end(), cell))
        {
            m_distanceField[cell.y][cell.x] = DISTANCE_FIELD_POTENTIAL_DOOR;

            zeroDistanceCoordinates.push_back(cell);
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
                    std::cout << std::format(" {:02}   ", currentCellValue);
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
            VectorIntXY{20, 20},
            3,
            2.0,
            room
        };
        
        builder.build();
    }
}
