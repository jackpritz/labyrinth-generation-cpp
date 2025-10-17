#include "CellUnitConverter.h"

#include <cassert>
#include <cmath>

namespace LabyrinthGeneration
{
    int CellUnitConverter::MetersToCellRound(double meters)
    {
        return static_cast<int>(std::round(meters / metersPerCellUnit));
    }

    int CellUnitConverter::MetersToCellFloor(double meters)
    {
        return static_cast<int>(std::floor(meters / metersPerCellUnit));
    }

    double CellUnitConverter::CellToMeters(int cellCoordinateComponent)
    {
        return cellCoordinateComponent * metersPerCellUnit;
    }

    void runCellUnitConverterTests()
    {
        CellUnitConverter converter{ 2.5 };

        assert(5 == converter.MetersToCellRound(12));
        assert(4 == converter.MetersToCellFloor(12));
        assert(7.5 == converter.CellToMeters(3));
    }
}