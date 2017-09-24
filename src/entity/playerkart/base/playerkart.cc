#include <SFML/Graphics.hpp>
#include <limits>
#include "playerkart.h"
#include "perspectiveutil.h"
#include "viewutil.h"
#include "mathutil.h"
#include "minimap.h"
#include "level.h"

const Point PlayerKart::PLAYER_KART_PERSPECTIVE_POINT = PerspectiveUtil::perspectivePoint(
        ViewUtil::BASE_POINT,
        ViewUtil::cameraPoint(Vector {ViewUtil::BASE_POINT, Angle {(double) 1 / (1 << 8)}}),
        ViewUtil::BASE_POINT,
        Angle {(double) 1 / (1 << 8)},
        ViewUtil::HORIZON_LINE_Y,
        ViewUtil::RENDERED_TILE_SIZE
);

const float PlayerKart::PLAYER_KART_PERSPECTIVE_SCALE = PerspectiveUtil::scaleForPerspectivePoint(
        Point {ViewUtil::BASE_POINT.x, PLAYER_KART_PERSPECTIVE_POINT.y },
        Point {ViewUtil::BASE_POINT.x, ViewUtil::HORIZON_LINE_Y},
        ViewUtil::BASE_POINT
    );

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

    Game::instance().level()->pushSprite(&mSprites[renderedSpriteIndex]);

	// Game::instance().window()->draw(mSprites[renderedSpriteIndex]);
	// d("Current player rendered sprite index: ", renderedSpriteIndex);
	// d("Current player sprite dim: ", mSprites[renderedSpriteIndex].getGlobalBounds().height);
}

void PlayerKart::initRenderTurningRanges() {
    int i = 0;
    mRenderTurningRanges[i++] = {
            std::numeric_limits<double>::lowest(),
            -mParams.wheelTurningSkidPoint,
            mSpriteCount - 3
        };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint,
            -mParams.wheelTurningSkidPoint / 2,
            mSpriteCount - 2
        };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint / 2,
            -mParams.wheelTurningSkidPoint / 4,
            mSpriteCount - 1
        };
    mRenderTurningRanges[i++] = {
            -mParams.wheelTurningSkidPoint / 4,
            mParams.wheelTurningSkidPoint / 4,
            0
        };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint / 4,
            mParams.wheelTurningSkidPoint / 2,
            1
        };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint / 2,
            mParams.wheelTurningSkidPoint,
            2
        };
    mRenderTurningRanges[i++] = {
            mParams.wheelTurningSkidPoint,
            std::numeric_limits<double>::max(),
            3
        };
}

void PlayerKart::initSprites() {
    for (int i = 0; i < mSpriteCount; i ++) {
        mSprites[i].setPosition(
            PLAYER_KART_PERSPECTIVE_POINT.x,
            PLAYER_KART_PERSPECTIVE_POINT.y);
    }
    for (int i = 0; i < mSkidGasSpriteCount; i ++) {
        mSkidGasSprites[i].setPosition(
            PLAYER_KART_PERSPECTIVE_POINT.x,
            PLAYER_KART_PERSPECTIVE_POINT.y
        );
    }
}