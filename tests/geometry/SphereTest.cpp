#include <gtest/gtest.h>
#include "geometry/Sphere.h"
#include "core/HitRecord.h"
#include "core/Ray.h"

/* 
Ray hitting Sphere (radius 1, center at 0)
                        ___
-5   -4   -3   -2  -1 /  0  \ 1    2
●----●----●----●---->|   ●   |●----●
^                   ^ \     / ^
|                   |   ‾‾‾   |
Ray Origin        t=4 (hit)  t=6 (exit)
                  ENTRY      EXIT
                  point      point
*/
TEST(SphereTest, RayHitsSphereAtCenter) {
    Sphere sphere{Vec3{0, 0, 0}, 1.0, nullptr};
    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};

    HitRecord record;
    bool hit = sphere.hit(record, ray, 0.0, 100.0);

    EXPECT_TRUE(hit);

    // Ray should hit at x = -1 (left side of sphere)
    EXPECT_EQ(record.position, Vec3(-1, 0, 0));
    
    // Normal should point outward (to the left)
    EXPECT_EQ(record.surfaceNormal, Vec3(-1, 0, 0));
    EXPECT_TRUE(record.frontFace);
}

TEST(SphereTest, RayMissesSphere) {
    Sphere sphere{Vec3{0, 0, 0}, 1.0, nullptr};
    
    // Ray passes above sphere
    Ray ray{Vec3{-5, 5, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    bool hit = sphere.hit(record, ray, 0.0, 100.0);
    
    EXPECT_FALSE(hit);
}

TEST(SphereTest, RayOriginatesInsideSphere) {
    Sphere sphere{Vec3{0, 0, 0}, 2.0, nullptr};
    
    // Ray starts inside sphere, pointing out
    Ray ray{Vec3{0, 0, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    bool hit = sphere.hit(record, ray, 0.0, 100.0);
    
    EXPECT_TRUE(hit);
    
    // Should hit at x = 2 (exit point)
    EXPECT_EQ(record.position, Vec3(2.0, 0.0, 0.0));
}

TEST(SphereTest, RayHitsWithinTRange) {
    Sphere sphere{Vec3{0, 0, 0}, 1.0, nullptr};
    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    
    // Hit should be at t = 4 (distance from -5 to -1)
    bool hit = sphere.hit(record, ray, 3.0, 5.0);
    EXPECT_TRUE(hit);
    
    // Hit is outside range
    hit = sphere.hit(record, ray, 0.0, 3.0);
    EXPECT_FALSE(hit);
}

TEST(SphereTest, RayIsTangentToSphere) {
    Sphere sphere{Vec3{0, 0, 0}, 1.0, nullptr};
    
    // Ray just barely touches sphere at top (tangent)
    Ray ray{Vec3{-5, 1, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    bool hit = sphere.hit(record, ray, 0.0, 100.0);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(record.position, Vec3(0.0, 1.0, 0.0));
}

TEST(SphereTest, RejectsSelfIntersection) {
    Sphere sphere(Vec3(0,0,0), 1.0, nullptr);

    Ray ray(Vec3(1,0,0), Vec3(1,0,0)); // starts exactly on surface
    HitRecord record;

    EXPECT_FALSE(sphere.hit(record, ray, 0.001, 100.0));
}
