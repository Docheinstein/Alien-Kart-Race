#include <SFML/Graphics.hpp>
#include "game.h"
#include "keyboardmanager.h"
#include "screenmanager.h"
#include "launcher.h"
#include "const.h"

#define LOG_TAG "{Game} "
#define CAN_LOG 1

#define GAME_TITLE "Alien Kart Race"

Game::Game() {
	init();
}

Game::~Game() {
	delete mScreenManager;
	delete mAsyncKeyboardManager;
	delete mWindow;
}

// PUBLIC

void Game::start() {
	const int TARGET_UPDATE_MS = 1000 / Const::TARGET_UPDATES_PER_SECOND;

	// Represents the cycles between benchmarks.
	const int CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME = 300;

	int performedCycles = 0;
	long cyclesMillisSum = 0;

	// Game loop
	sf::Clock clock;
    while (mWindow->isOpen()) {
		clock.restart();

		pollEvents();
		update();
		render();

		int cycleMillis = static_cast<int>(clock.getElapsedTime().asMilliseconds());
		cyclesMillisSum += cycleMillis;
		// double cycleConsumedMillisPercentage = (double) cycleMillis / TARGET_UPDATE_MS * 100;
		// benchmark("Millis used for the cycle ", cycleMillis);
		// benchmark("Maximum amount of millis available per cycle: ", TARGET_UPDATE_MS);
		// benchmark("Cycle is taking ", cycleMillis, "ms; ", cycleConsumedMillisPercentage,
		// 			 "% of the available time.");

		// There is enough time to sleep (in order not to consume CPU)
		if (cycleMillis <= TARGET_UPDATE_MS)
			sleep(sf::milliseconds(TARGET_UPDATE_MS - cycleMillis));
		else {
			// We are out of time => do not render until we are under the maximum
			// amount of time for the cycle (frame skip)
			int skippedFrames = 0;
			while (cycleMillis > TARGET_UPDATE_MS) {
				skippedFrames++;
				clock.restart();
				pollEvents();
				update();
				cycleMillis = static_cast<int>(clock.getElapsedTime().asMilliseconds());
			}
			bench("Skipped ", skippedFrames, " frames");
		}

		performedCycles++;
		if (performedCycles >= CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME) {
			const double avgMillis = cyclesMillisSum / (double) performedCycles;
			double avgMillisPercentage = (double) avgMillis / TARGET_UPDATE_MS * 100;
			bench(
			 "Avarage ms taken by lasts ", CYCLE_TO_PERFORM_FOR_NOTIFY_AVG_TIME,
			 " cycles: ", avgMillis,
			 "ms; ", avgMillisPercentage, "% of the available time.");
			performedCycles = 0;
			cyclesMillisSum = 0;
		}
    }
}

sf::RenderWindow* Game::window() {
	return mWindow;
}

KeyboardManager * Game::keysManager() {
	return mAsyncKeyboardManager;
}

// PRIVATE

void Game::init() {
    mWindow = new sf::RenderWindow(sf::VideoMode(
		Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT), GAME_TITLE);
	mAsyncKeyboardManager = new KeyboardManager();
	mScreenManager = new ScreenManager();
	mScreenManager->setScreen(new Launcher(mWindow, mAsyncKeyboardManager));
}

void Game::update() {
	mScreenManager->update();
}

void Game::render() {
	mWindow->clear(sf::Color::White);
	mScreenManager->render();
	mWindow->display();
}

void Game::pollEvents() {
	sf::Event event;
	while (mWindow->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mWindow->close();
			break;
		case sf::Event::KeyPressed:
			mAsyncKeyboardManager->notifyKeyPressed(event.key.code);
			break;
		default:
			break;
		}
	}
}

const char * Game::logTag() {
	return LOG_TAG;
}

bool Game::canLog() {
	return CAN_LOG;
}
