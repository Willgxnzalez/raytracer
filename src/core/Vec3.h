#pragma once
#include <cmath>
#include <ostream>

/**
 * Vec3 - 3D vector class for representing points, directions, and colors.
 * Type aliases:
 *   - point3: Represents a position in 3D space
 *   - color: Represents RGB color values
 */
struct Vec3 {
    float x, y, z;

    Vec3(): x(0.0), y(0.0), z(0.0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vec3 normalized() const {
        float mag = length();
        return Vec3(x / mag, y / mag, z / mag);
    }

    bool nearZero() const {
        constexpr float s = 1e-5f;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }

    float operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    float& operator[](int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vec3& operator*=(const Vec3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    
    Vec3& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b) {
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator*(const Vec3& v, float s) {
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(float s, const Vec3& v) {
    return v * s;
}

inline Vec3 operator/(const Vec3& v, float s) {
    return v * (1/s);
}

inline bool operator==(const Vec3& a, const Vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline float dot(const Vec3& a, const Vec3& b) { // How much 2 vectors align
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b) { // Gives Vector perpendicular to input vectors
    return Vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

inline std::ostream & operator << ( std::ostream & out, Vec3 v) {
    out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return out;
}