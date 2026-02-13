#include <gtest/gtest.h>
#include "accel/AABB.h"
#include "core/Vec3.h"
#include "core/Ray.h"

TEST(AABBTest, ConstructorAndMinMax) {
    Vec3 min{0, 0, 0};
    Vec3 max{1, 1, 1};
    AABB box(min, max);

    EXPECT_EQ(box.min, min);
    EXPECT_EQ(box.max, max);
}

TEST(AABBTest, HitRayInsideBox) {
    AABB box(Vec3{0,0,0}, Vec3{1,1,1});
    Ray ray(Vec3{0.5, 0.5, -1}, Vec3{0, 0, 1}); // Along +Z, should hit

    EXPECT_TRUE(box.hit(ray, 0.0, 10.0));
}

TEST(AABBTest, MissRay) {
    AABB box(Vec3{0,0,0}, Vec3{1,1,1});
    Ray ray(Vec3{2, 2, 2}, Vec3{1, 0, 0}); // Completely outside

    EXPECT_FALSE(box.hit(ray, 0.0, 10.0));
}

TEST(AABBTest, ParallelRayInsideSlab) {
    AABB box(Vec3{0,0,0}, Vec3{1,1,1});
    Ray ray(Vec3{0.5, 0.5, 0.5}, Vec3{0, 1e-5f, 0}); // Almost zero direction along Y

    EXPECT_TRUE(box.hit(ray, 0.0, 10.0));
}

TEST(AABBTest, ParallelRayOutsideSlab) {
    AABB box(Vec3{0,0,0}, Vec3{1,1,1});
    Ray ray(Vec3{2, 0.5, 0.5}, Vec3{0, 1e-5f, 0}); // Parallel, outside X slab

    EXPECT_FALSE(box.hit(ray, 0.0, 10.0));
}

TEST(AABBTest, SurroundingBox) {
    AABB box1(Vec3{0,0,0}, Vec3{1,1,1});
    AABB box2(Vec3{0.5,0.5,0.5}, Vec3{2,2,2});

    AABB surrounding = surroundingBox(box1, box2);

    EXPECT_EQ(surrounding.min, Vec3(0,0,0));
    EXPECT_EQ(surrounding.max, Vec3(2,2,2));
}

TEST(AABBTest, OperatorStream) {
    AABB box(Vec3{1,2,3}, Vec3{4,5,6});
    std::stringstream ss;
    ss << box;
    EXPECT_EQ(ss.str(), "[[1, 2, 3], [4, 5, 6]]");
}
