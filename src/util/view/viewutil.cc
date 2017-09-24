#include <cmath>
#include "viewutil.h"

const Point ViewUtil::BASE_POINT = { Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT + Game::WINDOW_WIDTH / 12};
const double ViewUtil::HORIZON_LINE_Y = Game::WINDOW_HEIGHT / 2 + Game::WINDOW_HEIGHT / 12;
const double ViewUtil::RENDERED_TILE_SIZE = 75 * Game::WINDOW_WIDTH / 320;
const double ViewUtil::TILE_SIZE = 15;
const double ViewUtil::CAMERA_TILE_BEHIND_PLAYER_COUNT = 1;

Point ViewUtil::cameraPoint(const Vector &kartVector) {
    return cameraVector(kartVector).position;
}

Vector ViewUtil::cameraVector(const Vector &kartVector) {
    return {
        Point {
            kartVector.position.x - sin(kartVector.direction.rad) * CAMERA_TILE_BEHIND_PLAYER_COUNT,
            kartVector.position.y + cos(kartVector.direction.rad) * CAMERA_TILE_BEHIND_PLAYER_COUNT},
        Angle {
            kartVector.direction.rad
        }
    };
}

bool ViewUtil::isOnScreen(const Point &p) {
	const bool insideScreenWidth = (p.x >= 0) && (p.x < Game::WINDOW_WIDTH);
	const bool insideScreenHeight = (p.y >= 0) && (p.y < Game::WINDOW_HEIGHT);

	return insideScreenWidth && insideScreenHeight;
}


bool ViewUtil::isUnderHorizon(const Point &p) {
	const bool underHorizonLine = (p.y >= HORIZON_LINE_Y);
    return underHorizonLine;
}

bool ViewUtil::isOnScreenUnderHorizon(const Point &p) {
	return isOnScreen(p) && isUnderHorizon(p);
}