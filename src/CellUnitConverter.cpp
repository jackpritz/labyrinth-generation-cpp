#include "CellUnitConverter.h"

#include <cassert>
#include <cmath>

namespace LabyrinthGeneration
{
    int CellUnitConverter::metersToCellRound(double meters)
    {
        return static_cast<int>(std::round(meters / m_metersPerCellUnit));
    }

    int CellUnitConverter::metersToCellFloor(double meters)
    {
        return static_cast<int>(std::floor(meters / m_metersPerCellUnit));
    }

    double CellUnitConverter::cellToMeters(int cellCoordinateComponent)
    {
        return cellCoordinateComponent * m_metersPerCellUnit;
    }

    void runCellUnitConverterTests()
    {
        CellUnitConverter converter{ 2.5 };

        assert(5 == converter.metersToCellRound(12));
        assert(4 == converter.metersToCellFloor(12));
        assert(7.5 == converter.cellToMeters(3));
    }
}