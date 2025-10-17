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

        int metersToCellRound(double meters);

        int metersToCellFloor(double meters);

        double cellToMeters(int cellCoordinateComponent);
    };

    void runCellUnitConverterTests();
}