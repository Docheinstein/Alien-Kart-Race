#include <SFML/Graphics.hpp>
#include "kartfactory.h"
#include "config.h"
#include "viewutil.h"
#include "mathutil.h"
#include "resourceutil.h"
#include "playervenusian.h"
#include "logger.h"

#define LOG_TAG "{KartFactory} "

sf::Texture ** KartFactory::sKartsTextures = textures();
sf::Texture ** KartFactory::sKartsSkidGasTextures = skidGasTextures();
sf::Texture *  KartFactory::sKartsLeadboardFacesTextures = leadeboardFacesTextures();
sf::Texture *  KartFactory::sKartsPickerTextures = pickerTextures();

// ------------------------
// PUBLIC -----------------
// ------------------------

Kart::KartParams KartFactory::params(KartType type, double randomnessFactor) {
    Kart::KartParams params;
    switch (type) {
    case VenusianType: {
        params.maxSpeed = 0.15;
        params.minSpeed = -0.05;
        params.maxWheelTurning = 0.45;
        params.wheelTurningSkidPoint = 0.30;
        params.accelerationFactor = (double) 1 / (double) (1 << 10);
        params.backwardAccelerationFactor = (double) 1 / (double) (1 << 9);
        params.decelerationFactor = params.accelerationFactor * 1.6;
        params.brakeFactor = params.accelerationFactor * 5;

        params.steeringWheelTurningFactor = 0.018;
        params.steeringWheelTurningFactorSkidding = 0.036;
    	params.steeringWheelReturnFactor = 0.016;

        params.slownessFactor = 0.5;

        params.weight = 0.5;
        break;
    }
    default:
        warn(LOG_TAG, "Asking params for invalid kart type. Returning un-initialized params.");
        break;
    }

    // For each parameters generate a random number in the range and add
    // it to the default parameter.
    if (randomnessFactor != 0) {
        Range randRange {-randomnessFactor, randomnessFactor};
        params.maxSpeed +=
            params.maxSpeed * MathUtil::random(randRange);
        params.minSpeed +=
            params.minSpeed * MathUtil::random(randRange);
        params.maxWheelTurning +=
            params.maxWheelTurning * MathUtil::random(randRange);
        params.wheelTurningSkidPoint +=
            params.wheelTurningSkidPoint * MathUtil::random(randRange);
        params.accelerationFactor +=
            params.accelerationFactor * MathUtil::random(randRange);
        params.backwardAccelerationFactor +=
            params.backwardAccelerationFactor * MathUtil::random(randRange);
        params.decelerationFactor +=
            params.decelerationFactor * MathUtil::random(randRange);
        params.brakeFactor +=
            params.brakeFactor * MathUtil::random(randRange);

        params.steeringWheelTurningFactor +=
            params.steeringWheelTurningFactor * MathUtil::random(randRange);
        params.steeringWheelTurningFactorSkidding +=
            params.steeringWheelTurningFactorSkidding * MathUtil::random(randRange);
        params.steeringWheelReturnFactor +=
            params.steeringWheelReturnFactor * MathUtil::random(randRange);

        params.slownessFactor +=
            params.slownessFactor * MathUtil::random(randRange);
        params.weight +=
            params.weight * MathUtil::random(randRange);
    }

    return params;
}


int KartFactory::sprites(KartType type, sf::Sprite *&sprites) {
    int spriteCountForType = spriteCount(type);

    sprites = new sf::Sprite[spriteCountForType];

    for (int i = 0; i < spriteCountForType; i ++) {
        sprites[i].setTexture(KartFactory::sKartsTextures[type][i]);

        Point originPoint = spriteOrigin(type, i);
		sprites[i].setOrigin(originPoint.x, originPoint.y);
    }
    return spriteCountForType;
}

int KartFactory::skidGasSprites(KartType type, sf::Sprite *&sprites) {
    int spriteCountForType = spriteSkidGasCount(type);

    sprites = new sf::Sprite[spriteCountForType];

    for (int i = 0; i < spriteCountForType; i ++) {
        sprites[i].setTexture(KartFactory::sKartsSkidGasTextures[type][i]);

        Point originPoint = spriteSkidGasOrigin(type, i);
		sprites[i].setOrigin(originPoint.x, originPoint.y);
    }
    return spriteCountForType;
}

// Not parametrized since i thing that any karts will have the same
// scale factor in the future.
float KartFactory::scaleFactor(/* KartType type*/) {
    return 1.8 * Config::WINDOW_WIDTH / 320;
}

sf::Sprite * KartFactory::leadeboardFaceSprite(KartType type) {
    sf::Sprite *leadeboardFace = new sf::Sprite();
    leadeboardFace->setTexture(KartFactory::sKartsLeadboardFacesTextures[type]);
    return leadeboardFace;
}

sf::Sprite * KartFactory::pickerSprite(KartType type) {
    sf::Sprite *pickerSprite = new sf::Sprite();
    pickerSprite->setTexture(KartFactory::sKartsPickerTextures[type]);
    return pickerSprite;
}

const char * KartFactory::name(KartType type) {
    switch (type) {
    case VenusianType:
        return "Venusian";
    default:
        warn(LOG_TAG, "Asking name for invalid kart type. Returning empty string.");
        return "";
    }
}

Kart * KartFactory::playerKart(Level *level, KartType type) {
    const char *playerName = "Me";

    switch (type) {
    case VenusianType:
        return new PlayerVenusian(level, playerName, new sf::Color(sf::Color::Green));
    default:
        warn(LOG_TAG, "Asking player kart creation for invalid type. Returning NULL.");
        return NULL;
    }
}

// ------------------------
// PRIVATE ----------------
// ------------------------

sf::Texture ** KartFactory::textures() {
    sf::Texture **textures;

	textures = new sf::Texture*[KartType::_KartTypeCount];
	for (int r = 0; r < KartType::_KartTypeCount; r++) {

        KartType type = static_cast<KartType>(r);
        int spriteCountForType = spriteCount(type);
		textures[r] = new sf::Texture[spriteCountForType];

        for (int i =  0; i < spriteCountForType; i++) {
            textures[r][i].loadFromFile(
                ResourceUtil::image(spriteFilename(type, i).c_str()).c_str());
        }
    }

    return textures;
}

sf::Texture ** KartFactory::skidGasTextures() {
    sf::Texture **textures;

	textures = new sf::Texture*[KartType::_KartTypeCount];
	for (int r = 0; r < KartType::_KartTypeCount; r++) {

        KartType type = static_cast<KartType>(r);
        int spriteCountForType = spriteSkidGasCount(type);
        textures[r] = new sf::Texture[spriteCountForType];

        for (int i =  0; i < spriteCountForType; i++) {
            textures[r][i].loadFromFile(
                ResourceUtil::image(spriteSkidGasFilename(type, i).c_str()).c_str());
        }
    }

    return textures;
}

sf::Texture * KartFactory::leadeboardFacesTextures() {
    sf::Texture *textures = new sf::Texture[KartType::_KartTypeCount];
    for (int i = 0; i < KartType::_KartTypeCount; i++) {
        textures[i].loadFromFile(
            ResourceUtil::image(spriteLeadeboardFaceFilename(
                static_cast<KartType>(i)).c_str()).c_str());
    }
    return textures;
}

sf::Texture * KartFactory::pickerTextures() {
    sf::Texture *textures = new sf::Texture[KartType::_KartTypeCount];
    for (int i = 0; i < KartType::_KartTypeCount; i++) {
        textures[i].loadFromFile(
            ResourceUtil::image(spritePickerFilename(
                static_cast<KartType>(i)).c_str()).c_str());
    }
    return textures;
}

// Kart sprites

int KartFactory::spriteCount(KartType type) {
    switch (type) {
    case VenusianType:
        return 72;
    default:
        warn(LOG_TAG, "Asking sprite count for invalid kart type. Returning 0.");
        return 0;
    }
}

std::string KartFactory::spriteFilename(KartType type, int index) {
    switch (type) {
    case VenusianType: {
        std::string filename = "venusian_" + std::to_string(index) + ".png";
        return filename;
    }
    default:
        warn(LOG_TAG, "Asking sprite filename for invalid type kart type. Returning empty string.");
        return "";
    }
}

Point KartFactory::spriteOrigin(KartType type, int index) {
    switch (type) {
    case VenusianType:
        return Point { 24, 36 };
    default:
        warn(LOG_TAG, "Asking origin for invalid kart type sprite. Returning {0, 0}.");
        return Point { 0, 0 };
    }
}

// Skid gas sprites

int KartFactory::spriteSkidGasCount(KartType type) {
    switch (type) {
    case VenusianType:
        return 2;
    default:
        return 0;
    }
}

std::string KartFactory::spriteSkidGasFilename(KartType type, int index) {
    switch (type) {
    case VenusianType: {
        std::string leftright = index == 0 ? "left" : "right";
        std::string filename = "venusian_skid_" + leftright + "_gas.png";
        return filename;
    }
    default:
        warn(LOG_TAG, "Asking gas sprite filename for invalid type kart type. Returning empty string.");
        return "";
    }
}



Point KartFactory::spriteSkidGasOrigin(KartType type, int index) {
    switch (type) {
    case VenusianType:
        return Point { 26, -6 };
    default:
        warn(LOG_TAG, "Asking origin for invalid kart type sprite. Returning {0, 0}.");
        return Point { 0, 0 };
    }
}

std::string KartFactory::spriteLeadeboardFaceFilename(KartType type) {
    switch (type) {
    case VenusianType: {
        return "venusian_leaderboard_face_framed.png";
    }
    default:
        warn(LOG_TAG, "Asking leaderboard sprite filename for invalid type kart type. Returning empty string.");
        return "";
    }
}


std::string KartFactory::spritePickerFilename(KartType type) {
    switch (type) {
    case VenusianType: {
        return "venusian_level_picker.png";
    }
    default:
        warn(LOG_TAG, "Asking picker sprite filename for invalid type kart type. Returning empty string.");
        return "";
    }
}
