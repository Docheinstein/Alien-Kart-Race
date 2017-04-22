#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "level.h"

using namespace sf;

const int TARGET_UPDATES_PER_SECOND = 30;
const int TARGET_UPDATE_MS = 1000 / TARGET_UPDATES_PER_SECOND;

Game & Game::instance() {
	static Game instance;
	return instance;
}

Game::Game() {
	init();
}

Game::~Game() {
	delete mLevel;
}

void Game::start() {
	// Game loop
	Clock clock;
    while (mWindow->isOpen()) {
		clock.restart();

		pollEvents();
		update();
		render();

		long cycleMillis = clock.getElapsedTime().asMilliseconds();

		// There is enough time to sleep
		if (cycleMillis < TARGET_UPDATE_MS) {
			sleep(sf::milliseconds(TARGET_UPDATE_MS - cycleMillis));
		}
		else {
			std::cout << "Skipping frames!" << std::endl;
			while (cycleMillis > TARGET_UPDATE_MS) {
				clock.restart();
				pollEvents();
				update();
				cycleMillis = clock.getElapsedTime().asMilliseconds();
			}
			std::cout << "Frames skip ended" << std::endl;
		}
    }
}

Level * Game::level() {
	return mLevel;
}

sf::RenderWindow* Game::window() {
	return mWindow;
}

// Private
void Game::pollEvents() {
	Event event;
	while (mWindow->pollEvent(event)) {
		handleEvent(event);
	}
}

void Game::update() {
	mLevel->update();
}

void Game::render() {
	// Draw
	mWindow->clear();

	mLevel->render();

	mWindow->display();
}

// Private
void Game::init() {
    mWindow = new RenderWindow(VideoMode(640, 480), "SFML Race Game");
	mLevel = new Level();
}

void Game::handleEvent(const sf::Event &event) {
	// std::cout << "New SFML event" << std::endl;
	switch (event.type) {
	case sf::Event::Closed:
		mWindow->close();
		break;
	default:
		break;
	}
}
