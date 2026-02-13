#include <gtest/gtest.h>
#include <iostream>
#include <type_traits>
#include "core/Vec3.h"
#include "core/Ray.h"
#include "accel/AABB.h"

template<typename T>
void analyze_type(const char* name) {
    std::cout << "=== " << name << " ===\n";
    std::cout << "Size: " << sizeof(T) << " bytes\n";
    std::cout << "Trivially copyable: " 
              << (std::is_trivially_copyable<T>::value ? "YES" : "NO") << "\n";
    std::cout << "Trivially copy constructible: "
              << (std::is_trivially_copy_constructible<T>::value ? "YES" : "NO") << "\n";
    std::cout << "Standard layout: "
              << (std::is_standard_layout<T>::value ? "YES" : "NO") << "\n";
    
    if (sizeof(T) <= 64 && std::is_trivially_copyable<T>::value) {
        std::cout << "CHEAP TO COPY - Pass by value\n";
    } else {
        std::cout << "EXPENSIVE TO COPY - Pass by const reference\n";
    }
    std::cout << "\n";
}

// TEST(MemoryAnalysis, PrintTypeInfo) {
//     analyze_type<Vec3>("Vec3");
//     analyze_type<Ray>("Ray");
//     analyze_type<AABB>("AABB");
// }

TEST(MemoryAnalysis, Vec3Size) {
    EXPECT_EQ(sizeof(Vec3), 12);
    EXPECT_TRUE(std::is_trivially_copyable<Vec3>::value);
}

TEST(MemoryAnalysis, RaySize) {
    EXPECT_EQ(sizeof(Ray), 24);
    EXPECT_TRUE(std::is_trivially_copyable<Ray>::value);
}

TEST(MemoryAnalysis, AABBSize) {
    EXPECT_EQ(sizeof(Ray), 24);
    EXPECT_TRUE(std::is_trivially_copyable<Ray>::value);
}