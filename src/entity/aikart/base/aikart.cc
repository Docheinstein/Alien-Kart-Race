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
#include "game.h"
#include "level.h"

#define LOG_TAG "{AIKart} "
#define CAN_LOG 1

int AIKart::minimapSize() {
    return Minimap::MINIMAP_SIZE / 128;
}

AIKart::AIKart() {
	mVector.direction = Angle {(double)1 / (1 << 9)};
}

AIKart::~AIKart() {

}

void AIKart::draw() {
        PlayerKart *playerKart = Game::instance().level()->playerKart();

        Point pc, pr, vp1, vp2;

        Point pp = PerspectiveUtil::perspectivePoint(
            position(),
            ViewUtil::cameraPoint(playerKart->vector()),
            ViewUtil::BASE_POINT,
            playerKart->direction(),
            ViewUtil::HORIZON_LINE_Y,
            ViewUtil::RENDERED_TILE_SIZE,
            pc,
            pr,
            vp1,
            vp2
        );
        d2("serious pp:", pp);
        d2("pc:", pc);
        d2("pr:", pr);
        d2("vp1:", vp1);
        d2("vp2:", vp2);

        Point pcForKart, prForKart, vp1ForKart, vp2ForKart;
    	Point dummypp = PerspectiveUtil::perspectivePoint(
            position(),
            ViewUtil::cameraPoint(playerKart->vector()),
            ViewUtil::BASE_POINT,
            playerKart->direction(),
            ViewUtil::HORIZON_LINE_Y,
            ViewUtil::RENDERED_TILE_SIZE,
            pcForKart,
            prForKart,
            vp1ForKart,
            vp2ForKart,
            direction()
        );
        d2("dummy pp:", dummypp);
        d2("pcForKart:", pcForKart);
        d2("prForKart:", prForKart);
        d2("vp1ForKart:", vp1ForKart);
        d2("vp2ForKart:", vp2ForKart);

            // Replce 1.8...
	    //const float SCALE_FACTOR = 4.4;
        const float kartScaleFactor = KartFactory::scaleFactor(KartFactory::KartType::Venusian);

        double perspectiveScaleFactor = PerspectiveUtil::scaleForPerspectivePoint(pp, vp2, pc);
        double scaleFactor = kartScaleFactor * perspectiveScaleFactor;

		// Sprite based on angle
        // Vector perspectiveVector(mVector.position, vp)
        //
		// double angleDiff = fmod(direction().rad + mWheelTurning - playerKart->direction().rad + 2 * M_PI, 2 * M_PI);
        // int spriteIndexForCurrentDirection = MathUtil::changeRangeToInt(MathUtil::Range {0, 2 * M_PI }, MathUtil::Range {0, mSpriteCount - 1}, angleDiff);
        //Angle angleDiff = direction() - playerKart->direction();
        //
        // Vector playerVector =   {PlayerKart::PLAYER_KART_PERSPECTIVE_POINT, vp2};
        // Vector kartVector =     {pp, vp2};
        // Angle angleDiff = kartVector.direction - playerVector.direction;

        // Angle angleDiff = GeometryUtil::angleDifference(
        //     Vector { pp, vp2 },
        //     PlayerKart::PLAYER_KART_PERSPECTIVE_POINT
        // );

        //Vector cameraVect = ViewUtil::cameraVector(playerKart->vector());
        // Point kartDirectionalVanishPoint = GeometryUtil::intersectionForTwoLines()
        double angleDiff;
#if 0
        Point auxPoint;
        Point vanishPoint = vp2;

        double radSum = playerKart->direction().rad + direction().rad;

        d2("RadSum: ", radSum);
        d2("PlayerKart dir: ", playerKart->direction().rad);
        d2("AIKart dir: ", direction().rad);
        if (playerKart->direction().rad > 0 && playerKart->direction().rad < M_PI / 2) {
            d2("PlayerKart between 0 and 90deg");
            if (radSum < M_PI) {
                auxPoint = vp2ForKart;
                d2("Using vp2_rot (1)");
            }
            else if (radSum < M_PI * 3 / 2) {
                auxPoint = pcForKart;
                d2("Using pc_rot (2)");
            }
            else if (radSum > M_PI * 3 / 2) {
                auxPoint = vp2ForKart;
                d2("Using vp2_rot (3)");
            }
        }
        else if (playerKart->direction().rad > M_PI / 2 && playerKart->direction().rad < M_PI) {
            d2("PlayerKart between 90 and 180deg");
            if (radSum < M_PI * 3 / 2) {
                auxPoint = vp2ForKart;
                d2("Using vp2_rot [1]");
            }
            else if (radSum > M_PI * 3 / 2 && radSum < M_PI * 5 / 4) {
                auxPoint = pcForKart;
                d2("Using pc_rot [2]");
            }
            else if (radSum > M_PI * 5 / 4) {
                auxPoint = vp2ForKart;
                d2("Using vp2_rot [3]");
            }
        }
        else {
            d2("Case not handled, quitting");
            return;
        }



        Vector ppVect  {pp, vanishPoint};
        Vector ppRotationVect {pp, auxPoint};

        // Vector ppVect = Vector {pp, vp2infinite};
        angleDiff = (ppRotationVect.direction - ppVect.direction - playerKart->direction()).rad;

        //pp = dummypp;

        d2("Angle diff: ", angleDiff);

        d2("ppVect: ", ppVect);
        d2("ppRotationVect: ", ppRotationVect);
        d2("");
#else
    angleDiff = (direction() - playerKart->direction()).rad;
#endif



        // GeometryUtil::angleDifference(ppVect, )
        // Angle angleDiff = Angle {GeometryUtil::angleDifference(
        //     cameraVect,
        //     mVector.position
        // ).rad};




        // d2("\nAngle diff:", angleDiff.rad);

        int spriteIndexForCurrentDirection =
            MathUtil::changeRangeToInt(
                MathUtil::Range {0, 2 * M_PI },
                MathUtil::Range {0, mSpriteCount},
                angleDiff) % mSpriteCount;


		mSprites[spriteIndexForCurrentDirection].setPosition(pp.x, pp.y);

		// mSprites[spriteIndexForCurrentDirection].setPosition(pp.x, pp.y);
		mSprites[spriteIndexForCurrentDirection].setScale(scaleFactor, scaleFactor);
        // d2("Venusian scale factor\t", kartScaleFactor);
        // d2("perspectiveScaleFactor\t",  perspectiveScaleFactor);
		// d2("perspectiveScale\t", scaleFactor);
		// d2("PP\t", pp);
		// d2("Current scaled AI sprite dim: ", mSprites[spriteIndexForCurrentDirection].getGlobalBounds().height);

		// mSprites[spriteIndexForCurrentDirection].setScale(1, 1);

        sf::FloatRect spriteRect = mSprites[spriteIndexForCurrentDirection].getGlobalBounds();
        Point spriteUL {spriteRect.left, spriteRect.top};
        Point spriteUR {spriteUL.x + spriteRect.width, spriteUL.y};
        Point spriteDR {spriteUR.x, spriteUL.y + spriteRect.height};
        Point spriteDL {spriteUR.x, spriteDR.y};
        if (ViewUtil::isOnScreenUnderHorizon(spriteUL) ||
            ViewUtil::isOnScreenUnderHorizon(spriteUR) ||
            ViewUtil::isOnScreenUnderHorizon(spriteDR) ||
            ViewUtil::isOnScreenUnderHorizon(spriteDL))
                Game::instance().level()->pushSprite(&mSprites[spriteIndexForCurrentDirection]);
		          //Game::instance().window()->draw(mSprites[spriteIndexForCurrentDirection]);


}

void AIKart::update() {
    return;
    Point nextPoint = mPath.points[mNextPathPointIndex];

	// double remainingTurnAngleForNextPoint = GeometryUtil::angleDifference(
	// 	GeometryUtil::angleBetweenTwoPoints(mVector.position, nextPoint),
	// 	mVector.direction
	// );

	double remainingTurnAngleForNextPoint = fmod(GeometryUtil::angleBetweenTwoPoints(mVector.position, nextPoint) - mVector.direction.rad + 2 * M_PI, 2 * M_PI);

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
	const double distanceToNextPoint = GeometryUtil::distanceBetweenTwoPoints(mVector.position, nextPoint);
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