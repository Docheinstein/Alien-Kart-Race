#include "game.h"
#include "perspectiveutil.h"
#include "geometryutil.h"

int main() {
	Game::instance().start();

    Point pcForKart, prForKart, vp1ForKart, vp2ForKart;
	Point camera { 7.2, 2.5 };
	Point dummypp = PerspectiveUtil::perspectivePoint(
        Point {camera.x - 2.4, camera.y - 0.8},
        camera,
        Point {7.2, 4.5},
        -0.453786,
        2.5,
        1,
        pcForKart,
        prForKart,
        vp1ForKart,
        vp2ForKart
		,0.523599 //Rotation
    );

    return 0;
}
