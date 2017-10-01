#ifndef MAP_ENTITY_H
#define MAP_ENTITY_H

#include "geometryentities.h"

/**
 * Represents an abstract entity placed into a map.
 * The unique constraints of this entity is have a position.
 */
class MapEntity {
protected:
	/**
	 * Returns the position of this entity on the map.
	 * @return the position of this entity.
	 */
	Point virtual position() = 0;
};

#endif // MAP_ENTITY_H
