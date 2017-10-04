#include <SFML/Graphics.hpp>
#include <limits>
#include "playerkart.h"
#include "perspectiveutil.h"
#include "viewutil.h"
#include "mathutil.h"
#include "minimap.h"
#include "level.h"
#include "config.h"


#define LOG_TAG "{PlayerKart} "
#define CAN_LOG 1

PlayerKart::PlayerKart(Level * level, const char *kartName, sf::Color *kartColor)
            : Kart(level, kartName, kartColor) {}

// ------------------------
// PUBLIC -----------------
// ------------------------

void PlayerKart::update() {
    // Update the kart as the player wants.
	Kart::update(
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
	);
}

void PlayerKart::draw() {
    // Find the sprite by taking the current direction into consideration.
    int renderedSpriteIndex = MathUtil::rangeBelongingTo(
        mRenderTurningRanges, TURNING_RANGE_COUNT, mWheelTurning);

    mLevel->pushSprite(&mSprites[renderedSpriteIndex]);
}

int PlayerKart::minimapSize() {
    return Config::MINIMAP_SIZE / 64;
}

// ------------------------
// PROTECTED --------------
// ------------------------

// Note that perspectivePoint() and perspectiveScale() are implemented as functions
// that wrap a static constant since this way is guaranteed that when the function is called
// the static constant those relies upon are initialized.
// Otherwise the initialization order of the static constants is undefined and might
// happen that the perspective point initialization is prior to another essential
// initialization (e.g. ViewUtil::BASE_POINT), that will cause wrong initialization.

Point PlayerKart::perspectivePoint() {
    static Point PERSPECTIVE_POINT = PerspectiveUtil::perspectivePoint(
        ViewUtil::BASE_POINT,
        ViewUtil::cameraPoint(DirectionalPoint {ViewUtil::BASE_POINT, Angle {(double) 1 / (1 << 8)}}),
        ViewUtil::BASE_POINT,
        Angle {(double) 1 / (1 << 8)},
        ViewUtil::HORIZON_LINE_Y,
        ViewUtil::RENDERED_TILE_SIZE
    );
    return PERSPECTIVE_POINT;
}

float PlayerKart::perspectiveScale() {
    static float PERSPECTIVE_SCALE = PerspectiveUtil::scaleForPerspectivePoint(
        Point {ViewUtil::BASE_POINT.x, perspectivePoint().y },
        Point {ViewUtil::BASE_POINT.x, ViewUtil::HORIZON_LINE_Y},
        ViewUtil::BASE_POINT
    );
    return PERSPECTIVE_SCALE;
}

void PlayerKart::initRenderTurningRanges() {
    int i = 0;

    // Each angle range is associated with a sprite.
    // Those will be used for render the sprite appropriate with
    // the current direction.
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
            perspectivePoint().x,
            perspectivePoint().y);
    }
    for (int i = 0; i < mSkidGasSpriteCount; i ++) {
        mSkidGasSprites[i].setPosition(
            perspectivePoint().x,
            perspectivePoint().y
        );
    }
}

const char * PlayerKart::logTag() {
	return LOG_TAG;
}

bool PlayerKart::canLog() {
	return CAN_LOG;
}