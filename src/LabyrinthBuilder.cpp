#include "LabyrinthBuilder.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <queue>

namespace LabyrinthGeneration
{
    LabyrinthBuilder::LabyrinthBuilder(
        VectorIntXY labyrinthDimensions,
        int numRoomsToSpawn,
        double cellUnit,
        Room room,
        std::optional<unsigned int> randomSeed) :
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

        // Set up random number generator
        if (m_randomSeed.has_value())
        { 
            m_randomGenerator.seed(m_randomSeed.value());
        }
        else
        {
            unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
            std::cout << "Using random seed: " << seed << "\n";
            m_randomGenerator.seed(seed);
        }

        spawnRooms();

        debugCoutDistanceField();
    }

    void LabyrinthBuilder::spawnRooms()
    {
        spawnFirstRoom();
        recalculateDistanceField();

        VectorIntXY center{m_labyrinthDimensions.x / 2, m_labyrinthDimensions.y / 2 };
        int numToSpawn{ m_numRoomsToSpawn - 1 };

        // Set up a distribution for the random number generator
        std::uniform_real_distribution<double> distribution(-1.0, 1.0);

        while (numToSpawn > 0)
        {
            // Pick a random direction
            VectorXY direction{
                distribution(m_randomGenerator) ,
                distribution(m_randomGenerator) };

            // Find an open space.
            // Start at center and move in the chosen direction looking for enough space for the new room.
            int roomsizeX = m_converter.metersToCellRound(m_room.getDimensions().x);
            int roomsizeY = m_converter.metersToCellRound(m_room.getDimensions().y);

            VectorXY potentialRoomPosition{
                m_converter.cellToMeters(center.x),
                m_converter.cellToMeters(center.y)
            };

            VectorIntXY potentialRoomCoordinates{ center.x, center.y };
            bool foundSpawn{ false };

            // Search for open space along the search path until:
            // 1. we find open space or
            // 2. we hit the edge.
            while (areRoomExtentsWithinLabyrinth(potentialRoomCoordinates, roomsizeX, roomsizeY))
            {
                bool roomPositionValid = true;

                // if we find overlap, increment our distance and continue
                for (int y = 0; y < roomsizeY; y++)
                {
                    if (!roomPositionValid) { break; }

                    for (int x = 0; x < roomsizeX; x++)
                    {
                        int cellValue = m_distanceField[potentialRoomCoordinates.y + y][potentialRoomCoordinates.x + x];
                        if (cellValue == DISTANCE_FIELD_ROOM || cellValue <= 0)
                        {
                            potentialRoomPosition = nextCoordinateAlongSearchPath(potentialRoomPosition, direction);
                            potentialRoomCoordinates = VectorIntXY(
                                m_converter.metersToCellFloor(potentialRoomPosition.x),
                                m_converter.metersToCellFloor(potentialRoomPosition.y));

                            // break out to while loop
                            roomPositionValid = false;
                            break;
                        }
                    }
                }

                // otherwise, we have found our spawn position
                if (roomPositionValid)
                {
                    foundSpawn = true;
                    break;
                }
            }

            if (!foundSpawn)
            {
                std::cout << "LabyrinthBuilder could not spawn a room along a search path! Trying a new path.\n";
                continue; // try again
            }
            else
            {
                numToSpawn--;
            }

            spawnRoom(potentialRoomCoordinates);

            connectToExistingRooms(potentialRoomCoordinates, m_room);

            addRoomDoorsToDistanceField(potentialRoomCoordinates);

            // Update distance field
            recalculateDistanceField();
        }
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
        for (const PlaneTransform& door : m_room.getDoors())
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

    bool LabyrinthBuilder::areRoomExtentsWithinLabyrinth(VectorIntXY position, int sizeX, int sizeY)
    {
        return
            isInDistanceField(position + VectorIntXY{ 0, sizeY }) &&
            isInDistanceField(position) &&
            isInDistanceField(position + VectorIntXY{ sizeX, 0 }) &&
            isInDistanceField(position + VectorIntXY{ sizeX, sizeY });
    }

    void LabyrinthBuilder::setPotentialDoorCell(VectorIntXY cell)
    {
        // If cell not found in zeroDistanceCoordinates cache
        if (m_zeroDistanceCoordinates.end() ==
            std::find(m_zeroDistanceCoordinates.begin(), m_zeroDistanceCoordinates.end(), cell))
        {
            m_distanceField[cell.y][cell.x] = DISTANCE_FIELD_POTENTIAL_DOOR;

            m_zeroDistanceCoordinates.push_back(cell);
        }
    }

    void LabyrinthBuilder::setHallwayCell(VectorIntXY cell)
    {
        // hall overrides potential door. Always set this.
        m_distanceField[cell.y][cell.x] = DISTANCE_FIELD_HALL;

        VectorIntXY coordinate {cell.x, cell.y};
        if (std::find(m_zeroDistanceCoordinates.begin(), m_zeroDistanceCoordinates.end(), coordinate) == m_zeroDistanceCoordinates.end())
        {
            m_zeroDistanceCoordinates.push_back(coordinate);
        }
    }

    VectorXY LabyrinthBuilder::nextCoordinateAlongSearchPath(VectorXY currentposition, VectorXY searchDirection)
    {
        double nextX, nextY;

        if (searchDirection.x > 0)
        {
            nextX = currentposition.x + m_cellUnit;
        }
        else
        {
            nextX = currentposition.x - m_cellUnit;
        }

        if (searchDirection.y > 0)
        {
            nextY = currentposition.y + m_cellUnit;
        }
        else
        {
            nextY = currentposition.y - m_cellUnit;
        }

        // Use z = mx + b to fill in missing values.
        // m: slope
        // b: z such that x = 0
        double slope = searchDirection.y / searchDirection.x;
        double yIntercept = currentposition.y - (slope * currentposition.x); // b = z - (m * x)
        VectorXY targetInterceptX = VectorXY{
            nextX,
            (slope * nextX) + yIntercept // y = (m * x) + b
        };
        VectorXY targetInterceptZ = VectorXY{
            (nextY - yIntercept) / slope, // x = (y - b) / m
            nextY
        };

        // Choose closest candidate as new currentPosition
        if (VectorXY::distanceSquared(currentposition, targetInterceptX) < VectorXY::distanceSquared(currentposition, targetInterceptZ))
        {
            return targetInterceptX;
        }
        else
        {
            return targetInterceptZ;
        }
    }

    void LabyrinthBuilder::connectToExistingRooms(VectorIntXY roomSpawnCoordinate, const Room& room)
    {
        std::vector<PlaneTransform> doors = room.getDoors();
        if (doors.size() == 0) { throw std::runtime_error{ "Tried to connect a room, but it has no doors!" }; }

        VectorIntXY minimumDistanceDoor{};
        int currentMinimumDistance{ std::numeric_limits<int>::max() };

        // pick a door to connect based on minimum distance in distance field
        for (const PlaneTransform& door : doors )
        {
            VectorIntXY doorCoordinates = findDoorCoordinate(roomSpawnCoordinate, door);
            int currentDistance{ m_distanceField[doorCoordinates.y][doorCoordinates.x] };

            if (isInDistanceField(doorCoordinates) &&
                currentDistance < currentMinimumDistance)
            {
                currentMinimumDistance = currentDistance;
                minimumDistanceDoor = doorCoordinates;
            }
        }

        VectorIntXY currentPathLocation = minimumDistanceDoor;

        std::vector<VectorIntXY> path{};
        path.push_back(currentPathLocation);

        while (m_distanceField[currentPathLocation.y][currentPathLocation.x] > 0)
        {
            // look in all directions for minimum distance. set that as new current location.
            VectorIntXY minimumDistanceCell{};
            int currentMinimumCellDistance{ std::numeric_limits<int>::max() };

            for (VectorIntXY direction : m_traversalDirections)
            {
                VectorIntXY cellCoord{ currentPathLocation + direction };
                int cellValue{ m_distanceField[cellCoord.y][cellCoord.x] };

                if (cellValue < currentMinimumCellDistance)
                {
                    currentMinimumCellDistance = cellValue;
                    minimumDistanceCell = cellCoord;
                }
            }

            currentPathLocation = minimumDistanceCell;
            path.push_back(currentPathLocation);
        }

        for (VectorIntXY cell : path)
        {
            if (m_distanceField[cell.y][cell.x] != DISTANCE_FIELD_HALL)
            {
                // spawn hall floor
                //var newHall = Instantiate(hallFloorAndCeiling, transform);
                //newHall.transform.localPosition = unitConverter.CellToMeters(new Vector3(cell.x, 0, cell.z));

                setHallwayCell(cell);
            }
        }
    }



    void LabyrinthBuilder::recalculateDistanceField()
    {
        // Check all zero distance coordinates for recalculation of neighbors
        std::queue<VectorIntXY> toCheck{};
        for (VectorIntXY coordinate : m_zeroDistanceCoordinates)
        {
            toCheck.push(coordinate);
        }

        while (toCheck.size() > 0)
        {
            VectorIntXY currentCoordinate{ toCheck.front() };
            toCheck.pop();

            int currentCoordinateDistance = m_distanceField[currentCoordinate.y][currentCoordinate.x];

            // max(currentCoordinateDistance, 0) to handle sentinel values.
            if (currentCoordinateDistance < 0) { currentCoordinateDistance = 0; }

            // Check each direction. If a neighbor cell needs a value, update and queue it. Otherwise move to the next cell.
            for (VectorIntXY direction : m_traversalDirections)
            {
                VectorIntXY cellToCheck = currentCoordinate + direction;

                if (!isInDistanceField(cellToCheck))
                {
                    continue;
                }

                if (m_distanceField[cellToCheck.y][cellToCheck.x] != DISTANCE_FIELD_ROOM
                    && m_distanceField[cellToCheck.y][cellToCheck.x] > currentCoordinateDistance + 1)
                {
                    m_distanceField[cellToCheck.y][cellToCheck.x] = currentCoordinateDistance + 1;
                    toCheck.push(cellToCheck);
                }
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

        //std::optional<unsigned int> seed{ static_cast<unsigned int>(2269388892) }; // explicit random seed

        LabyrinthBuilder builder{
            VectorIntXY{40, 40},
            8,
            2.0,
            room,
            //seed
        };
        
        builder.build();
    }
}
