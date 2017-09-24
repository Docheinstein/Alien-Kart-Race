#ifndef PERSPECTIVE_UTIL_H
#define PERSPECTIVE_UTIL_H

#include "geometryutil.h"

class PerspectiveUtil {
public:

    static Point perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            Angle objectRotation = 0
    );

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
                            Point &out_vp2,
                            Angle objectRotation = 0
    );

    static Point perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            const Point &vp1,
                            const Point &vp2,
                            Angle objectRotation = 0
    );

    static void calculateVanishPointsForAngle(
        const Angle &cameraAngle,
        double horizonY,
        const Point &basePoint,
        Point &out_vp1,
        Point &out_vp2,
        Angle objectRotation = 0
    );

    static double perspectiveHeight(
        const Point &perspectivePoint,
        const Point &vanishPoint,
        const Point &constructionPoint,
        double nonPerspectiveHeight
    );

    static double scaleForPerspectivePoint(
        const Point &perspectivePoint,
        const Point &vanishPoint,
        const Point &constructionPoint
    );

private:
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
                            Point &out_pr,
                            Angle objectRotation = 0
    );
};

#endif