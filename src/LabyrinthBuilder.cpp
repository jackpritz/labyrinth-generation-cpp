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
        std::optional<int> randomSeed) :
        m_randomSeed{ randomSeed },
        m_numRoomsToSpawn{numRoomsToSpawn},
        m_cellUnit{cellUnit},
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

        debugCoutDistanceField();
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

        LabyrinthBuilder builder{
            VectorIntXY{10, 5},
            3,
            2.0
        };
        
        builder.build();
    }
}
