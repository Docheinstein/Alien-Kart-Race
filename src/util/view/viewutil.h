#ifndef VIEW_UTIL_H
#define VIEW_UTIL_H

#include "geometryentities.h"

/**
 * Provides utility functions and constants for
 * perspective view not depending from perspective calculation.
 */
class ViewUtil {
public:
    /** Base point of the perspective system. */
    static const Point BASE_POINT;

    /** Horizon line of the perspective system. */
	static const double HORIZON_LINE_Y;

    /** Size that will be used for render a single tile. */
	static const double RENDERED_TILE_SIZE;

    /** Actual tile size in the tileset. */
	static const double TILE_SIZE;

    /** Unit of distance between the camera and the player kart. */
    static const double CAMERA_TILE_BEHIND_PLAYER_COUNT;

    /**
     * Returns the directional point of the camera for the given kart direction point.
     * The returned directional point will have the same direction but a different
     * position depending on #CAMERA_TILE_BEHIND_PLAYER_COUNT.
     * @param  kartDirectionalPoint the directional point of the player kart.
     * @return                      the direction point of the camera.
     */
    static DirectionalPoint cameraDirectionalPoint(const DirectionalPoint &kartDirectionalPoint);

    /**
     * Returns the point of the camera for the given kart direction point.
     * The returned point depends on #CAMERA_TILE_BEHIND_PLAYER_COUNT.
     * @param  kartDirectionalPoint the directional point of the player kart.
     * @return                      the point of the camera.
     */
    static Point cameraPoint(const DirectionalPoint &kartDirectionalPoint);

    /**
     * Returns whether the given point is within the screen.
     * @param  p the point.
     * @return   whether the point is within the screen.
     */
    static bool isOnScreen(const Point &p);

    /**
     * Returns whether the given point is under the horizon line.
     * @param  p the point
     * @return    whether the point is under the horizon line.
     */
    static bool isUnderHorizon(const Point &p);

    /**
     * Returns whether the given point is withing the screen AND
     * under the horizon line.
     * @param  p the point
     * @return    whether the point is under the horizon line.
     */
    static bool isOnScreenUnderHorizon(const Point &p);
};

#endif // VIEW_UTIL_H