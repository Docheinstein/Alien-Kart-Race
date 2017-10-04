#include <cmath>
#include "perspectiveutil.h"
#include "logger.h"
#include "game.h"

#define CAN_LOG 1
#define LOG_TAG "{PerspectiveUtil} "

Point PerspectiveUtil::perspectivePoint(
                        const Point &mapPoint,
                        const Point &cameraPoint,
                        const Point &basePoint,
                        const Angle &cameraAngle,
                        double horizonY,
                        double mapTileSize) {
    Point p1, p2, p3, p4;
    return perspectivePoint(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, horizonY, mapTileSize,
        p1, p2, p3, p4);
}

Point PerspectiveUtil::perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double mapTileSize,
                            const Point &vp1,
                            const Point &vp2) {
    Point pc, pr;

    calculateConstructionPoints(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, mapTileSize,
        vp1, vp2, pc, pr);

    return perspectivePoint(vp1, vp2, pc, pr);
}

Point PerspectiveUtil::perspectivePoint(
                        const Point &mapPoint,
                        const Point &cameraPoint,
                        const Point &basePoint,
                        const Angle &cameraAngle,
                        double horizonY,
                        double mapTileSize,
                        Point &out_pc,
                        Point &out_pr,
                        Point &out_vp1,
                        Point &out_vp2) {

    calculateVanishPointsForAngle(cameraAngle, horizonY, basePoint, out_vp1, out_vp2);

    calculateConstructionPoints(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, mapTileSize,
        out_vp1, out_vp2, out_pc, out_pr);

    return perspectivePoint(out_vp1, out_vp2, out_pc, out_pr);
}

void PerspectiveUtil::calculateVanishPointsForAngle(
    const Angle &cameraAngle,
    double horizonY,
    const Point &basePoint,
    Point &out_vp1,
    Point &out_vp2) {

    double angleRad = -cameraAngle.rad;

    // Calculate the vanish points
 	const double vanishPoint1X = basePoint.x - tan(M_PI / 2 - angleRad) * (basePoint.y - horizonY);
	const double vanishPoint2X = basePoint.x + tan(angleRad) * (basePoint.y - horizonY);

    // Copies the points outside the function for further use.
	out_vp1 = Point { vanishPoint1X, horizonY };
	out_vp2 = Point { vanishPoint2X, horizonY };

#if CAN_LOG
    debug(2, LOG_TAG, "Calculating vanish points for angle: ", cameraAngle,
            "\n\tVP1: ", out_vp1,
            "\n\tVP2: ", out_vp2
    );
#endif
}

Point PerspectiveUtil::perspectivePoint(
                            const Point &vp1,
                            const Point &vp2,
                            const Point &pc,
                            const Point &pr) {

	// Lines that join projected points on the baseline to the vanish points.
	const Line pc_vp2 = GeometryUtil::lineForTwoPoints(pc, vp2);
	const Line pr_vp1 = GeometryUtil::lineForTwoPoints(pr, vp1);

	// Definitive perspective point
	Point pp = GeometryUtil::intersectionForTwoLines(pc_vp2, pr_vp1);

#if CAN_LOG
    debug(2, LOG_TAG, "Calculating perspective point for: ",
        "\n\t VP1: ", vp1,
        "\n\t VP2: ", vp2,
        "\n\t pc: ", pc,
        "\n\t pr: ", pr,
        "\n\t Definitive perspective point: ", pp);
#endif

    return pp;
}

void PerspectiveUtil::calculateConstructionPoints(
                        const Point &mapPoint,
                        const Point &cameraPoint,
                        const Point &basePoint,
                        const Angle &cameraAngle,
                        double mapTileSize,
                        const Point &vp1,
                        const Point &vp2,
                        Point &out_pc,
                        Point &out_pr) {

    double cameraRad = -cameraAngle.rad;

	// Offset that the considered map point has in relation to the camera point
	const double relativeToCenterCol = cameraPoint.x - mapPoint.x;
	const double relativeToCenterRow = cameraPoint.y - mapPoint.y;

	// Offsets of the projected points on the baseline relative to the center.
	double baselineColOffset = (relativeToCenterCol) / cos (cameraRad);
	double baselineRowOffset = ((relativeToCenterRow) / cos(M_PI / 2 - cameraRad));

#if CAN_LOG
    debug(2, LOG_TAG, "\n\trelativeToCenterCol\t", relativeToCenterCol,
        "\n\trelativeToCenterRow\t", relativeToCenterRow,
        "\n\tbaselineColOffset\t", baselineColOffset,
        "\n\tbaselineRowOffset\t", baselineRowOffset);
#endif

    // Construction point that lie on the baseline (pc for the column, pr for the row)
    // Copies the points outside the function for further use.
	out_pc = Point { basePoint.x - baselineColOffset * mapTileSize, basePoint.y };
	out_pr = Point { basePoint.x + baselineRowOffset * mapTileSize, basePoint.y };

#if CAN_LOG
    debug(2, LOG_TAG, "Calculating construction points for angle: ", cameraAngle,
            "\n\tPC: ", out_pc,
            "\n\tPR: ", out_pr
    );
#endif
}

double PerspectiveUtil::perspectiveHeight(const Point &perspectivePoint, const Point &vanishPoint,
                                         const Point &constructionPoint, double nonPerspectiveHeight) {
    return scaleForPerspectivePoint(perspectivePoint, vanishPoint, constructionPoint) *
        nonPerspectiveHeight;
}

double PerspectiveUtil::scaleForPerspectivePoint(const Point &perspectivePoint, const Point &vanishPoint,
                                                 const Point &constructionPoint) {
    // Calculate how far is the perspective point from the vanish point in relation to
    // the distance between the vanish point and the opposite construction point.

    double distancePP_VP = GeometryUtil::distanceBetweenTwoPoints(perspectivePoint, vanishPoint);
    double distanceConstruction_VP = GeometryUtil::distanceBetweenTwoPoints(constructionPoint, vanishPoint);

    return distancePP_VP / distanceConstruction_VP;
}

double PerspectiveUtil::angleAdjustmentForPerspectivePoint(
        const Point &pp,
        const Point &basePoint,
        double horizonY,
        double screenWidth) {

    // The logic is that a point horizontaly far from the camera point (which is on center)
    // has got a greater angle adjustment.
    // On the opposite side, a point vertically far from the camera point
    // has got a lower angle adjustment.
    // The following calc is an estimation, its not precise from a perspective
    // point of view, but does work for this game purpose.

    const double HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT = 35 * M_PI / 180;
    const double BASELINE_ADJUSTMENT_FACTOR = 1;
    const double VERTICAL_CAMERA_ANGLE_ADJUSTMENT_FACTOR = 2;

    double horizontalAdjustmentRad = MathUtil::changeRange(
        Range {0, screenWidth },
        Range {-HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT, HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT },
        pp.x);

    double verticalAjustmentFactor = MathUtil::changeRange(
        Range {horizonY, basePoint.y },
        Range {VERTICAL_CAMERA_ANGLE_ADJUSTMENT_FACTOR, BASELINE_ADJUSTMENT_FACTOR},
        pp.y
    );

    return verticalAjustmentFactor * horizontalAdjustmentRad;
}