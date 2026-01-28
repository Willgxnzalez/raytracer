#include <gtest/gtest.h>
#include <iostream>
#include <type_traits>
#include "math/Vec3.h"
#include "core/Ray.h"

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
// }

TEST(MemoryAnalysis, Vec3Size) {
    EXPECT_LE(sizeof(Vec3), 64);
    EXPECT_TRUE(std::is_trivially_copyable<Vec3>::value);
}

TEST(MemoryAnalysis, RaySize) {
    EXPECT_LE(sizeof(Ray), 64);
    EXPECT_TRUE(std::is_trivially_copyable<Ray>::value);
}