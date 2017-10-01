#ifndef MAP_MOVING_ENTITY_H
#define MAP_MOVING_ENTITY_H

#include "mapentity.h"

/**
 * Represents an abstract entity placed into a map that have a direction.
 */
class MapMovingEntity : public MapEntity {
protected:
	/**
	 * Returns the direction of this entity.
	 * The angle is between 0 and 2PI, clockwise, starting from the top y-axis.
	 * @return the direction of this entity.
	 */
	Angle virtual direction() = 0;
};

#endif // MAP_ENTITY_H
