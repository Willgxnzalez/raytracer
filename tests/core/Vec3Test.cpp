#include <gtest/gtest.h>
#include "core/Vec3.h"

constexpr float TEST_EPS = 1e-5f;

TEST(Vec3Test, DefaultConstructor) {
    Vec3 v{};
    EXPECT_EQ(v.x, 0.0);
    EXPECT_EQ(v.y, 0.0);
    EXPECT_EQ(v.z, 0.0);
}

TEST(Vec3Test, SplatConstructor) {
    Vec3 v0{1.0f};
    Vec3 v1{1.0f, 1.0f, 1.0f};
    EXPECT_EQ(v0, v1);
}

// ============================================================================
// Operations
// ============================================================================

TEST(Vec3Test, OutputStreamOperator) {
    Vec3 v{1.5, 2.75, 3.125};
    
    std::ostringstream oss;
    oss << v;
    
    EXPECT_EQ(oss.str(), "[1.5, 2.75, 3.125]");
}

TEST(Vec3Test, AdditionOperator) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    Vec3 result = v1 + v2;
    
    EXPECT_EQ(result, Vec3(5.0, 7.0, 9.0));
}

TEST(Vec3Test, SubtractionOperator) {
    Vec3 v1{5, 7, 9};
    Vec3 v2{1, 2, 3};
    Vec3 result = v1 - v2;
    
    EXPECT_EQ(result, Vec3(4.0, 5.0, 6.0));
}

TEST(Vec3Test, MultiplicationByScalar) {
    Vec3 v{2, 3, 4};
    Vec3 result = v * 2.5;
    
    EXPECT_EQ(result, Vec3(5.0, 7.5, 10.0));
}

TEST(Vec3Test, ScalarMultiplicationCommutative) {
    Vec3 v{2, 3, 4};
    Vec3 result1 = v * 3.0;
    Vec3 result2 = 3.0 * v;
    
    EXPECT_EQ(result1, result2);
}

TEST(Vec3Test, DivisionByScalar) {
    Vec3 v{10, 20, 30};
    Vec3 result = v / 5.0;
    
    EXPECT_EQ(result, Vec3(2.0, 4.0, 6.0));
}

TEST(Vec3Test, AdditionAssignment) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    v1 += v2;
    
    EXPECT_EQ(v1, Vec3(5.0, 7.0, 9.0));
}

TEST(Vec3Test, SubtractionAssignment) {
    Vec3 v1{10, 15, 20};
    Vec3 v2{2, 3, 4};
    v1 -= v2;
    
    EXPECT_EQ(v1, Vec3(8.0, 12.0, 16.0));
}

TEST(Vec3Test, MultiplicationAssignment) {
    Vec3 v{2, 3, 4};
    v *= 3.0;
    
    EXPECT_EQ(v, Vec3(6.0, 9.0, 12.0));
}

TEST(Vec3Test, DivisionAssignment) {
    Vec3 v{20, 30, 40};
    v /= 2.0;
    
    EXPECT_EQ(v, Vec3(10.0, 15.0, 20.0));
}

TEST(Vec3Test, NegationOperator) {
    Vec3 v{1, -2, 3};
    Vec3 result = -v;
    
    EXPECT_EQ(result, Vec3(-1.0, 2.0, -3.0));
}

TEST(Vec3Test, MultiplicationByZero) {
    Vec3 v{5, 10, 15};
    Vec3 result = v * 0.0;
    
    EXPECT_EQ(result, Vec3(0.0, 0.0, 0.0));
}

TEST(Vec3Test, AdditionIdentity) {
    Vec3 v{3, 4, 5};
    Vec3 zero{0, 0, 0};
    Vec3 result = v + zero;
    
    EXPECT_EQ(result, v);
}

TEST(Vec3Test, MultiplicationByOne) {
    Vec3 v{3, 4, 5};
    Vec3 result = v * 1.0;
    
    EXPECT_EQ(result, v);
}

TEST(Vec3Test, DivisionByOne) {
    Vec3 v{3, 4, 5};
    Vec3 result = v / 1.0;
    
    EXPECT_EQ(result, v);
}

TEST(Vec3Test, ChainedOperations) {
    Vec3 v{2, 3, 4};
    Vec3 result = (v * 2.0 + Vec3{1, 1, 1}) / 3.0;
    
    EXPECT_NEAR(result.x, (2.0 * 2.0 + 1.0) / 3.0, TEST_EPS);
    EXPECT_NEAR(result.y, (3.0 * 2.0 + 1.0) / 3.0, TEST_EPS);
    EXPECT_NEAR(result.z, (4.0 * 2.0 + 1.0) / 3.0, TEST_EPS);
}

TEST(Vec3Test, AssociativeMultiplication) {
    Vec3 v{2, 3, 4};
    Vec3 result1 = (v * 2.0) * 3.0;
    Vec3 result2 = v * (2.0 * 3.0);
    
    EXPECT_EQ(result1, result2);
}

// ============================================================================
// Dot Product
// ============================================================================

TEST(Vec3Test, DotProductIdentity) {
    // Test: A·B = |A||B|cos(angle)
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    float dot_product = dot(v1, v2);
    float magnitudes = v1.length() * v2.length();
    float cos_angle = dot_product / magnitudes;
    
    // cos(angle) should be between -1 and 1
    EXPECT_GE(cos_angle, -1.0);
    EXPECT_LE(cos_angle, 1.0);
    
    float expected = magnitudes * cos_angle;
    EXPECT_NEAR(dot_product, expected, TEST_EPS);
}

TEST(Vec3Test, DotProductWithParallelVectors) {
    Vec3 v1{3, 4, 0};  // length = 5
    Vec3 v2{6, 8, 0};  // length = 10, same direction
    
    float d = dot(v1, v2);
    float expected = v1.length() * v2.length();  // cos(0) = 1
    
    EXPECT_NEAR(d, expected, TEST_EPS);
}

TEST(Vec3Test, DotProductWithAntiparallelVectors) {
    Vec3 v1{3, 4, 0};   // length = 5
    Vec3 v2{-3, -4, 0}; // length = 5, opposite direction
    
    float d = dot(v1, v2);
    float expected = -v1.length() * v2.length();  // cos(180deg) = -1
    
    EXPECT_NEAR(d, expected, TEST_EPS);
}

TEST(Vec3Test, DotProductWithPerpendicularVectors) {
    Vec3 v1{1, 0, 0};  // X-axis
    Vec3 v2{0, 1, 0};  // Y-axis
    
    float d = dot(v1, v2);
    
    EXPECT_EQ(d, 0.0);  // cos(90deg) = 0
}

TEST(Vec3Test, DotProductWithUnitVectors) {
    Vec3 v1{3, 4, 0};
    Vec3 v2{1, 0, 0};
    
    Vec3 u1 = v1.normalized();
    Vec3 u2 = v2.normalized();
    
    float cos_angle = dot(u1, u2); // Dot product of unit vectors directly gives cos(angle)
    
    EXPECT_NEAR(cos_angle, 0.6, TEST_EPS);
}

// ============================================================================
// Cross Product
// ============================================================================

TEST(Vec3Test, CrossProductWithPerpendicularVectors) {
    // Test: A X B = |A||B|sin(angle)
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    
    Vec3 cross_product = cross(v1, v2);
    
    EXPECT_EQ(dot(cross_product, v1), 0);
    EXPECT_EQ(dot(cross_product, v2), 0);
}

TEST(Vec3Test, CrossProductRightHandRule) {
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

TEST(Vec3Test, CrossProductAnticommutative) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    Vec3 cross1 = cross(v1, v2);
    Vec3 cross2 = cross(v2, v1);
    
    // A × B = -(B × A)
    EXPECT_NEAR(cross1.x, -cross2.x, TEST_EPS);
    EXPECT_NEAR(cross1.y, -cross2.y, TEST_EPS);
    EXPECT_NEAR(cross1.z, -cross2.z, TEST_EPS);
}

TEST(Vec3Test, CrossProductWithParallelVectors) {
    Vec3 v1{2, 4, 6};
    Vec3 v2{1, 2, 3};
    Vec3 zero{0, 0, 0};

    // Cross product of parallel vectors is zero vector
    EXPECT_EQ(cross(v1, v2), zero);
}

TEST(Vec3Test, CrossProductWithSelf) {
    Vec3 v{3, 4, 5};
    Vec3 zero{0, 0, 0};

    // A × A = 0 (vector crossed with itself is zero)
    EXPECT_EQ(cross(v, v), zero);
}

TEST(Vec3Test, CrossProductMagnitude) {
    Vec3 v1{3, 0, 0};  // length = 3
    Vec3 v2{0, 4, 0};  // length = 4, perpendicular to v1
    
    Vec3 result = cross(v1, v2);
    
    // |A × B| = |A||B|sin(angle)
    // For perpendicular vectors, sin(90deg) = 1
    float expected_magnitude = v1.length() * v2.length();
    
    EXPECT_NEAR(result.length(), expected_magnitude, TEST_EPS);
}

TEST(Vec3Test, CrossProductCalculation) {
    Vec3 v1{1, 2, 3};
    Vec3 v2{4, 5, 6};
    
    Vec3 result = cross(v1, v2);
    
    // Manual calculation with determinants:
    // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4)
    // = (12 - 15, 12 - 6, 5 - 8)
    // = (-3, 6, -3)
    
    EXPECT_NEAR(result.x, -3.0, TEST_EPS);
    EXPECT_NEAR(result.y, 6.0, TEST_EPS);
    EXPECT_NEAR(result.z, -3.0, TEST_EPS);
}

TEST(Vec3Test, CrossProductTriangleNormal) {
    Vec3 v0{0, 0, 0};
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{0, 0, 1};

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    // Normal should point in +Z direction for CCW triangle
    EXPECT_EQ(cross(edge1, edge2), v3);
}
    
TEST(Vec3Test, CrossProductScalarTripleProduct) {
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{0, 0, 1};
    
    // Scalar triple product: A · (B × C)
    // Represents volume of parallelepiped
    float volume = dot(v1, cross(v2, v3));
    
    EXPECT_NEAR(volume, 1.0, TEST_EPS);  // Unit cube
}

TEST(Vec3Test, CrossProductDistributive) {
    Vec3 a{1, 2, 3};
    Vec3 b{4, 5, 6};
    Vec3 c{7, 8, 9};
    
    // A × (B + C) = (A × B) + (A × C)
    Vec3 left = cross(a, b + c);
    Vec3 right = cross(a, b) + cross(a, c);
    
    EXPECT_NEAR(left.x, right.x, TEST_EPS);
    EXPECT_NEAR(left.y, right.y, TEST_EPS);
    EXPECT_NEAR(left.z, right.z, TEST_EPS);
}