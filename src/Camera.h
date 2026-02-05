#pragma once
#include "core/Vec3.h"
#include "core/Ray.h"

/**
 * Camera - Perspective camera with configurable field of view.
 */
class Camera {
    Vec3 origin_;     // Camera position (ray origin)
    Vec3 lowerLeft_;  // world-space corner of viewport
    Vec3 horizontal_; // viewport width vector
    Vec3 vertical_;   // viewport height vector
    Vec3 u_, v_, w_;  // Orthonomal basis: Right, Up, Back

    int imageWidth_, imageHeight_;
    
    double aperture_, focusDistance_;

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
        const Vec3& lookFrom, 
        const Vec3& lookAt, 
        const Vec3& vUp, 
        int imageWidth, 
        int imageHeight, 
        double vFovDegrees,
        double aperture,
        double focusDistance
    );

    Ray shootRay(int i, int j) const;
};