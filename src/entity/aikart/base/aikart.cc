#include <cmath>
#include "aikart.h"
#include "fileutil.h"
#include "geometryutil.h"
#include "minimap.h"
#include "geometryutil.h"

#define CAN_LOG 0
#define LOG_TAG "{AIKart} "

const char * AIKart::logTag() {
	return LOG_TAG;
}

bool AIKart::canLog() {
	return CAN_LOG;
}

int AIKart::minimapSize() {
    return Minimap::MINIMAP_SIZE / 128;
}

AIKart::AIKart() {

}

AIKart::~AIKart() {

}

void AIKart::draw() {

}

void AIKart::update() {

    Point currentPoint { mCol, mRow };
    Point nextPoint = mPath.points[mNextPathPointIndex];

    double remainingTurnAngleForNextPoint = fmod(
        GeometryUtil::angleBetweenTwoPoints(currentPoint, nextPoint) - mDirection + M_PI * 2,
        M_PI * 2
    );

    d2("currentPoint: ", currentPoint);
    d2("nextPoint: ", nextPoint);
    d2("Angle between current and next point: ", GeometryUtil::angleBetweenTwoPoints(currentPoint, nextPoint));
    d2("Angle for reach next point considering current direction: ", remainingTurnAngleForNextPoint);
    d2("Current AI Kart direction:", mDirection);

	const double ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON = 0.4;

    bool shouldGoBackward;
    bool shouldGoForward;
    bool shouldGoLeft;
    bool shouldGoRight;

    // Going in the right direction for the next point
    if (remainingTurnAngleForNextPoint < ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON)  {
        shouldGoLeft = shouldGoRight = false;
        // d2("Going to the right direction, not turning");
    }
    else {
        // d2("Going to the wrong direction, turning");
        shouldGoRight = (remainingTurnAngleForNextPoint >= ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON &&
			 			remainingTurnAngleForNextPoint < M_PI);
        shouldGoLeft = (remainingTurnAngleForNextPoint >= M_PI - ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON &&
			 			 remainingTurnAngleForNextPoint <= M_PI * 2);
    }

    // if (remainingTurnAngleForNextPoint > M_PI / 2 && remainingTurnAngleForNextPoint < M_PI * 3/2 ) {
    //     shouldGoBackward = true;// Change this
    //     shouldGoForward = false;
    // }
    // else {
        shouldGoBackward = false;
        shouldGoForward = true;
    // }


    d2("AI Kart Decision: ",
        "\n\tGo forward: ", shouldGoForward,
        "\n\tGo backward: ", shouldGoBackward,
        "\n\tGo left: ", shouldGoLeft,
        "\n\tGo right: ", shouldGoRight
    );
    Kart::update(shouldGoForward, shouldGoBackward, shouldGoLeft, shouldGoRight);

	const double POINT_REACHED_DISTANCE_EPSILON = 3.5;
	const double distanceToNextPoint = GeometryUtil::distanceBetweenTwoPoints(currentPoint, nextPoint);
	if (distanceToNextPoint < POINT_REACHED_DISTANCE_EPSILON) {
			mNextPathPointIndex = (mNextPathPointIndex + 1) % mPath.pointCount;

			// d("Point reached, going to next at index: ", mNextPathPointIndex, ": [", nextPoint, "]");    // d2("AI Kart Decision: ",
    //     "\n\tGo forward: ", shouldGoForward,
    //     "\n\tGo backward: ", shouldGoBackward,
    //     "\n\tGo left: ", shouldGoLeft,
    //     "\n\tGo right: ", shouldGoRight
    // );
	}
	else {
		d("Distance to next point: [", mNextPathPointIndex ,"]", distanceToNextPoint);
	}
}

void AIKart::pathArrayFillFunction(int readVal, int fileRow, int fileCol) {
	int halfCol = fileCol / 2;
	bool firstOfPair = (fileCol % 2 == 0);
	firstOfPair ?
		mPath.points[halfCol].y = readVal :
        mPath.points[halfCol].x = readVal;
}

void AIKart::initAIPath() {
    int fileRowCount, fileColCount;
	FileUtil::getMatrixSize<int>(mPathFilename.c_str(), fileRowCount, fileColCount);
	mPath.pointCount = fileColCount / 2;
    mPath.points = new Point[mPath.pointCount];

    void (AIKart::*pathFillFunctionPtr)(int, int, int) = &AIKart::pathArrayFillFunction;

    FileUtil::loadStructureFromFileKnowningSize<int, AIKart>(
    	mPathFilename.c_str(), fileRowCount, fileColCount, this, pathFillFunctionPtr);

    d("Loaded AI path: ", mPathFilename, " of size: ", mPath.pointCount);

    for (int i = 0; i < mPath.pointCount; i++) {
        d("Path point[", i, "]", mPath.points[i].y, ", ", mPath.points[i].x);
    }

    // Set the current position to the first point
    mNextPathPointIndex = /*0*/ 15;
}