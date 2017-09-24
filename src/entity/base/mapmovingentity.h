#ifndef MAP_MOVING_ENTITY_H
#define MAP_MOVING_ENTITY_H

#include "mapentity.h"

class MapMovingEntity : public MapEntity {
protected:
	Angle virtual direction() = 0;
};

#endif // MAP_ENTITY_H
