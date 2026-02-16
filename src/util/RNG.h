#pragma once
#include <cstdint>

class RNG {
public:
    RNG(uint64_t seed, uint64_t sequence = 1) {
        state = 0;
        inc = (sequence << 1u) | 1u; // ensure odd increment
        nextUInt(); // scramble initial state
        state += seed;
        nextUInt(); // scramble state with seed
    }

    uint32_t nextUInt() {
        // PCG by Melissa O'Neill
        uint64_t oldState = state;
        state = oldState * 6364136223846793005ULL + inc; // Linear Congruential Generator

        // permutate 64-bit state into 32-bit output
        uint32_t xorshifted = ((oldState >> 18u) ^ oldState) >> 27u; // truncate to 32-bits implicitly
        uint32_t rot = oldState >> 59u; // Keep 5 most significant bits

        return ((xorshifted >> rot) | 
                (xorshifted << ((-rot) & 31))); // Mask to 32 bit integer
    }

    float uniform01() {
        return (nextUInt() >> 8) * 5.9604644775390625e-8f; // (random in [0, 2^24-1]) * (1.0 / 2^24)
    }

    float uniform(float min, float maxExclusive) {
        return min + (maxExclusive - min) * uniform01();
    }

    int uniformInt(int min, int maxExclusive) {
        return min + (nextUInt() % (maxExclusive - min));
    }

private:
    uint64_t state;
    uint64_t inc;
};