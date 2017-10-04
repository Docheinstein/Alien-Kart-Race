#include <SFML/Graphics.hpp>
#include "sfmlutil.h"

void SFMLUtil::centerOrigin(sf::Sprite *s) {
    s->setOrigin(s->getLocalBounds().width / 2, s->getLocalBounds().height / 2);
}

void SFMLUtil::centerOrigin(sf::Text *t) {
    t->setOrigin(t->getLocalBounds().width / 2, t->getLocalBounds().height / 2);
}