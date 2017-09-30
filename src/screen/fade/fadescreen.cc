#include <SFML/Graphics.hpp>
#include "fadescreen.h"
#include "const.h"
#include "mathutil.h"

#define FADE_ALPHA_HIGH 255
#define FADE_ALPHA_LOW 1

FadeScreen::FadeScreen( sf::RenderWindow *window, KeyboardManager *keysManager,
                        int fadeInDuration, int fadeOutDuration)
                        : Screen(window, keysManager) {
    mFadeInDuration = fadeInDuration;
    mFadeOutDuration = fadeOutDuration;

    mFadeInTimer.initialize(mFadeInDuration, this, &FadeScreen::fadeInFinished);
    mFadeInTimer.start();

	sf::Image overlayImage;
	overlayImage.create(Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT, sf::Color::Black);
	mOverlayTexture = new sf::Texture();
	mOverlayTexture->loadFromImage(overlayImage);

	mOverlaySprite = new sf::Sprite();
	mOverlaySprite->setTexture(*mOverlayTexture);
}

FadeScreen::~FadeScreen() {
	delete mOverlaySprite;
	delete mOverlayTexture;
}

void FadeScreen::update() {
	if (mFadeInTimer.isRunning())
        mFadeInTimer.update();
    else if (mFadeOutTimer.isRunning())
        mFadeOutTimer.update();
}

void FadeScreen::render() {
    if (!isFading())
        return;

    int fadeDuration;
    int currentFadeTicks;
    int alphaFrom;
    int alphaTo;

    if (mFadeInTimer.isRunning()) {
        fadeDuration = mFadeInDuration;
        currentFadeTicks = mFadeInTimer.currentTicks();
        alphaFrom = FADE_ALPHA_HIGH;
        alphaTo = FADE_ALPHA_LOW;
    }
    else if (mFadeOutTimer.isRunning()) {
        fadeDuration = mFadeOutDuration;
        currentFadeTicks = mFadeOutTimer.currentTicks();
        alphaFrom = FADE_ALPHA_LOW;
        alphaTo = FADE_ALPHA_HIGH;
    }

	int alphaValueForCurrentUpdate = MathUtil::changeRange(
		MathUtil::Range {0, fadeDuration},
		MathUtil::Range {alphaFrom, alphaTo},
		TimeUtil::updatesToMillis(currentFadeTicks)
	);

	mOverlaySprite->setColor(sf::Color(0, 0, 0, alphaValueForCurrentUpdate));
	mWindow->draw(*mOverlaySprite);
}

void FadeScreen::fadeOut() {
    mFadeOutTimer.initialize(mFadeOutDuration, this, &FadeScreen::fadeOutFinished);
    mFadeOutTimer.start();
}

bool FadeScreen::isFadingIn() {
    return mFadeInTimer.isRunning();
}

bool FadeScreen::isFadingOut() {
    return mFadeOutTimer.isRunning();
}

bool FadeScreen::isFading() {
    return isFadingIn() || isFadingOut();
}

void FadeScreen::fadeInFinished() {

}

void FadeScreen::fadeOutFinished() {

}