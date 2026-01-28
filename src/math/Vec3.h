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
    double x, y, z;

    Vec3(): x(0.0), y(0.0), z(0.0) {}
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    double lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vec3 normalized() const {
        double mag = length();
        return Vec3(x / mag, y / mag, z / mag);
    }

    double operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    double& operator[](int i) {
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

    Vec3& operator*=(double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    
    Vec3& operator/=(double s) {
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

inline Vec3 operator*(const Vec3& v, double s) {
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(double s, const Vec3& v) {
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline bool operator==(const Vec3& a, const Vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline double dot(const Vec3& a, const Vec3& b) { // How much 2 vectors align
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