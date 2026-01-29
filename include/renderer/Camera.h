#pragma once
#include "../math/Vec3.h"
#include "../math/Ray.h"

/**
 * Camera - Perspective camera with configurable field of view.
 * 
 * Creates a pinhole camera positioned at origin, oriented to face the -Z direction with the up-axis set to +Y.
 * The viewport is positioned at focal length distance from the camera.
 */
class Camera {
    Vec3 origin; // LookFrom point, camera position
    Vec3 lowerLeftCorner; // world-space corner of viewport
    Vec3 horizontal; // viewport width vector
    Vec3 vertical; // viewport height vector
    double focalLength; // Determines angular spread of rays

    int imageWidth;
    int imageHeight;

    // Orthonormal basis vectors for camera coordinate system
    Vec3 u; // camera right
    Vec3 v; // camera up
    Vec3 w; // camera backward(points from scene toward camera)

public:
    /**
     * Construct a new Camera object
     * 
     * @param lookFrom Camera position in world space
     * @param lookAt Point in world space the camera is looking at
     * @param vUp Up direction vector for the camera (usually (0,1,0))
     * @param _imageWidth Width of image in pixels
     * @param _imageHeight Height of image in pixels
     * @param vFovDegrees Vertical field of view in degrees
     */
    Camera(
        const Vec3& lookFrom, 
        const Vec3& lookAt, 
        const Vec3& vUp, 
        int _imageWidth, 
        int _imageHeight, 
        double vFovDegrees
    );

    Ray shootRay(int i, int j) const;
};