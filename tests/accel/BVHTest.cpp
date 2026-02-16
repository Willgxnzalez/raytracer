#include <gtest/gtest.h>
#include "accel/BVH.h"
#include "accel/AABB.h"
#include "geometry/Sphere.h"
#include "renderer/Scene.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include "core/Vec3.h"
#include <memory>
#include <vector>
#include <cmath>

// ============================================================================
// Test Fixtures
// ============================================================================

class BVHTest : public ::testing::Test {
protected:
    void SetUp() override {
        scene = std::make_unique<Scene>();
        defaultMat = scene->addDiffuse(Vec3(0.5f, 0.5f, 0.5f));
    }

    void TearDown() override {
        scene.reset();
    }

    int addSphere(const Vec3& center, float radius) {
        return scene->addSphere(center, radius, defaultMat);
    }

    std::unique_ptr<Scene> scene;
    int defaultMat;
};

// ============================================================================
// Construction Tests
// ============================================================================

TEST_F(BVHTest, BuildEmptyScene) {
    scene->build();
    SUCCEED();  // Doesn't crash
}

TEST_F(BVHTest, BuildSingleObjectCreatesLeaf) {
    addSphere(Vec3(0, 0, 0), 1.0f);
    scene->build();
    
    Ray ray(Vec3(-5, 0, 0), Vec3(1, 0, 0));
    HitRecord record;
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    EXPECT_TRUE(hit);
}

TEST_F(BVHTest, BuildTwoObjectsCreatesInternalNode) {
    addSphere(Vec3(-2, 0, 0), 1.0f);
    addSphere(Vec3(2, 0, 0), 1.0f);
    scene->build();
    
    Ray ray1(Vec3(-5, 0, 0), Vec3(1, 0, 0));
    HitRecord record;
    EXPECT_TRUE(scene->intersect(record, ray1, 0.001f, 100.0f));
    
    Ray ray2(Vec3(5, 0, 0), Vec3(-1, 0, 0));
    EXPECT_TRUE(scene->intersect(record, ray2, 0.001f, 100.0f));
}

// ============================================================================
// Bounding Box Tests
// ============================================================================

TEST_F(BVHTest, LeafNodeBoundingBoxMatchesObject) {
    int sphereIdx = addSphere(Vec3(5, 3, -2), 2.0f);
    scene->build();
    
    AABB treeBox = scene->getBVH().boundingBox();
    
    const Sphere& sphere = scene->getSpheres()[sphereIdx];
    AABB sphereBox = sphereBounds(sphere);
    
    EXPECT_EQ(treeBox.min, sphereBox.min);
    EXPECT_EQ(treeBox.max, sphereBox.max);
}

TEST_F(BVHTest, InternalNodeBoundingBoxIsUnion) {
    int idx1 = addSphere(Vec3(-10, 0, 0), 1.0f);
    int idx2 = addSphere(Vec3(10, 0, 0), 1.0f);
    scene->build();
    
    AABB box = scene->getBVH().boundingBox();
    
    const Sphere& s1 = scene->getSpheres()[idx1];
    const Sphere& s2 = scene->getSpheres()[idx2];
    AABB unionBox = surroundingBox(sphereBounds(s1), sphereBounds(s2));

    EXPECT_EQ(box.min, unionBox.min);
    EXPECT_EQ(box.max, unionBox.max);
}

TEST_F(BVHTest, BoundingBoxContainsAllObjects) {
    // Create a 5x5 grid of spheres
    for (int x = 0; x < 5; ++x) {
        for (int z = 0; z < 5; ++z) {
            addSphere(Vec3(x * 3.0f, 0, z * 3.0f), 1.0f);
        }
    }
    scene->build();
    
    AABB box = scene->getBVH().boundingBox();
    EXPECT_EQ(box.min, Vec3(-1, -1, -1));
    EXPECT_EQ(box.max, Vec3(13, 1, 13));
}

// ============================================================================
// Ray Intersection Tests
// ============================================================================

TEST_F(BVHTest, HitSingleSphere) {
    addSphere(Vec3(0, 0, 0), 1.0f);
    scene->build();
    
    Ray ray(Vec3(-5, 0, 0), Vec3(1, 0, 0));
    HitRecord record;
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    
    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 4.0f, 0.01f);
    EXPECT_NEAR(record.position.x, -1.0f, 0.01f);
    EXPECT_NEAR(record.position.y, 0.0f, 0.01f);
    EXPECT_NEAR(record.position.z, 0.0f, 0.01f);
}

TEST_F(BVHTest, HitClosestSphere) {
    addSphere(Vec3(0, 0, -5), 1.0f);   // Far
    addSphere(Vec3(0, 0, -2), 0.5f);   // Close
    scene->build();
    
    Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
    HitRecord record;
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    
    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 1.5f, 0.01f);  // Should hit closer sphere
}

TEST_F(BVHTest, MissAllSpheres) {
    addSphere(Vec3(0, 0, 0), 1.0f);
    scene->build();
    
    Ray ray(Vec3(0, 10, 0), Vec3(1, 0, 0));  // Ray passes above sphere
    HitRecord record;
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    
    EXPECT_FALSE(hit);
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST_F(BVHTest, IdenticalSpherePositions) {
    Vec3 pos(0, 0, -5);
    
    // Add multiple spheres at the same position
    addSphere(pos, 1.0f);
    addSphere(pos, 0.8f);
    addSphere(pos, 0.6f);
    
    scene->build();
    
    Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
    HitRecord record;
    
    // Should hit the closest (biggest radius = 1.0)
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    EXPECT_TRUE(hit);
    EXPECT_EQ(record.t, 4.0f);  // 5 - 0.6
}

TEST_F(BVHTest, RayParallelToSplitAxis) {
    // Create spheres along x-axis
    addSphere(Vec3(-5, 0, 0), 1.0f);
    addSphere(Vec3(5, 0, 0), 1.0f);
    
    scene->build();
    
    // Ray along x-axis
    Ray ray(Vec3(-10, 0, 0), Vec3(1, 0, 0));
    HitRecord record;
    
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    EXPECT_TRUE(hit);
}

TEST_F(BVHTest, VerifyBVHHierarchy) {
    // Create 8 spheres (will create a tree of depth 3)
    for (int i = 0; i < 8; ++i) {
        addSphere(Vec3(i * 2.0f, 0, 0), 1.0f);
    }
    
    scene->build();
    
    // Verify the tree was built (bounding box encompasses all spheres)
    AABB box = scene->getBVH().boundingBox();
    EXPECT_EQ(box.min.x, -1);
    EXPECT_EQ(box.max.x, 15);
}

TEST_F(BVHTest, LargeScenePerformance) {
    // Create a larger scene to test BVH efficiency
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            addSphere(Vec3(x * 3.0f, y * 3.0f, 0), 0.5f);
        }
    }
    
    scene->build();
    
    // Ray that only hits a few spheres
    Ray ray(Vec3(0, 0, 5), Vec3(0, 0, -1));
    HitRecord record;
    bool hit = scene->intersect(record, ray, 0.001f, 100.0f);
    
    EXPECT_TRUE(hit);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}