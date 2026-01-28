#include <gtest/gtest.h>
#include "math/Vec3.h"

TEST(Vec3Test, DefaultConstructor) {
    Vec3 v;
    EXPECT_EQ(v.x, 0.0);
    EXPECT_EQ(v.y, 0.0);
    EXPECT_EQ(v.z, 0.0);
}

TEST(Vec3Test, NormalizedMagnitude) {
    Vec3 v{4, 7, 6};
    Vec3 n = v.normalized();
    EXPECT_EQ(n.length(), 1.0);
}

TEST(Vec3Test, OutputStreamOperator) {
    Vec3 v{1.5, 2.75, 3.125};
    
    std::ostringstream oss;
    oss << v;
    
    EXPECT_EQ(oss.str(), "[1.5, 2.75, 3.125]");
}

TEST(Vec3DotTest, DotProductIdentity) {
    // Test: A·B = |A||B|cos(angle)
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    double dot_product = dot(v1, v2);
    double magnitudes = v1.length() * v2.length();
    double cos_angle = dot_product / magnitudes;
    
    // cos(angle) should be between -1 and 1
    EXPECT_GE(cos_angle, -1.0);
    EXPECT_LE(cos_angle, 1.0);
    
    double expected = magnitudes * cos_angle;
    EXPECT_NEAR(dot_product, expected, 1e-10);
}

TEST(Vec3DotTest, DotProductWithParallelVectors) {
    Vec3 v1{3, 4, 0};  // length = 5
    Vec3 v2{6, 8, 0};  // length = 10, same direction
    
    double d = dot(v1, v2);
    double expected = v1.length() * v2.length();  // cos(0) = 1
    
    EXPECT_NEAR(d, expected, 1e-10);
}

TEST(Vec3DotTest, DotProductWithAntiparallelVectors) {
    Vec3 v1{3, 4, 0};   // length = 5
    Vec3 v2{-3, -4, 0}; // length = 5, opposite direction
    
    double d = dot(v1, v2);
    double expected = -v1.length() * v2.length();  // cos(180deg) = -1
    
    EXPECT_NEAR(d, expected, 1e-10);
}

TEST(Vec3DotTest, DotProductWithPerpendicularVectors) {
    Vec3 v1{1, 0, 0};  // X-axis
    Vec3 v2{0, 1, 0};  // Y-axis
    
    double d = dot(v1, v2);
    
    EXPECT_EQ(d, 0.0);  // cos(90deg) = 0
}

TEST(Vec3DotTest, DotProductWithUnitVectors) {
    Vec3 v1{3, 4, 0};
    Vec3 v2{1, 0, 0};
    
    Vec3 u1 = v1.normalized();
    Vec3 u2 = v2.normalized();
    
    double cos_angle = dot(u1, u2); // Dot product of unit vectors directly gives cos(angle)
    
    EXPECT_NEAR(cos_angle, 0.6, 1e-10);
}

TEST(Vec3CrossTest, CrossProductWithPerpendicularVectors) {
    // Test: A X B = |A||B|sin(angle)
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    
    Vec3 cross_product = cross(v1, v2);
    
    EXPECT_EQ(dot(cross_product, v1), 0);
    EXPECT_EQ(dot(cross_product, v2), 0);
}

TEST(Vec3CrossTest, CrossProductRightHandRule) {
    Vec3 x_axis{1, 0, 0};
    Vec3 y_axis{0, 1, 0};
    Vec3 z_axis{0, 0, 1};
    
    // X × Y = Z (right-hand rule)
    EXPECT_EQ(cross(x_axis, y_axis), z_axis);
    
    // Y × Z = X
    EXPECT_EQ(cross(y_axis, z_axis), x_axis);
    
    // Z × X = Y
    EXPECT_EQ(cross(z_axis, x_axis), y_axis);
}

TEST(Vec3CrossTest, CrossProductAnticommutative) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    Vec3 cross1 = cross(v1, v2);
    Vec3 cross2 = cross(v2, v1);
    
    // A × B = -(B × A)
    EXPECT_NEAR(cross1.x, -cross2.x, 1e-10);
    EXPECT_NEAR(cross1.y, -cross2.y, 1e-10);
    EXPECT_NEAR(cross1.z, -cross2.z, 1e-10);
}

TEST(Vec3CrossTest, CrossProductWithParallelVectors) {
    Vec3 v1{2, 4, 6};
    Vec3 v2{1, 2, 3};
    Vec3 zero{0, 0, 0};

    // Cross product of parallel vectors is zero vector
    EXPECT_EQ(cross(v1, v2), zero);
}

TEST(Vec3CrossTest, CrossProductWithSelf) {
    Vec3 v{3, 4, 5};
    Vec3 zero{0, 0, 0};

    // A × A = 0 (vector crossed with itself is zero)
    EXPECT_EQ(cross(v, v), zero);
}

TEST(Vec3CrossTest, CrossProductMagnitude) {
    Vec3 v1{3, 0, 0};  // length = 3
    Vec3 v2{0, 4, 0};  // length = 4, perpendicular to v1
    
    Vec3 result = cross(v1, v2);
    
    // |A × B| = |A||B|sin(angle)
    // For perpendicular vectors, sin(90deg) = 1
    double expected_magnitude = v1.length() * v2.length();
    
    EXPECT_NEAR(result.length(), expected_magnitude, 1e-10);
}

TEST(Vec3CrossTest, CrossProductCalculation) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    Vec3 result = cross(v1, v2);
    
    // Manual calculation with determinants:
    // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4)
    // = (12 - 15, 12 - 6, 5 - 8)
    // = (-3, 6, -3)
    
    EXPECT_NEAR(result.x, -3.0, 1e-10);
    EXPECT_NEAR(result.y, 6.0, 1e-10);
    EXPECT_NEAR(result.z, -3.0, 1e-10);
}

TEST(Vec3CrossTest, CrossProductTriangleNormal) {
    Vec3 v0{0, 0, 0};
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{0, 0, 1};

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    // Normal should point in +Z direction for CCW triangle
    EXPECT_EQ(cross(edge1, edge2), v3);
}
    
TEST(Vec3CrossTest, CrossProductScalarTripleProduct) {
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{0, 0, 1};
    
    // Scalar triple product: A · (B × C)
    // Represents volume of parallelepiped
    double volume = dot(v1, cross(v2, v3));
    
    EXPECT_NEAR(volume, 1.0, 1e-10);  // Unit cube
}

TEST(Vec3CrossTest, CrossProductDistributive) {
    Vec3 a{1, 2, 3};
    Vec3 b{4, 5, 6};
    Vec3 c{7, 8, 9};
    
    // A × (B + C) = (A × B) + (A × C)
    Vec3 left = cross(a, b + c);
    Vec3 right = cross(a, b) + cross(a, c);
    
    EXPECT_NEAR(left.x, right.x, 1e-10);
    EXPECT_NEAR(left.y, right.y, 1e-10);
    EXPECT_NEAR(left.z, right.z, 1e-10);
}