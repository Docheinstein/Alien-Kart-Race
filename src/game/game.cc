#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "level.h"
#include "log.h"
#include "map.h"

using namespace sf;

const int TARGET_UPDATES_PER_SECOND = 60;
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
	const int CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME = 1000;
	int performedCycles = 0;
	long cyclesMillisSum = 0;

	// Game loop
	Clock clock;
    while (mWindow->isOpen()) {
		clock.restart();

		pollEvents();
		update();
		render();

		long cycleMillis = clock.getElapsedTime().asMilliseconds();
		cyclesMillisSum += cycleMillis;
		double cycleConsumedMillisPercentage = (double) cycleMillis / TARGET_UPDATE_MS * 100;
		// benchmark("Millis used for the cycle ", cycleMillis);
		// benchmark("Maximum amount of millis available per cycle: ", TARGET_UPDATE_MS);
		// benchmark("Cycle is taking ", cycleMillis, "ms; ", cycleConsumedMillisPercentage, "% of the available time.");
		// There is enough time to sleep
		if (cycleMillis < TARGET_UPDATE_MS) {
			sleep(sf::milliseconds(TARGET_UPDATE_MS - cycleMillis));

		}
		else {
			int skippedFrames = 0;
			while (cycleMillis > TARGET_UPDATE_MS) {
				skippedFrames++;
				clock.restart();
				pollEvents();
				update();
				cycleMillis = clock.getElapsedTime().asMilliseconds();
			}
			// benchmark("Skipped ", skippedFrames, " frames");
		}

		performedCycles++;
		if (performedCycles >= CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME) {
			const double avgMillis = cyclesMillisSum / (double) performedCycles;
			double avgMillisPercentage = (double) avgMillis / TARGET_UPDATE_MS * 100;
			benchmark(
			 "Avarage ms taken by lasts ", CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME,
			 " cycles: ", avgMillis,
			 "ms; ", avgMillisPercentage, "% of the available time.");
			performedCycles = 0;
			cyclesMillisSum = 0;
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
		// handleEvent(event);
		switch (event.type) {
		case sf::Event::Closed:
			mWindow->close();
			break;
		default:
			break;
		}
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
    mWindow = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Alien Kart Race");
	mLevel = new Level();
}

void Game::handleEvent(const sf::Event &event) {
	// std::cout << "New SFML event" << std::endl;

}
