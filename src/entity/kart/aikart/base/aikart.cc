#include <SFML/Graphics.hpp>
#include <cmath>
#include "aikart.h"
#include "fileutil.h"
#include "geometryutil.h"
#include "resourceutil.h"
#include "perspectiveutil.h"
#include "viewutil.h"
#include "mathutil.h"
#include "kartfactory.h"
#include "minimap.h"
#include "playerkart.h"
#include "config.h"
#include "level.h"

#define LOG_TAG "{AIKart} "
#define CAN_LOG 1

// Little angle (just to be sure that angles don't
// assume the 0 value (awful things might happen...))
const double ANGLE_RAD_EPSILON = (double)1 / (1 << 9);

AIKart::AIKart( Level * level, const char *kartName,
                sf::Color *kartColor, const char *pathName)
                 : Kart(level, kartName, kartColor) {
     mPathFilename = pathName;
     mDirectionalPoint.direction = Angle {ANGLE_RAD_EPSILON};
}

AIKart::~AIKart() {
    delete [] mPath.points;
}

// ------------------------
// PUBLIC -----------------
// ------------------------

void AIKart::update() {
    // The AI update basically look for the next point and take
    // the movmente decision based on the direction between the
    // kart position and the next point position.
    Point nextPoint = mPath.points[mNextPathPointIndex];

    double remainingTurnAngleForNextPoint = (directionalPoint() - nextPoint).rad;

    // When the direction of the kart is below this rad, the direction
    // is considered ok. Making this value low ensure that the kart take
    // exactly the right direction of the next point, but due other factor
    // like skidding and wheel turning return, might happen that the kart
    // never reaches the excpected direction and turn around the next point.
	const double ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON = 0.4;

    bool shouldGoBackward = false; // Never going backward
    bool shouldGoForward = true; // Always go forward
    bool shouldGoLeft;
    bool shouldGoRight;

    // Going in the right direction for the next point
    if (remainingTurnAngleForNextPoint < ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON)  {
        shouldGoLeft = shouldGoRight = false;
    }
    else {
        shouldGoRight = (remainingTurnAngleForNextPoint >= ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON &&
			 			remainingTurnAngleForNextPoint < M_PI);
        shouldGoLeft = (remainingTurnAngleForNextPoint >= M_PI - ALMOST_IN_RIGHT_DIRECTION_ANGLE_EPSILON &&
			 			 remainingTurnAngleForNextPoint <= M_PI * 2);
    }

    Kart::update(shouldGoForward, shouldGoBackward, shouldGoLeft, shouldGoRight);

	const double POINT_REACHED_DISTANCE_EPSILON = 3.5;
	const double distanceToNextPoint =
        GeometryUtil::distanceBetweenTwoPoints(mDirectionalPoint.position, nextPoint);
	if (distanceToNextPoint < POINT_REACHED_DISTANCE_EPSILON) {
			mNextPathPointIndex = (mNextPathPointIndex + 1) % mPath.pointCount;
		d(  "Kart [", name(), "] reached path point ",
            mNextPathPointIndex, " at index [", nextPoint, "]");
	}
	else {
		d2(  "Kart [", name(), "] Distance to next point ",
           mNextPathPointIndex, " at index [", nextPoint, "]: ", distanceToNextPoint);
	}
}


void AIKart::draw() {
    // Draws the player in the appropriate perspective point
    //  with the appropriate perspective scale.
    //
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

    // Adjust the angle differece by the adjustment for a point on the
    // screen position of this kart's perspective point
    double adjustmentAngleDiff = PerspectiveUtil::angleAdjustmentForPerspectivePoint(
        pp, ViewUtil::BASE_POINT, ViewUtil::HORIZON_LINE_Y, Config::WINDOW_WIDTH);

    double angleDiff = (direction() - playerKart->direction() - adjustmentAngleDiff).rad;

    // Estimate the sprite to use by looking at the angle difference
    // between the camera and the ai kart
    int spriteIndexForCurrentDirection =
        MathUtil::changeRangeToInt(
            Range {0, 2 * M_PI },
            Range {0, static_cast<double>(mSpriteCount)},
            angleDiff) % mSpriteCount;

	mSprites[spriteIndexForCurrentDirection].setPosition(pp.x, pp.y);
	mSprites[spriteIndexForCurrentDirection].setScale(scaleFactor, scaleFactor);

    sf::FloatRect spriteRect = mSprites[spriteIndexForCurrentDirection].getGlobalBounds();
    Point spriteUL {spriteRect.left, spriteRect.top};
    Point spriteUR {spriteUL.x + spriteRect.width, spriteUL.y};
    Point spriteDR {spriteUR.x, spriteUL.y + spriteRect.height};
    Point spriteDL {spriteUR.x, spriteDR.y};

    // Add the sprite to the sprites to render the next render phase only if visibile
    if (ViewUtil::isOnScreenUnderHorizon(spriteUL) ||
        ViewUtil::isOnScreenUnderHorizon(spriteUR) ||
        ViewUtil::isOnScreenUnderHorizon(spriteDR) ||
        ViewUtil::isOnScreenUnderHorizon(spriteDL))
            mLevel->pushSprite(&mSprites[spriteIndexForCurrentDirection]);
}


int AIKart::minimapSize() {
    return Config::MINIMAP_SIZE / 128;
}

// ------------------------
// PROTECTED --------------
// ------------------------

void AIKart::initAIPath() {
    int fileRowCount, fileColCount;
    std::string pathFile = ResourceUtil::raw(mPathFilename.c_str());
	FileUtil::getMatrixSize<int>(pathFile.c_str(), fileRowCount, fileColCount);
	mPath.pointCount = fileColCount / 2;
    mPath.points = new Point[mPath.pointCount];

    void (AIKart::*pathFillFunctionPtr)(int, int, int) = &AIKart::pathArrayFillFunction;

    FileUtil::loadStructureFromFileKnowningSize<int, AIKart>(
        pathFile.c_str(), fileRowCount, fileColCount, this, pathFillFunctionPtr);

    d("Kart [", name(), "] Loaded AI path: ", pathFile, " of size: ", mPath.pointCount);

    for (int i = 0; i < mPath.pointCount; i++)
        d("Kart [", name(), "] Path point[", i, "]: ", mPath.points[i]);

    mNextPathPointIndex = 0;
}

void AIKart::pathArrayFillFunction(int readVal, int fileRow, int fileCol) {
	int halfCol = fileCol / 2;
	bool firstOfPair = (fileCol % 2 == 0);
	firstOfPair ?
		mPath.points[halfCol].y = readVal :
        mPath.points[halfCol].x = readVal;
}

const char * AIKart::logTag() {
	return LOG_TAG;
}

bool AIKart::canLog() {
	return CAN_LOG;
}