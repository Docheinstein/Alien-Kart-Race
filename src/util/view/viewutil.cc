#include <cmath>
#include "viewutil.h"
#include "config.h"

const Point ViewUtil::BASE_POINT = { (double) Config::WINDOW_WIDTH / 2,
                                      Config::WINDOW_HEIGHT + (double) Config::WINDOW_WIDTH / 12};
const double ViewUtil::HORIZON_LINE_Y =
                            (double) Config::WINDOW_HEIGHT / 2 + (double) Config::WINDOW_HEIGHT / 12;
const double ViewUtil::RENDERED_TILE_SIZE = 75 * Config::WINDOW_WIDTH / 320;
const double ViewUtil::TILE_SIZE = 15;
const double ViewUtil::CAMERA_TILE_BEHIND_PLAYER_COUNT = 1;

Point ViewUtil::cameraPoint(const DirectionalPoint &kartDirectionalPoint) {
    return cameraDirectionalPoint(kartDirectionalPoint).position;
}

DirectionalPoint ViewUtil::cameraDirectionalPoint(const DirectionalPoint &kartDirectionalPoint) {
    DirectionalPoint kartDirPointCopy {kartDirectionalPoint.position, kartDirectionalPoint.direction};
    kartDirPointCopy.advance((kartDirectionalPoint.direction.rad), -CAMERA_TILE_BEHIND_PLAYER_COUNT);
    return kartDirPointCopy;
}

bool ViewUtil::isOnScreen(const Point &p) {
	const bool insideScreenWidth = (p.x >= 0) && (p.x < Config::WINDOW_WIDTH);
	const bool insideScreenHeight = (p.y >= 0) && (p.y < Config::WINDOW_HEIGHT);

	return insideScreenWidth && insideScreenHeight;
}


bool ViewUtil::isUnderHorizon(const Point &p) {
	const bool underHorizonLine = (p.y >= HORIZON_LINE_Y);
    return underHorizonLine;
}

bool ViewUtil::isOnScreenUnderHorizon(const Point &p) {
	return isOnScreen(p) && isUnderHorizon(p);
}