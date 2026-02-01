#include <gtest/gtest.h>
#include "geometry/Scene.h"
#include "geometry/Sphere.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include <memory>

TEST(SceneTest, SingleSphere) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 1.0, nullptr));
    scene.build();
    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    bool hit = scene.hit(record, ray, 0.0, 100.0);
    
    EXPECT_TRUE(hit);
    
    // Ray should hit at x = -1 (left side of sphere)
    EXPECT_EQ(record.position, Vec3(-1, 0, 0));
    
    // Normal should point outward (to the left)
    EXPECT_EQ(record.surfaceNormal, Vec3(-1, 0, 0));
}

TEST(SceneTest, HitsNearestOfTwoSpheres) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{5, 0, 0}, 1.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{10, 0, 0}, 1.0, nullptr));
    scene.build();
    Ray ray{Vec3{0, 0, 0}, Vec3{1, 0, 0}};
    
    HitRecord record;
    bool hit = scene.hit(record, ray, 0.0, 100.0);
    
    EXPECT_TRUE(hit);
    
    // Ray should hit left side of first sphere at x = 4
    EXPECT_EQ(record.position, Vec3(4, 0, 0));
    
    // Normal should point outward (to the left)
    EXPECT_EQ(record.surfaceNormal, Vec3(-1, 0, 0));
}

TEST(SceneTest, RayMissesSpheres) {
    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3{5, 0, 0}, 1.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{10, 0, 0}, 1.0, nullptr));
    scene.build();
    Ray ray{Vec3{0, 5, 0}, Vec3{1, 0, 0}};

    HitRecord record;
    bool hit = scene.hit(record, ray, 0.0, 100.0);
    
    EXPECT_FALSE(hit);
}

TEST(SceneTest, OverlappingSpheres) {
    Scene scene;
    // Two spheres overlapping at origin
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 2.0, nullptr));
    scene.add(std::make_shared<Sphere>(Vec3{0, 0, 0}, 1.0, nullptr));
    scene.build();

    
    Ray ray{Vec3{-5, 0, 0}, Vec3{1, 0, 0}};
    HitRecord record;
    
    bool hit = scene.hit(record, ray, 0.0, 100.0);
    
    EXPECT_TRUE(hit);
    
    // Should hit the larger sphere first (at x=-2)
    EXPECT_EQ(record.position, Vec3(-2, 0, 0));
    EXPECT_NEAR(record.t, 3.0, 1e-10);  // Distance from -5 to -2
}
