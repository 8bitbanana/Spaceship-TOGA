#include "Util.h"

#include <random>

namespace Util {

    std::minstd_rand engine;

    void init_random() {
        engine = std::minstd_rand();
        engine.seed(50);
    }

    unsigned int random() {
        return engine();
    }

    float random_float(float min, float max) {
        auto rand = (float)random();
        return min + (rand / (engine.max()/(max-min)));
    }
}