#include <gtest/gtest.h>
#include "math/Ray.h"

TEST(RayTest, DefaultConstructor) {
    Vec3 origin{0, 0, 0};
    Vec3 direction{0, 0, 0};
    Ray r;
    
    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

TEST(RayTest, Constructor) {
    Vec3 origin{1, 2, 3};
    Vec3 direction{4, 5, 6};
    Ray r{origin, direction};
    
    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

TEST(RayTest, AtTimeZeroIsOrigin) {
    Vec3 origin{1, 2, 3};
    Vec3 direction{4, 5, 6}; 
    Ray r{origin, direction};
    
    EXPECT_EQ(r.at(0), origin);
}

TEST(RayTest, AtOneEqualsOriginPlusDirection) {
    Vec3 origin{1, 2, 3};
    Vec3 direction{4, 5, 6};
    Ray r{origin, direction};
    
    Vec3 expected = origin + direction;
    
    EXPECT_EQ(r.at(1.0), expected);
}