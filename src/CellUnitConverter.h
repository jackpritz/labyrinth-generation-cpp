#pragma once

namespace LabyrinthGeneration
{
    /// <summary>
    /// Utility class to convert between world space and cell coordinate space.
    /// </summary>
    class CellUnitConverter
    {
        double m_metersPerCellUnit;

    public:
        CellUnitConverter(double metersPerCellUnit) : m_metersPerCellUnit{ metersPerCellUnit } {}

        int MetersToCellRound(double meters);

        int MetersToCellFloor(double meters);

        double CellToMeters(int cellCoordinateComponent);
    };

    void runCellUnitConverterTests();
}