#include <gtest/gtest.h>
#include "geometry/BVHTree.h"
#include "geometry/BVHNode.h"
#include "geometry/AABB.h"
#include "geometry/Sphere.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include "core/Vec3.h"
#include "materials/Lambertian.h"
#include <memory>
#include <vector>
#include <cmath>

// ============================================================================
// Test Fixtures
// ============================================================================

class BVHTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        material = std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
        tree = std::make_unique<BVHTree>();
    }

    void TearDown() override {
        tree.reset();
    }

    std::shared_ptr<Hittable> makeSphere(const Vec3& center, double radius) {
        return std::make_shared<Sphere>(center, radius, material.get());
    }

    std::unique_ptr<BVHTree> tree;
    std::shared_ptr<Lambertian> material;
};

// ============================================================================
// Construction Tests
// ============================================================================

TEST_F(BVHTreeTest, BuildEmptyTree) {
    HittableList objects;
    tree->build(objects);
    SUCCEED();  // Doesn't crash
}

TEST_F(BVHTreeTest, BuildSingleObjectCreatesLeaf) {
    HittableList objects;
    objects.push_back(makeSphere(Vec3(0,0,0), 1.0));
    tree->build(objects);
    EXPECT_TRUE(tree->root()->isLeaf());
}

TEST_F(BVHTreeTest, BuildTwoObjectsCreatesInternalNode) {
    HittableList objects;
    objects.push_back(makeSphere(Vec3(-2,0,0), 1.0));
    objects.push_back(makeSphere(Vec3(2,0,0), 1.0));
    tree->build(objects);
    EXPECT_FALSE(tree->root()->isLeaf());
}

// ============================================================================
// Bounding Box Tests
// ============================================================================

TEST_F(BVHTreeTest, LeafNodeBoundingBoxMatchesObject) {
    HittableList objects;
    auto sphere = makeSphere(Vec3(5, 3, -2), 2.0);
    objects.push_back(sphere);
    tree->build(objects);
    
    AABB treeBox = tree->boundingBox();
    AABB sphereBox = sphere->boundingBox();
    
    EXPECT_EQ(treeBox.min, sphereBox.min);
    EXPECT_EQ(treeBox.max, sphereBox.max);
}

TEST_F(BVHTreeTest, InternalNodeBoundingBoxIsUnion) {
    HittableList objects;
    auto sphere1 = makeSphere(Vec3(-10, 0, 0), 1.0);
    auto sphere2 = makeSphere(Vec3(10, 0, 0), 1.0);
    objects.push_back(sphere1);
    objects.push_back(sphere2);
    tree->build(objects);
    
    AABB box = tree->boundingBox();
    AABB unionBox = surroundingBox(sphere1->boundingBox(), sphere2->boundingBox());

    EXPECT_EQ(box.min, unionBox.min);
    EXPECT_EQ(box.max, unionBox.max);
}

TEST_F(BVHTreeTest, BoundingBoxContainsAllObjects) {
    HittableList objects;

    // Create a 5x5 grid of spheres
    for (int x = 0; x < 5; ++x) {
        for (int z = 0; z < 5; ++z) {
            objects.push_back(makeSphere(
                Vec3(x * 3.0, 0, z * 3.0),
                1
            ));
        }
    }
    tree->build(objects);
    
    AABB box = tree->boundingBox();
    EXPECT_EQ(box.min, Vec3(-1, -1, -1));
    EXPECT_EQ(box.max, Vec3(13, 1, 13));
}

// ============================================================================
// Ray Intersection Tests
// ============================================================================

TEST_F(BVHTreeTest, HitSingleSphere) {
    HittableList objects;
    objects.push_back(makeSphere(Vec3(0, 0, 0), 1.0));
    tree->build(objects);
    
    Ray ray(Vec3(-5, 0, 0), Vec3(1, 0, 0));

    HitRecord record;
    bool hit = tree->hit(record, ray, 0.001, 100.0);
    
    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 4.0, 0.01);  // Ray hits at distance ~4
    EXPECT_EQ(record.position, Vec3(-1, 0, 0)); // Ray should hit at x = -1 (left side of sphere)
    EXPECT_EQ(record.surfaceNormal, Vec3(-1, 0, 0)); // Normal should point outward (to the left)
}

// ============================================================================
// BVH Acceleration Tests
// ============================================================================

TEST_F(BVHTreeTest, BVHReducesIntersectionTests) {
    // Create a large grid of spheres
    HittableList objects;
    const int gridSize = 10;
    
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            for (int z = 0; z < gridSize; ++z) {
                objects.push_back(makeSphere(
                    Vec3(x * 3.0, y * 3.0, z * 3.0 - 50),
                    0.5
                ));
            }
        }
    }
    
    tree->build(objects);
    
    // Reset AABB hit counter
    gAABBHits = 0;
    
    // Ray that only intersects a small portion of the scene
    Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
    HitRecord record;
    tree->hit(record, ray, 0.001, 100.0);
    
    // BVH should cull most of the spheres
    // Without BVH -> test all 1000 spheres
    // With BVH -> far fewer AABBs
    std::cout << "AABB hits: " << gAABBHits << " < 1000 spheres / 2 " << std::endl;
    EXPECT_LT(gAABBHits, objects.size()/2);
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST_F(BVHTreeTest, IdenticalSpherePositions) {
    HittableList objects;
    Vec3 pos(0, 0, -5);
    
    // Add multiple spheres at the same position
    objects.push_back(makeSphere(pos, 1.0));
    objects.push_back(makeSphere(pos, 0.8));
    objects.push_back(makeSphere(pos, 0.6));
    
    tree->build(objects);
    
    Ray ray(Vec3(0, 0, 0), Vec3(0, 0, -1));
    HitRecord record;
    
    // Should still hit successfully
    bool hit = tree->hit(record, ray, 0.001, 100.0);
    EXPECT_TRUE(hit);
}

TEST_F(BVHTreeTest, RayParallelToSplitAxis) {
    HittableList objects;
    
    // Create spheres along x-axis
    objects.push_back(makeSphere(Vec3(-5, 0, 0), 1.0));
    objects.push_back(makeSphere(Vec3(5, 0, 0), 1.0));
    
    tree->build(objects);
    
    // Ray along x-axis
    Ray ray(Vec3(-10, 0, 0), Vec3(1, 0, 0));
    HitRecord record;
    
    bool hit = tree->hit(record, ray, 0.001, 100.0);
    EXPECT_TRUE(hit);
}

TEST_F(BVHTreeTest, VerifyBVHHierarchy) {
    HittableList objects;
    
    // Create 8 spheres (will create a tree of depth 3)
    for (int i = 0; i < 8; ++i) {
        objects.push_back(makeSphere(Vec3(i * 2.0, 0, 0), 1));
    }
    
    tree->build(objects);
    
    // Verify the tree was built (bounding box encompasses all spheres)
    AABB box = tree->boundingBox();
    EXPECT_EQ(box.min.x, -1);
    EXPECT_EQ(box.max.x, 15);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}