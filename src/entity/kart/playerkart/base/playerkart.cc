#include <SFML/Graphics.hpp>
#include <limits>
#include "playerkart.h"
#include "perspectiveutil.h"
#include "viewutil.h"
#include "mathutil.h"
#include "minimap.h"
#include "level.h"


const Point PlayerKart::PERSPECTIVE_POINT = PerspectiveUtil::perspectivePoint(
    ViewUtil::BASE_POINT,
    ViewUtil::cameraPoint(DirectionalPoint {ViewUtil::BASE_POINT, Angle {(double) 1 / (1 << 8)}}),
    ViewUtil::BASE_POINT,
    Angle {(double) 1 / (1 << 8)},
    ViewUtil::HORIZON_LINE_Y,
    ViewUtil::RENDERED_TILE_SIZE
);

const float PlayerKart::PERSPECTIVE_SCALE = PerspectiveUtil::scaleForPerspectivePoint(
    Point {ViewUtil::BASE_POINT.x, PERSPECTIVE_POINT.y },
    Point {ViewUtil::BASE_POINT.x, ViewUtil::HORIZON_LINE_Y},
    ViewUtil::BASE_POINT
);

PlayerKart::PlayerKart(Level * level, const char *kartName, sf::Color *kartColor)
            : Kart(level, kartName, kartColor) {}

int PlayerKart::minimapSize() {
    return Minimap::MINIMAP_SIZE / 64;
}

void PlayerKart::update() {
	Kart::update(
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
	);
}

void PlayerKart::draw() {
    int renderedSpriteIndex = static_cast<int>(MathUtil::rangeBelongingTo(
        mRenderTurningRanges, TURNING_RANGE_COUNT, mWheelTurning));

    mLevel->pushSprite(&mSprites[renderedSpriteIndex]);
}

void PlayerKart::initRenderTurningRanges() {
    int i = 0;
    mRenderTurningRanges[i++] = {
            std::numeric_limits<double>::lowest(),
            -mParams.wheelTurningSkidPoint,
            mSpriteCount - 5
    };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint,
            -mParams.wheelTurningSkidPoint * 0.75,
            mSpriteCount - 4
    };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint * 0.75,
            -mParams.wheelTurningSkidPoint * 0.55,
            mSpriteCount - 3
    };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint * 0.55,
            -mParams.wheelTurningSkidPoint * 0.35,
            mSpriteCount - 2
    };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint * 0.35,
            -mParams.wheelTurningSkidPoint * 0.15,
            mSpriteCount - 1
    };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint * 0.15,
            mParams.wheelTurningSkidPoint * 0.15,
            0
    };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint * 0.15,
            mParams.wheelTurningSkidPoint * 0.35,
            1
    };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint * 0.35,
            mParams.wheelTurningSkidPoint * 0.55,
            2
    };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint * 0.55,
            mParams.wheelTurningSkidPoint * 0.75,
            3
    };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint * 0.75,
            mParams.wheelTurningSkidPoint,
            4
    };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint,
            std::numeric_limits<double>::max(),
            5
    };
}

void PlayerKart::initSprites() {
    for (int i = 0; i < mSpriteCount; i ++) {
        mSprites[i].setPosition(
            PERSPECTIVE_POINT.x,
            PERSPECTIVE_POINT.y);
    }
    for (int i = 0; i < mSkidGasSpriteCount; i ++) {
        mSkidGasSprites[i].setPosition(
            PERSPECTIVE_POINT.x,
            PERSPECTIVE_POINT.y
        );
    }
}