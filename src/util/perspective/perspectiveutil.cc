#include <cmath>
#include "perspectiveutil.h"
#include "logger.h"
#include "game.h"

#define DBG 0

Point PerspectiveUtil::perspectivePoint(
                        const Point &mapPoint,
                        const Point &cameraPoint,
                        const Point &basePoint,
                        const Angle &cameraAngle,
                        double horizonY,
                        double mapTileSize,
                        Angle objectRotation) {
    Point p1, p2, p3, p4;
    return perspectivePoint(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, horizonY, mapTileSize,
        p1, p2, p3, p4, objectRotation);
}

Point PerspectiveUtil::perspectivePoint(
                            const Point &mapPoint,
                            const Point &cameraPoint,
                            const Point &basePoint,
                            const Angle &cameraAngle,
                            double horizonY,
                            double mapTileSize,
                            const Point &vp1,
                            const Point &vp2,
                            Angle objectRotation) {
    Point p1, p2;
    return perspectivePointInternal(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, horizonY, mapTileSize,
        vp1, vp2, p1, p2, objectRotation);
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
                        Point &out_vp2,
                        Angle objectRotation) {
    calculateVanishPointsForAngle(cameraAngle, horizonY, basePoint, out_vp1, out_vp2, objectRotation);
    return perspectivePointInternal(
        mapPoint, cameraPoint, basePoint,
        cameraAngle, horizonY, mapTileSize,
        out_vp1, out_vp2, out_pc, out_pr, objectRotation);
}

void PerspectiveUtil::calculateVanishPointsForAngle(
    const Angle &cameraAngle,
    double horizonY,
    const Point &basePoint,
    Point &out_vp1,
    Point &out_vp2,
    Angle objectRotation) {

    double angleRad = -cameraAngle.rad + objectRotation.rad;
    //double angleRad = objectRotation.rad != 0 ? objectRotation.rad : -cameraAngle.rad;

    // Calculate the vanish points
 	const double vanishPoint1X = basePoint.x - tan(M_PI / 2 - angleRad) * (basePoint.y - horizonY);
	const double vanishPoint2X = basePoint.x + tan(angleRad) * (basePoint.y - horizonY);
    // const double vanishPoint1X = basePoint.x - tan(M_PI / 2 - a.rad) * (basePoint.y - horizonY);

    // const double vanishPoint2X = basePoint.x + tan(a.rad) * (basePoint.y - horizonY);

    // Copies the points outside the function for further use.
	out_vp1 = Point { vanishPoint1X, horizonY };
	out_vp2 = Point { vanishPoint2X, horizonY };

#if DBG
    debug(1,"Calcultating vanish for angle: ", cameraAngle);
    debug(1,"(basePoint.y - horizonY)", (basePoint.y - horizonY));
    debug(1,"tan(M_PI / 2 - cameraAngle.rad): ", tan(M_PI / 2 - cameraAngle.rad));
    debug(1,"tan(cameraAngle.rad) ", tan(cameraAngle.rad) );
    debug(1,"tan(M_PI / 2 - cameraAngle.rad) * (basePoint.y - horizonY) ", tan(M_PI / 2 - cameraAngle.rad) * (basePoint.y - horizonY));
    debug(1,"tan(cameraAngle.rad) * (basePoint.y - horizonY)", tan(cameraAngle.rad) * (basePoint.y - horizonY));
    debug(1,"out_vp1 ", out_vp1);
    debug(1,"out_vp2 ", out_vp2);
#endif
}

Point PerspectiveUtil::perspectivePointInternal(
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
                            Angle objectRotation) {

    double cameraRad = -cameraAngle.rad;

	// Offset that the considered map point has in relation to the camera point
	const double relativeToCenterCol = cameraPoint.x - mapPoint.x;
	const double relativeToCenterRow = cameraPoint.y - mapPoint.y;

	// Offsets of the projected points on the baseline relative to the center.
	double baselineColOffset = (relativeToCenterCol) / cos (cameraRad);
	double baselineRowOffset = ((relativeToCenterRow) / cos(M_PI / 2 - cameraRad));

#if DBG
    debug(1,"\n\trelativeToCenterCol\t", relativeToCenterCol,
        "\n\trelativeToCenterRow\t", relativeToCenterRow,
        "\n\tbaselineColOffset\t", baselineColOffset,
        "\n\tbaselineRowOffset\t", baselineRowOffset);
#endif

    if (objectRotation.rad != 0) {

        cameraRad = -cameraAngle.rad;

        double rotationAngleDiff = (objectRotation - cameraAngle).rad;

        const double distancePointHorizonCol =
            relativeToCenterRow + baselineColOffset * cos(M_PI / 2 - cameraRad);
        const double distancePointHorizonRow =
            relativeToCenterCol + baselineRowOffset * cos(cameraRad);

        const double standardAngleTrigArea = fabs((distancePointHorizonRow * distancePointHorizonCol) / 2);
        const double standardAngleTrigHeight = /* h = 2A / b */
            fabs((standardAngleTrigArea * 2) / (baselineRowOffset + baselineColOffset));

        const double distanceOfPointProjectedOnHypotFromHorizonRow =
            (distancePointHorizonRow * distancePointHorizonRow) /
            (baselineColOffset + baselineRowOffset);

        const double distanceOfPointProjectedOnHypotFromHorizonOrigin =
            baselineRowOffset - distanceOfPointProjectedOnHypotFromHorizonRow;

        const double baselineRowOffsetRotatedPoint =
            standardAngleTrigHeight / tan(rotationAngleDiff);

        const double baselineColOffsetRotatedPoint =
            standardAngleTrigHeight / tan(M_PI / 2 - rotationAngleDiff);

        baselineColOffset = - distanceOfPointProjectedOnHypotFromHorizonOrigin + baselineColOffsetRotatedPoint;
        baselineRowOffset = distanceOfPointProjectedOnHypotFromHorizonOrigin + baselineRowOffsetRotatedPoint;

#if DBG
        debug(1, "Rotated object perspective details",
            "\n\tdistancePointHorizonCol\t", distancePointHorizonCol,
            "\n\tdistancePointHorizonRow\t", distancePointHorizonRow,
            "\n\tstandardAngleTrigArea\t", standardAngleTrigArea,
            "\n\tstandardAngleTrigHeight\t", standardAngleTrigHeight,
            "\n\tdistanceOfPointProjectedOnHypotFromHorizonRow\t", distanceOfPointProjectedOnHypotFromHorizonRow,
            "\n\tdistanceOfPointProjectedOnHypotFromHorizonOrigin\t", distanceOfPointProjectedOnHypotFromHorizonOrigin,
            "\n\tbaselineColOffsetRotatedPoint\t", baselineColOffsetRotatedPoint,
            "\n\tbaselineRowOffsetRotatedPoint\t", baselineRowOffsetRotatedPoint,
            "\n\tbaselineColOffset\t", baselineColOffset,
            "\n\tbaselineRowOffset\t", baselineRowOffset
        );
#endif

    }


    // Construction point that lie on the baseline (pc for the column, pr for the row)
    // Copies the points outside the function for further use.
	out_pc = { basePoint.x - baselineColOffset * mapTileSize, basePoint.y };
	out_pr = { basePoint.x + baselineRowOffset * mapTileSize, basePoint.y };

	// Line that join projected points on the baseline to the vanish points.
	const Line pc_vp2 = GeometryUtil::lineForTwoPoints(out_pc, vp2);
	const Line pr_vp1 = GeometryUtil::lineForTwoPoints(out_pr, vp1);

	// Definitive perspective point
	Point pp = GeometryUtil::intersectionForTwoLines(pc_vp2, pr_vp1);

#if DBG
    debug(1, "Calculated pp for",
        "\nmapPoint\t", mapPoint,
        "\ncameraPoint\t", cameraPoint,
        "\nbasePoint\t", basePoint,
        "\ncameraAngle\t", cameraAngle,
        "\nhorizonY\t", horizonY,
        "\nmapTileSize\t", mapTileSize,
        "\nvp1\t", vp1,
        "\nvp2\t", vp2,
        "\nout_pc\t", out_pc,
        "\nout_pr\t", out_pr,
        "\n--\t-- PP: ", pp);
#endif
    return pp;
}



#if 0
void PerspectiveUtil::calculateVanishPointsForAngle(
    const Angle &cameraAngle,
    double horizonY,
    const Point &basePoint,
    Point &out_vp1,
    Point &out_vp2,
    Angle objectRotation) {

    const Angle &a = objectRotation.rad != 0 ? objectRotation : cameraAngle;

    // Calculate the vanish points
 	const double vanishPoint1X = basePoint.x - tan(M_PI / 2 + a.rad) * (basePoint.y - horizonY);
	const double vanishPoint2X = basePoint.x + tan(-a.rad) * (basePoint.y - horizonY);

    // Copies the points outside the function for further use.
	out_vp1 = Point { vanishPoint1X, horizonY };
	out_vp2 = Point { vanishPoint2X, horizonY };
}

Point PerspectiveUtil::perspectivePointInternal(
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
                            Angle objectRotation) {
	// Offset that the considered map point has in relation to the camera point
	const double relativeToCenterCol = cameraPoint.x - mapPoint.x;
	const double relativeToCenterRow = cameraPoint.y - mapPoint.y;

	// Offsets of the projected points on the baseline relative to the center.
	const double baselineColOffset = (relativeToCenterCol) / cos (-cameraAngle.rad);
	const double baselineRowOffset = (relativeToCenterRow) / cos(M_PI / 2 + cameraAngle.rad);



    // Construction point that lie on the baseline (pc for the column, pr for the row)
    // Copies the points outside the function for further use.
	out_pc = { basePoint.x - baselineColOffset * mapTileSize, basePoint.y };
	out_pr = { basePoint.x + baselineRowOffset * mapTileSize, basePoint.y };

	// Line that join projected points on the baseline to the vanish points.
	const Line pc_vp2 = GeometryUtil::lineForTwoPoints(out_pc, vp2);
	const Line pr_vp1 = GeometryUtil::lineForTwoPoints(out_pr, vp1);

	// Definitive perspective point
	const Point pp = GeometryUtil::intersectionForTwoLines(pc_vp2, pr_vp1);

    // debug(1, "Calculating pp for",
    //     "\nmapPoint\t", mapPoint,
    //     "\ncameraPoint\t", cameraPoint,
    //     "\nbasePoint\t", basePoint,
    //     "\ncameraAngle\t", cameraAngle,
    //     "\nhorizonY\t", horizonY,
    //     "\nmapTileSize\t", mapTileSize,
    //     "\n--\t-- PP: ", pp);
    return pp;
}
#endif

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

    double perspectiveScale = distancePP_VP / distanceConstruction_VP;

    // debug(1, "Calculating height for",
    //     "\nperspectivePoint\t", perspectivePoint,
    //     "\nvanishPoint\t", vanishPoint,
    //     "\nconstructionPoint\t", constructionPoint,
    //     "\n--\t-- SCALE: ", perspectiveScale);
    return perspectiveScale;
}

double PerspectiveUtil::angleAdjustmentForPerspectivePoint(
    double xLeft, double xRight, double yTop, double yBottom, const Point &pp) {

    // Adjust angle diff
    const double HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT = 35 * M_PI / 180;
    const double BASELINE_ADJUSTMENT_FACTOR = 1;
    const double VERTICAL_CAMERA_ANGLE_ADJUSTMENT_FACTOR = 2;

    double horizontalAdjustmentRad = MathUtil::changeRange(
        Range {xLeft, xRight },
        Range {-HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT, HORIZONTAL_CAMERA_ANGLE_ADJUSTMENT },
        pp.x);

    double verticalAjustmentFactor = MathUtil::changeRange(
        Range {yTop, yBottom },
        Range {VERTICAL_CAMERA_ANGLE_ADJUSTMENT_FACTOR, BASELINE_ADJUSTMENT_FACTOR},
        pp.y
    );

    return verticalAjustmentFactor * horizontalAdjustmentRad;
}