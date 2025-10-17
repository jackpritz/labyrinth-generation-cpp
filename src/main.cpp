#include <cassert>
#include <iostream>

#include "CellUnitConverter.h"
#include "PlaneTransform.h"
#include "Room.h"
#include "Vector3.h"
#include "VectorIntXY.h"
#include "VectorXY.h"

using namespace LabyrinthGeneration;

int main()
{
    runVectorXYTests();
    runVectorIntXYTests();
    runCellUnitConverterTests();
    runVector3Tests();
    runPlaneTransformTests();
    runRoomTests();
}
