#ifndef PERSPECTIVE_UTIL_H
#define PERSPECTIVE_UTIL_H

#include "geometryutil.h"

/** Provides utility functions for perspective view (points and scales). */
class PerspectiveUtil {
public:

    /**
     * Returns the point coordinate on the screen for the given parameters.
     * @param  mapPoint       the point representing the coordinate in the map (2D).
     * @param  cameraPoint    the camera point (observer) in the map (2D).
     * @param  basePoint      the baseline point on the screen.
     * @param  cameraAngle    the angle of observation from the camera to the map point.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  mapTileSize    the size that will be used for the tiles.
     * @return                the point with screen coordinates in perspective view.
     */
    static Point perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize
    );

    /**
     * Returns the point coordinate on the screen for the given parameters.
     * This method gives out the perspective points used for build the final point (pc, pr, vp1, vp2).
     * @param  mapPoint       the point representing the coordinate in the map (2D).
     * @param  cameraPoint    the camera point (observer) in the map (2D).
     * @param  basePoint      the baseline point on the screen.
     * @param  cameraAngle    the angle of observation from the camera to the map point.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  mapTileSize    the size that will be used for the tiles.
     * @param[out]  out_pc    the column point on the baseline used for build the perspective point.
     * @param[out]  out_pr    the row point on the baseline used for build the perspective point.
     * @param[out]  out_vp1   the vanish point on the horizon line opposite to the column point
     *                        used for build the perspective point.
     * @param[out]  out_vp2   the vanish point on the horizon line opposite to the row point
     *                        used for build the perspective point.
     * @return                the point with screen coordinates in perspective view.
     */
    static Point perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            Point &out_pc,
                            Point &out_pr,
                            Point &out_vp1,
                            Point &out_vp2
    );

    /**
     * Returns the point coordinate on the screen for the given parameters.
     * This method call suppose that the caller already knows the vanish points
     * (those can be calculated from a previous call of #perspectivePoint or #calculateVanishPointsForAngle).
     * This method is faster than the other since doesn't need to recalculate the vanish points.
     * Use this if there are many points to estimate (i.e. map rendering).
     * @param  mapPoint       the point representing the coordinate in the map (2D).
     * @param  cameraPoint    the camera point (observer) in the map (2D).
     * @param  basePoint      the baseline point on the screen.
     * @param  cameraAngle    the angle of observation from the camera to the map point.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  mapTileSize    the size that will be used for the tiles.
     * @param[in]  vp1        the vanish point on the horizon line opposite to the column point
     *                        used for build the perspective point.
     * @param[in]  vp2        the vanish point on the horizon line opposite to the row point
     *                        used for build the perspective point.
     * @return                the point with screen coordinates in perspective view.
     */
    static Point perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            const Point &vp1,
                            const Point &vp2
    );

    /**
     * Calculates the vanish points for the given parameters.
     * @param cameraAngle the angle of the observer.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  basePoint      the baseline point on the screen.
     * @param[out]  out_vp1   the vanish point on the horizon line opposite to the column point
     *                        usable for build the perspective point.
     * @param[out]  out_vp2   the vanish point on the horizon line opposite to the row point
     *                        usable for build the perspective point.
     */
    static void calculateVanishPointsForAngle(
        const Angle &cameraAngle,
        double horizonY,
        const Point &basePoint,
        Point &out_vp1,
        Point &out_vp2
    );

    /**
     * Returns the height that an object situated on the baseline
     * (which "is in contact" with the screen) will have if situated
     * on the given perspective point with the given vanish and construction
     * point (which must have already been calculated).
     * @param  perspectivePoint     the point on the screen perspective view.
     * @param  vanishPoint          a vanish point used for build the perspective point.
     * @param  constructionPoint    the construction point opposite to the given vanish point.
     * @param  nonPerspectiveHeight the height the object will have if in contact with the
     *                              screen in a non-perspective view.
     * @return                      the height of an object situated on the perspective point.
     */
    static double perspectiveHeight(
        const Point &perspectivePoint,
        const Point &vanishPoint,
        const Point &constructionPoint,
        double nonPerspectiveHeight
    );

    /**
     * Returns the scale that an object situated on the baseline
     * (which "is in contact" with the screen) will have if situated
     * on the given perspective point with the given vanish and construction
     * point (which must have already been calculated).
     * If the perspective point is over the baseline (as it ought to be) the
     * scale factor will be <= 1.0 (a further object is seen little).
     * @param  perspectivePoint     the point on the screen perspective view.
     * @param  vanishPoint          a vanish point used for build the perspective point.
     * @param  constructionPoint    the construction point opposite to the given vanish point.
     * @return                      the height of an object situated on the perspective point.
     */
    static double scaleForPerspectivePoint(
        const Point &perspectivePoint,
        const Point &vanishPoint,
        const Point &constructionPoint
    );

    /**
     * Returns an estimated adjustment angle for a perspective point.
     * In details: an object that rely on a perspective point on the screen
     * should be seen with different angles depending on the position on
     * the screen; this method returns an estimated value for that adjustment.
     * @param  perspectivePoint the the point on the screen perspective view.
     * @param  basePoint      the baseline point on the screen.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  screenWidth    the width of the screen.
     * @return                the angle adjustment in rad for the given perspective point.
     */
    static double angleAdjustmentForPerspectivePoint(
        const Point &perspectivePoint,
        const Point &basePoint,
        double horizonY,
        double screenWidth
    );

private:

    /**
     * Returns the point coordinate on the screen for the given parameters.
     * This method gives out the perspective points used for build the final point (pc, pr, vp1, vp2).
     * @param  mapPoint       the point representing the coordinate in the map (2D).
     * @param  cameraPoint    the camera point (observer) in the map (2D).
     * @param  basePoint      the baseline point on the screen.
     * @param  cameraAngle    the angle of observation from the camera to the map point.
     * @param  horizonY       the y-coordinate of the horizon line on the screen.
     * @param  mapTileSize    the size that will be used for the tiles.
     * @param[in]  vp1        the vanish point on the horizon line opposite to the column point
     *                        used for build the perspective point.
     * @param[in]  vp1        the vanish point on the horizon line opposite to the row point
     *                        used for build the perspective point.
     * @param[out]  out_pc    the column point on the baseline used for build the perspective point.
     * @param[out]  out_pr    the row point on the baseline used for build the perspective point.
     * @return                the point with screen coordinates in perspective view.
     */
    static Point perspectivePointInternal(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            const Point &vp1,
                            const Point &vp2,
                            Point &out_pc,
                            Point &out_pr
    );
};

#endif