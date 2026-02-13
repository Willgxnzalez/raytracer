#pragma once
#include "core/Vec3.h"
#include "core/Ray.h"
#include "util/RNG.h"

/**
 * Camera - Perspective camera with configurable field of view.
 */
class Camera {
    Point3 origin_;     // Camera position (ray origin)
    Point3 lowerLeft_;  // world-space corner of viewport
    Vec3 horizontal_; // viewport width vector
    Vec3 vertical_;   // viewport height vector
    Vec3 u_, v_, w_;  // Orthonomal basis: Right, Up, Back

    int imageWidth_, imageHeight_;
    
    float aperture_, focusDistance_;

public:
    /**
     * Construct a perspective camera.
     * 
     * @param lookFrom Camera position in world space
     * @param lookAt Point in world space the camera is looking at
     * @param vUp Up direction vector (usually (0,1,0))
     * @param imageWidth Image width in pixels
     * @param imageHeight Image height in pixels
     * @param vFovDegrees Vertical field of view in degrees
     * @param aperture Lens aperture diameter for depth of field configuration.
     *                 Larger values (e.g., 2.0) = shallow focus, blurred background.
     *                 Smaller values (e.g., 0.1) = deep focus, everything sharp.
     *                 Use 0.0 for pinhole (infinite depth of field).
     * @param focusDistance Distance to the plane in perfect focus
     */
    Camera(
        const Point3& lookFrom, 
        const Point3& lookAt, 
        const Vec3& vUp, 
        int imageWidth, 
        int imageHeight, 
        float vFovDegrees,
        float aperture,
        float focusDistance
    );

    Ray shootRay(int x, int y, RNG& rng) const;
};