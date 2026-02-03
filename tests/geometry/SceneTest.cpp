#include <gtest/gtest.h>
#include "geometry/Scene.h"
#include "geometry/Sphere.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include <memory>

// ============================================================================
// Scene Integration Tests
// ============================================================================

TEST(SceneTest, HitsSingleObject) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 1.0, nullptr));
    scene.build();

    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};
    HitRecord record;

    bool hit = scene.hit(record, ray, 0.001, 100.0);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 4.0, 1e-6);
    EXPECT_EQ(record.position, Vec3(-1, 0, 0));
    EXPECT_EQ(record.surfaceNormal, Vec3(-1, 0, 0));
}

TEST(SceneTest, ReturnsNearestHit) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{5, 0, 0}, 1.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{10, 0, 0}, 1.0, nullptr));
    scene.build();

    Ray ray{Vec3{0, 0, 0}, Vec3{1, 0, 0}};
    HitRecord record;

    bool hit = scene.hit(record, ray, 0.001, 100.0);

    EXPECT_TRUE(hit);
    EXPECT_EQ(record.position, Vec3(4, 0, 0));   // nearest sphere
    EXPECT_LT(record.t, 6.0);                    // sanity check
}

TEST(SceneTest, RayMissesAllObjects) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{5, 0, 0}, 1.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{10, 0, 0}, 1.0, nullptr));
    scene.build();

    Ray ray{Vec3{0, 5, 0}, Vec3{1, 0, 0}};
    HitRecord record;

    EXPECT_FALSE(scene.hit(record, ray, 0.001, 100.0));
}

TEST(SceneTest, OverlappingObjectsHitClosestSurface) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 2.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 1.0, nullptr));
    scene.build();

    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};
    HitRecord record;

    bool hit = scene.hit(record, ray, 0.001, 100.0);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 3.0, 1e-6);             // -5 → -2
    EXPECT_EQ(record.position, Vec3(-2, 0, 0));  // larger sphere surface
}

TEST(SceneTest, RespectsTMinAndTMax) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, -5}, 1.0, nullptr));
    scene.build();

    Ray ray{Vec3{0, 0, 0}, Vec3{0, 0, -1}};
    HitRecord record;

    EXPECT_FALSE(scene.hit(record, ray, 6.0, 100.0)); // tMin past hit
    EXPECT_FALSE(scene.hit(record, ray, 0.001, 3.0)); // tMax before hit
    EXPECT_TRUE(scene.hit(record, ray, 0.001, 100.0));
}
