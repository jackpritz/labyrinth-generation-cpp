#include <cassert>
#include <iostream>

#include "VectorXY.h"
#include "VectorIntXY.h"
#include "CellUnitConverter.h"

using namespace LabyrinthGeneration;

int main()
{
    runVectorXYTests();
    runVectorIntXYTests();
    runCellUnitConverterTests();
}
