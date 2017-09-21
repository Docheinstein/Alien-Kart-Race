#ifndef KARTPARAMFACTORY_H
#define KARTPARAMFACTORY_H

#include "kart.h"

class KartParamsFactory {
public:

    enum KartType {
        Venusian
    };

    static Kart::KartParams params(KartType type);
};

#endif // KARTPARAMFACTORY_H