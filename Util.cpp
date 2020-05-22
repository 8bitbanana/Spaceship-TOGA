#include "Util.h"

#include <random>
#include <chrono>

namespace Util {

    std::minstd_rand engine;

    void init_random() {
        auto time = std::chrono::high_resolution_clock().now();

        unsigned int seed = *(unsigned int*)(void*)&time;

        printf("Seed: %u", seed);

        engine = std::minstd_rand();
        engine.seed(seed);
    }

    unsigned int random() {
        return engine();
    }

    float random_float(float min, float max) {
        auto rand = (float)random();
        return min + (rand / (engine.max()/(max-min)));
    }
}