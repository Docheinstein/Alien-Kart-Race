#include "kartparamsfactory.h"

Kart::KartParams KartParamsFactory::params(KartType type) {
    Kart::KartParams params;
    switch (type) {
    case Venusian:
        params.maxSpeed = 0.15;
        params.minSpeed = -0.05;
        params.maxWheelTurning = 0.45;
        params.wheelTurningSkidPoint = 0.30;
        params.accelerationFactor = (double) 1 / (double) (1 << 10);
        params.backwardAccelerationFactor = (double) 1 / (double) (1 << 9);
        params.decelerationFactor = params.accelerationFactor * 3;
        params.brakeFactor = params.accelerationFactor * 5;

        params.steeringWheelTurningFactor = 0.018;
        params.steeringWheelTurningFactorSkidding = 0.036;
    	params.steeringWheelReturnFactor = 0.016;
        params.bounceSpeedInitialSpeed = params.maxSpeed * 0.9;
        params.bounceDecellerationFactor = (double) 1 / 160;
    }
    return params;
}