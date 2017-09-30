#include <SFML/Graphics.hpp>
#include <cmath>
#include "aikart.h"
#include "fileutil.h"
#include "geometryutil.h"
#include "perspectiveutil.h"
#include "viewutil.h"
#include "mathutil.h"
#include "kartfactory.h"
#include "minimap.h"
#include "playerkart.h"
#include "const.h"
#include "level.h"

#define LOG_TAG "{AIKart} "
#define CAN_LOG 1

int AIKart::minimapSize() {
    return Minimap::MINIMAP_SIZE / 128;
}

AIKart::AIKart( Level * level, const char *kartName, sf::Color *kartColor)
                 : Kart(level, kartName, kartColor) {
	mDirectionalPoint.direction = Angle {(double)1 / (1 << 9)};
}

AIKart::~AIKart() {

}

void AIKart::draw() {
    Kart *playerKart = mLevel->playerKart();

    Point pc, pr, vp1, vp2;

    Point pp = PerspectiveUtil::perspectivePoint(
        position(),
        ViewUtil::cameraPoint(playerKart->directionalPoint()),
        ViewUtil::BASE_POINT,
        playerKart->direction(),
        ViewUtil::HORIZON_LINE_Y,
        ViewUtil::RENDERED_TILE_SIZE,
        pc,
        pr,
        vp1,
        vp2
    );
    const float kartScaleFactor = KartFactory::scaleFactor();

    double perspectiveScaleFactor = PerspectiveUtil::scaleForPerspectivePoint(pp, vp2, pc);
    double scaleFactor = kartScaleFactor * perspectiveScaleFactor;

    double adjustmentAngleDiff = PerspectiveUtil::angleAdjustmentForPerspectivePoint(
        0, Const::WINDOW_WIDTH, ViewUtil::HORIZON_LINE_Y, ViewUtil::BASE_POINT.y, pp);

    double angleDiff = (direction() - playerKart->direction() - adjustmentAngleDiff).rad;

    int spriteIndexForCurrentDirection =
        MathUtil::changeRangeToInt(
            MathUtil::Range {0, 2 * M_PI },
            MathUtil::Range {0, static_cast<double>(mSpriteCount)},
            angleDiff) % mSpriteCount;
    // d2("Choosing int index: ", spriteIndexForCurrentDirection);

	mSprites[spriteIndexForCurrentDirection].setPosition(pp.x, pp.y);

	mSprites[spriteIndexForCurrentDirection].setScale(scaleFactor, scaleFactor);
    // d2("Venusian scale factor\t", kartScaleFactor);
    // d2("perspectiveScaleFactor\t",  perspectiveScaleFactor);
	// d2("perspectiveScale\t", scaleFactor);
	// d2("PP\t", pp);
	// d2("Current scaled AI sprite dim: ", mSprites[spriteIndexForCurrentDirection].getGlobalBounds().height);

    sf::FloatRect spriteRect = mSprites[spriteIndexForCurrentDirection].getGlobalBounds();
    Point spriteUL {spriteRect.left, spriteRect.top};
    Point spriteUR {spriteUL.x + spriteRect.width, spriteUL.y};
    Point spriteDR {spriteUR.x, spriteUL.y + spriteRect.height};
    Point spriteDL {spriteUR.x, spriteDR.y};
    if (ViewUtil::isOnScreenUnderHorizon(spriteUL) ||
        ViewUtil::isOnScreenUnderHorizon(spriteUR) ||
        ViewUtil::isOnScreenUnderHorizon(spriteDR) ||
        ViewUtil::isOnScreenUnderHorizon(spriteDL))
            mLevel->pushSprite(&mSprites[spriteIndexForCurrentDirection]);
}

void AIKart::update() {
    //return;
    Point nextPoint = mPath.points[mNextPathPointIndex];

    double remainingTurnAngleForNextPoint = (directionalPoint() - nextPoint).rad;

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


    // d2("AI Kart Decision: ",
    //     "\n\tGo forward: ", shouldGoForward,
    //     "\n\tGo backward: ", shouldGoBackward,
    //     "\n\tGo left: ", shouldGoLeft,
    //     "\n\tGo right: ", shouldGoRight
    // );
    Kart::update(shouldGoForward, shouldGoBackward, shouldGoLeft, shouldGoRight);

	const double POINT_REACHED_DISTANCE_EPSILON = 3.5;
	const double distanceToNextPoint = GeometryUtil::distanceBetweenTwoPoints(mDirectionalPoint.position, nextPoint);
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
		// d("Distance to next point: [", mNextPathPointIndex ,"]", distanceToNextPoint);
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
    mNextPathPointIndex = /*0*/ 0;
}

const char * AIKart::logTag() {
	return LOG_TAG;
}

bool AIKart::canLog() {
	return 1;
}