#ifndef KART_H
#define KART_H

#include <list>
#include "mapmovingentity.h"
#include "loggerinterface.h"

namespace sf {
	class Texture;
	class Sprite;
	class Color;
}

class Level;

/**
 * Base entity representing a kart.
 * This is responsible for handle stuff common between different types of karts.
 */
class Kart : public MapMovingEntity, public LoggerInterface {
public:

	/** Encapsulates the parameters of the karts. */
    struct KartParams {
		/** Maximum speed (tile / cycle). */
		double maxSpeed;

		/** Minimum speed while going backward expressed as positive number (tile / cycle).*/
		double minSpeed;

		/** Maximum angle of the steering wheel (rad). */
		double maxWheelTurning;

		/**
		 * Accelleration that will be added to the speed
		 * each cycle while going forward (Tile / cycle^2).
		 */
		double accelerationFactor; //

		/**
		 * Decelleration that will be subtracted from the speed
		 * each cycle while doing nothing (neither accelerate nor braking) (Tile / cycle^2).
		 */
		double decelerationFactor;

		/**
		 * Accelleration that will be added to the speed
		 * each cycle while going backward (Tile / cycle^2).
		 */
		double backwardAccelerationFactor;

		/**
		 * Decelleration that will be subtracted from the speed
		 * each cycle while braking (Tile / cycle^2).
		 */
		double brakeFactor;

		/**
		 * Increment of the steering wheel angle that will be added
		 * each cycle while turning (rad / cycle).
		 */
		double steeringWheelTurningFactor;

		/**
		 * Increment of the steering wheel angle that will be added
		 * each cycle while turning and skidding (rad / cycle).
		 */
		double steeringWheelTurningFactorSkidding;

		/**
		 * Decrement for the steering wheel to return to its natural position.
		 * This is needed since when no direction is chosen by the kart,
		 * the steering wheel returns to its assets some time after (rad / cycle).
		 */
		double steeringWheelReturnFactor;

		/** Angle of the steering wheel from which this karts begins skidding (rad). */
		double wheelTurningSkidPoint;

		/**
		 * Factor of slowness that reduce every feautre of this karts while
		 * going on slow terrain (ratio between 0 and 1).
		 */
		double slownessFactor;

		/** Weight of the kart (immaginary unit). */
		double weight;
    };

	/**
	 * Creates a new kart.
	 * @param level     the level this karts will be part of.
	 * @param kartName  the name that will be given to this kart
	 *                  (and will be shown on the leaderboard).
	 * @param kartColor the color that will be given to this kart
	 *                  (used for leaderboard and minimap).
	 */
	Kart(Level * level, const char *kartName, sf::Color *kartColor);
	virtual ~Kart();

	Point position();
	Angle direction();

	/**
	 * Returns the directional point of this kart.
	 * The direction points contains both information about position and direction.
	 * @return the directional point of this kart.
	 */
	DirectionalPoint directionalPoint();

	/**
	 * Returns the name given to this kart.
	 * @return the name of this kart.
	 */
	const char *name();

	/**
	 * Returns the color given to this kart.
	 * @return the color of this kart.
	 */
	sf::Color color();

	/**
	 * Returns the icon that should be used for represents this kart on the leaderboard.
	 * @return the leaderboard icon of this kart.
	 */
	sf::Sprite * leadeboardSprite();

	/**
	 * Sets the position of this kart.
	 * @param p point representing the position of this kart on the map.
	 */
	void setPosition(Point p);

	/**
	 * Sets the direction of this kart.
	 * @param a angle representing the direction of this kart.
	 */
	void setDirection(Angle a);

	/**
	 * Updates the kart for the given choice.
	 * This methods handles changes of speed, direction and collision with
	 * objects in the map (bounds) but not with other karts.
	 * @param goForward  whether go forward.
	 * @param goBackward whether go backward.
	 * @param goLeft     whether go left.
	 * @param goRight    whether go right.
	 */
	void update(bool goForward, bool goBackward, bool goLeft, bool goRight);

	/**
	 * Handles the crash with the given kart.
	 * If this kart is already crashing with the given kart, the crash will
	 * be ignored, otherwise a collision force of a magnitute depending on
	 * the weights, the speeds, the positions and the direction will be added to this kart.
	 * Note that will be handled only the crash for this kart,
	 * for handle the crash for the given kart this method must be called on it too.
	 * @param k the kart this instance of kart is crashing with.
	 */
	void crash(Kart *k);

	/** Updates the state of this kart */
	virtual void update() = 0;

	/** Draws on the game window. */
	virtual void draw() = 0;

	/**
	 * Returns the size that this kart will have on the minimap.
	 * @return the size of this kart on the minimap, in pixel.
	 */
	virtual int minimapSize() = 0;

protected:
	/** Represents the type of collision this kart can be involved in. */
	enum CollisionType {
		/** The kart is bouncing (with a map bound). */
		Bounce,
		/** The kart is crashing (with another kart). */
		Crash
	};

	/**
	 * Wrapper of vector that represents a collision force.
	 * This encapsulates information about the type of collision.
	 */
	struct CollisionVector {
		CollisionType type;
		Vector v;
	} typedef CollisionVector;

	/** Level this kart is involved in. */
	Level *mLevel;

	/** Directional point that contains information about position and direction. */
	DirectionalPoint mDirectionalPoint;

	/** Params of this kart */
	KartParams mParams;

	/** Sprites of this kart to use for render on the map. */
	sf::Sprite *mSprites;

	/** Number of sprite of this kart to used for render on the map. */
	int mSpriteCount;

	/** Sprite to use for the leaderboard. */
	sf::Sprite *mLeaderboardSprite;

	/** Current turning of the steering wheel (rad). */
	double mWheelTurning;

	/** Current speed (tile / cycle). */
	double mSpeed;

	/** Name of this kart. */
	const char *mName;

	/** Color of this kart. */
	sf::Color *mColor;

	/** Forces applied to this kart (both bounce or crash forces). */
	std::list<CollisionVector> mCollisionForces;

	/**
	 * Handles the bounce with an object (typically a map object).
	 * The bounce won't be considered if the kart is already bouncing.
	 */
	void bounce();

	/**
	 * Returs whether this kart is skidding.
	 * @return whether is skidding.
	 */
	bool isSkidding();

	/**
	 * Returns whether this kart is bouncing.
	 * Note that a kart is considered as bouncing even if it is not
	 * actually in touch with the object the cause the bounce; instead
	 * it is considered as bouncing if the bounce force is still applied to the kart.
	 * @return whether is bouncing.
	 */
	bool isBouncing();

	/**
	 * Advance the kart in the current direction,
	 * This evaluates the collision forces applied to the object, so
	 * might happen that the final force doesn't make this kart
	 * going to the current direction (i.e. while bouncing or crashing)-
	 */
	void advanceInCurrentDirection();

    const char *logTag();
    bool canLog();
};

#endif // KART_H