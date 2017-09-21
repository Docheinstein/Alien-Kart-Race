#include <SFML/Graphics.hpp>
#include "playerkart.h"

#include "minimap.h"


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
	PlayerKartSpriteType renderedSpriteType;

	if (mWheelTurning < -mParams.wheelTurningSkidPoint)
		renderedSpriteType = Left3;
	else if (mWheelTurning > mParams.wheelTurningSkidPoint)
		renderedSpriteType = Right3;

	else if (mWheelTurning < -mParams.wheelTurningSkidPoint / 2)
		renderedSpriteType = Left2;
	else if (mWheelTurning > mParams.wheelTurningSkidPoint / 2)
		renderedSpriteType = Right2;

	else if (mWheelTurning < -mParams.wheelTurningSkidPoint / 4)
		renderedSpriteType = Left1;
	else if (mWheelTurning > mParams.wheelTurningSkidPoint / 4)
		renderedSpriteType = Right1;

	else
		renderedSpriteType = Center;

	Game::instance().window()->draw(mSprites[renderedSpriteType]);
}