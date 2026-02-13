#include <gtest/gtest.h>
#include "util/RNG.h"

constexpr uint64_t TESTSEED = 1212;  

TEST(RNGTest, Uniform01HalfOpen) {
    RNG rng{TESTSEED, 1};
    
    int i = 0;
    while (i++ < 100000) {
        float f = rng.uniform01();
        EXPECT_GE(f, 0.0f);
        EXPECT_LT(f, 1.0f);
    }
}

TEST(RNGTest, UniformRangeHalfOpen) {
    RNG rng{TESTSEED, 1};

    int i = 0;
    while (i++ < 100000) {
        float f = rng.uniform(1.0f, 10.0f);
        EXPECT_GE(f, 1.0f);
        EXPECT_LT(f, 10.0f);
    }
}

TEST(RNGTest, DifferentRandomValuesGenerated) {
    RNG rng{TESTSEED, 1};
    
    float f = rng.uniform01();
    int i = 0;
    while (i++ < 100000) {
        float newF = rng.uniform01();
        EXPECT_NE(f, newF);
        f = newF;
    }
}

TEST(RNGTest, RNGIsDeterministic) {
    RNG rng1{TESTSEED, 1};
    RNG rng2{TESTSEED, 1};

    int i = 0;
    while (i++ < 100000) {
        EXPECT_EQ(rng1.nextUInt(), rng2.nextUInt());
    }
}

TEST(RNGTest, DifferentStreamsProduceDifferentSequences) {
    RNG rng1{TESTSEED, 1};
    RNG rng2{TESTSEED, 2};

    int i = 0;
    while (i++ < 100000) {
        EXPECT_NE(rng1.nextUInt(), rng2.nextUInt());
    }
}

