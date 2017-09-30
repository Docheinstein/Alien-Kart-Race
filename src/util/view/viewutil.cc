#include <cmath>
#include "viewutil.h"
#include "const.h"

const Point ViewUtil::BASE_POINT = { Const::WINDOW_WIDTH / 2, Const::WINDOW_HEIGHT + Const::WINDOW_WIDTH / 12};
const double ViewUtil::HORIZON_LINE_Y = Const::WINDOW_HEIGHT / 2 + Const::WINDOW_HEIGHT / 12;
const double ViewUtil::RENDERED_TILE_SIZE = 75 * Const::WINDOW_WIDTH / 320;
const double ViewUtil::TILE_SIZE = 15;
const double ViewUtil::CAMERA_TILE_BEHIND_PLAYER_COUNT = 1;

Point ViewUtil::cameraPoint(const DirectionalPoint &kartDirectionalPoint) {
    return cameraDirectionalPoint(kartDirectionalPoint).position;
}

DirectionalPoint ViewUtil::cameraDirectionalPoint(const DirectionalPoint &kartDirectionalPoint) {
    return {
        Point {
            kartDirectionalPoint.position.x - sin(kartDirectionalPoint.direction.rad) * CAMERA_TILE_BEHIND_PLAYER_COUNT,
            kartDirectionalPoint.position.y + cos(kartDirectionalPoint.direction.rad) * CAMERA_TILE_BEHIND_PLAYER_COUNT},
        Angle {
            kartDirectionalPoint.direction.rad
        }
    };
}

bool ViewUtil::isOnScreen(const Point &p) {
	const bool insideScreenWidth = (p.x >= 0) && (p.x < Const::WINDOW_WIDTH);
	const bool insideScreenHeight = (p.y >= 0) && (p.y < Const::WINDOW_HEIGHT);

	return insideScreenWidth && insideScreenHeight;
}


bool ViewUtil::isUnderHorizon(const Point &p) {
	const bool underHorizonLine = (p.y >= HORIZON_LINE_Y);
    return underHorizonLine;
}

bool ViewUtil::isOnScreenUnderHorizon(const Point &p) {
	return isOnScreen(p) && isUnderHorizon(p);
}