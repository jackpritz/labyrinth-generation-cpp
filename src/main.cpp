#include <cassert>
#include <iostream>

#include "VectorXY.h"
#include "VectorIntXY.h"
#include "CellUnitConverter.h"
#include "Vector3.h"

using namespace LabyrinthGeneration;

int main()
{
    runVectorXYTests();
    runVectorIntXYTests();
    runCellUnitConverterTests();
    runVector3Tests();
}
