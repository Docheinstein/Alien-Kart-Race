#include <cmath>
#include <SFML/Graphics.hpp>
#include "kart.h"
#include "level.h"
#include "map.h"
#include "mathutil.h"

// Enable the flight mode for moving through the map without
// slowness or bounds, at constant speed in the pressed direction.
#define FLIGHT_MODE 0

// Little angle (just to be sure that angles don't
// assume the 0 value (awful things might happen...))
const double ANGLE_RAD_EPSILON = (double)1 / (1 << 9);

// Factor of impact when bouncing with a wall
const double COLLISION_BOUNCE_FACTOR = 0.7;

// How much faster does the bouce force reduce its magnitude
const double COLLISION_BOUNCE_REDUCTION_FACTOR = (double)1 / 120;

// Factor of impact wheh crashing with a kart
const double COLLISION_CRASH_FACTOR = 0.2;

// How much faster does the crash force reduce its magnitude
const double COLLISION_CRASH_REDUCTION_FACTOR = (double)1 / (1 << 9);

// How much does the angular increment decrease while skidding.
const double SKID_ANGULAR_INCREMENT_DECREASE_FACTOR = 0.45;

// The speed factor the kart assumes when skid at the maximum turning angle.
const double SKID_SPEED_BOUND_LOWEST = 0.8;

// The speed factor the kart assumes when skid at the minimum turning angle.
const double SKID_SPEED_BOUND_HIGHEST = 1.0;

Kart::Kart(Level * level, const char *kartName, sf::Color *kartColor) {
	mLevel = level;
	mName = kartName;
	mColor = kartColor;

	mDirectionalPoint.position = Point {0, 0};
	mDirectionalPoint.direction = Angle {ANGLE_RAD_EPSILON};
	mSpeed = 0;
	mWheelTurning = 0;
}

Kart::~Kart() {
	delete [] mSprites;
	delete mLeaderboardSprite;
	delete mColor;
}

// ------------------------
// PUBLIC -----------------
// ------------------------

Point Kart::position() {
	return mDirectionalPoint.position;
}

Angle Kart::direction() {
	return mDirectionalPoint.direction;
}

DirectionalPoint Kart::directionalPoint() {
	return mDirectionalPoint;
}

const char * Kart::name() {
	return mName;
}

sf::Color Kart::color() {
	return *mColor;
}

sf::Sprite *Kart::leadeboardSprite() {
	return mLeaderboardSprite;
}

void Kart::setPosition(const Point p) {
	mDirectionalPoint.position = p;
}

void Kart::setDirection(const Angle a) {
	mDirectionalPoint.direction = a;
}

void Kart::update(bool goForward, bool goBackward, bool goLeft, bool goRight) {
#if FLIGHT_MODE
	// When in flight mode the kart goes directly
	// to the wanted direction, ignoring any obstacle.
	double turning = 0;
	double speed = 0;

	if (goForward)
		speed = mParams.maxSpeed;
	else if (goBackward)
		speed = -mParams.maxSpeed;

	if (goLeft)
		turning = -0.01;
	else if (goRight)
		turning = 0.01;

	mDirectionalPoint.direction += turning;
	mDirectionalPoint.advance(speed);
#else
	Map::TileEvent currentTileEvent = mLevel->map()->tileEvent(position());
	bool skidding = isSkidding();

	double maxWheelTurning = mParams.maxWheelTurning;
	double maxSpeed = mParams.maxSpeed;
	double minSpeed = mParams.minSpeed;
	double accelerationFactor = mParams.accelerationFactor;
	double backAccelerationFactor = mParams.backwardAccelerationFactor;
	double turningFactor = skidding ?
		mParams.steeringWheelTurningFactorSkidding :
		mParams.steeringWheelTurningFactor;
	double returnFactor = mParams.steeringWheelReturnFactor;

	// Reduces the parameters if on a slow type tile
	if (currentTileEvent == Map::TileEvent::Slow) {
		maxWheelTurning *= mParams.slownessFactor;
		maxSpeed *= mParams.slownessFactor;
		minSpeed *= mParams.slownessFactor;
		accelerationFactor *= mParams.slownessFactor;
		backAccelerationFactor *= mParams.slownessFactor;
		turningFactor *= mParams.slownessFactor;
		returnFactor *= mParams.slownessFactor;
	}

	// Left / Right
	// Ensures the wheel angle is under the maximum
	if (goRight) {
		mWheelTurning += turningFactor;
		mWheelTurning = std::min(mWheelTurning, maxWheelTurning);
	}
	else if (goLeft) {
		mWheelTurning -= turningFactor;
		mWheelTurning = std::max(mWheelTurning, -maxWheelTurning);
	}
	else {
		// If neither left nor right is pressed, let the steering wheel
		// return to its natural assets (epsilon degrees)
		if (mWheelTurning != 0) {
			if (mWheelTurning > 0) {
				mWheelTurning -= returnFactor;
				mWheelTurning = std::max(mWheelTurning, (double) 0);
			}
			else {
				mWheelTurning += returnFactor;
				mWheelTurning = std::min(mWheelTurning, (double) 0);
			}
		}
	}

	double previousSpeed = mSpeed;
	double previousRow = mDirectionalPoint.position.y;
	double previousCol = mDirectionalPoint.position.x;

	// Forward / Backward
	if (goForward) {
		// Accellerating

		// Was going faster than max speed (was on the road and is going to the grass)
		if (mSpeed > maxSpeed)
			mSpeed -= mParams.decelerationFactor;
		// Can go faster
		else if (mSpeed + accelerationFactor < maxSpeed)
			mSpeed += accelerationFactor;
	}
	else if (goBackward) {
		// Was going forward => brake
		if (mSpeed > 0) {
			mSpeed -= mParams.brakeFactor;
			mSpeed = std::max((double) 0, mSpeed);
		}
		// Was stationary => go backward
		else {
			// Was going faster than max speed (was on the road and is going to the grass)
			if (mSpeed < minSpeed)
				mSpeed += mParams.decelerationFactor;
			// Can go faster
			else if (mSpeed - backAccelerationFactor > minSpeed)
				mSpeed -= backAccelerationFactor;
		}
	}
	// Neither forward nor backward is requested => decellerate in the direction
	// opposite to the current one
	else {
		// Was going forward => decrease speed
		if (mSpeed > 0) {
			mSpeed -= mParams.decelerationFactor;
			// Ensure the speed is not below 0
			mSpeed = std::max(mSpeed, (double) 0);
		}
		// Was going backward => increase speed
		else {
			mSpeed += mParams.decelerationFactor;
			// Ensure the speed is not above 0
			mSpeed = std::min(mSpeed, (double) 0);
		}
	}

	// Calculates the angular speed that depends on the current
	// angle of the steering wheel and add it to the direction

	double turningRadius = 1 / sin(mWheelTurning);
	double turningPerimeter = 2 * M_PI * turningRadius;
	double angularSpeed = mSpeed / turningPerimeter;
	double angularIncrement = angularSpeed * 2 * M_PI;

	if (skidding)
		angularIncrement *= SKID_ANGULAR_INCREMENT_DECREASE_FACTOR;

	mDirectionalPoint += angularIncrement;

	advanceInCurrentDirection();

	// If we are traversing an unpassable object, discard any
	// calc about direction and speed and starts a bounce
	if (currentTileEvent == Map::TileEvent::Unpassable) {
		mSpeed = previousSpeed;
		mDirectionalPoint.position.y = previousRow;
		mDirectionalPoint.position.x = previousCol;
		bounce();
	}

	// Ensure direction is not 0, just in case (not a good number
	//  for division and trigonometric function)
	if (mDirectionalPoint.direction.rad == 0)
		mDirectionalPoint.direction.rad = ANGLE_RAD_EPSILON; // Little value > 0

	// d2("Updating kart [", name(), "] ",
	// 	"\n---- Requested movement ----",
	// 	"\n\tForward            : ", goForward,
	// 	"\n\tBackward           : ", goBackward,
	// 	"\n\tLeft               : ", goLeft,
	// 	"\n\tRight              : ", goRight,
	// 	"\n---- Kart details ----",
	// 	"\n\tPosition:          :", position(),
	// 	"\n\tDirection:         :", direction(),
	// 	"\n\tSpeed:             :", mSpeed,
	// 	"\n\tWheel Turning:     :", mWheelTurning,
	// 	"\n---- Kart extra details ----",
	// 	"\n\tIs skidding        :", isSkidding(),
	// 	"\n\tIs bouncing        :", isBouncing(),
	// 	"\n\tOn Slow tile       :", (currentTileEvent == Map::TileEvent::Slow),
	// 	"\n\tOn Unpassable tile :", (currentTileEvent == Map::TileEvent::Unpassable)
	// );
#endif
}

void Kart::crash(Kart *k) {
	// Calculate the crash force by taking the angle betweeen the
	// coordinate of the two karts.
	DirectionalPoint kartsDirPoint(k->position(), position());

	mCollisionForces.push_back(
		CollisionVector {
			Crash,
		 	Vector  {kartsDirPoint.direction, k->mSpeed * k->mParams.weight  *
											COLLISION_CRASH_FACTOR / mParams.weight}
		}
	);
}

// ------------------------
// PROTECTED --------------
// ------------------------

void Kart::bounce() {
	// Do not bounce if its already bouncing.
	if (!isBouncing()) {
		// Add a bounce force that goes to the opposite direction to
		// the current one
		DirectionalPoint oppositeDirPoint(position(), Angle {(direction().rad + M_PI) });
		double bounceMagnitude = mSpeed * COLLISION_BOUNCE_FACTOR / mParams.weight;
		Vector oppositeVector {oppositeDirPoint.direction, bounceMagnitude};

		while (mLevel->map()->tileEvent(position()) == Map::TileEvent::Unpassable)
			mDirectionalPoint.advance(oppositeVector);

		// Reduce the current speed to 0: only the bounce force will act on this kart
		mSpeed = 0;
		mCollisionForces.push_back(CollisionVector { Bounce, oppositeVector });

		d("Kart [", name(), "] started a bounce: ", oppositeVector);
	}
}

bool Kart::isSkidding() {
	return abs(mWheelTurning) > abs(mParams.wheelTurningSkidPoint);
}

bool Kart::isBouncing() {
	bool bouncing = false;
    for (std::list<CollisionVector>::iterator i = mCollisionForces.begin();
        i != mCollisionForces.end() && !bouncing;
		i++) {
		bouncing |= ((*i).type == Bounce);
	}
	return bouncing;
}

void Kart::advanceInCurrentDirection() {
	double realSpeed = mSpeed;

	if (isSkidding()) {
		// While skidding the speed is reduced to a number between
		// the lowest and the highest bound, that depends on the current skid ratio.
		double speedBoundWhileSkidding = MathUtil::changeRange(
			Range {mParams.maxWheelTurning, mParams.wheelTurningSkidPoint},
			Range {SKID_SPEED_BOUND_LOWEST, SKID_SPEED_BOUND_HIGHEST},
			abs(mWheelTurning)
		);

		d2("Kart [", name(), "] reduces its speed because of the skid by a factor of: ",
			speedBoundWhileSkidding);

		realSpeed *= speedBoundWhileSkidding;
	}

	Vector finalForce;
	finalForce.direction = direction();
	finalForce.magnitude = realSpeed;

	// Determinate the final force by sum every force acting on the kart
    for (std::list<CollisionVector>::iterator collisionIter = mCollisionForces.begin();
        collisionIter != mCollisionForces.end();
		/* Do not increment by default */) {
		d2("Kart [", name(), "]; Force ", (*collisionIter).v,
			" will contribuite to final force vector");
		finalForce += (*collisionIter).v;

		double forceDecrement = mParams.weight;

		if ((*collisionIter).type == Bounce)
			forceDecrement *= COLLISION_BOUNCE_REDUCTION_FACTOR;
		else if ((*collisionIter).type == Crash)
			forceDecrement *= COLLISION_CRASH_REDUCTION_FACTOR;

		bool sign = (*collisionIter).v.magnitude > 0;
		(*collisionIter).v.magnitude +=
			((*collisionIter).v.magnitude < 0 ? 1 : -1 ) * forceDecrement;
		bool newSign = (*collisionIter).v.magnitude > 0;

		if (sign != newSign || (*collisionIter).v.magnitude == 0) {
			d2("Kart [", name(), "]; Force ", (*collisionIter).v, " finished its lifetime");
			collisionIter = mCollisionForces.erase(collisionIter);
		}
		else {
			collisionIter++;
		}
	}

	mDirectionalPoint.advance(finalForce);
}
