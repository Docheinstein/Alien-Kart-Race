#ifndef MAP_ENTITY_H
#define MAP_ENTITY_H

#include "geometryutil.h"

class MapEntity {
protected:
	Point virtual position() = 0;
};

#endif // MAP_ENTITY_H
