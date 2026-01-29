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
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal; // displacement vector for left->right edge of viewport 
    Vec3 vertical; // displacement vector for bottom->top edge of viewport
    double focalLength; // Determines angular spread of rays

    int imageWidth;
    int imageHeight;

public:
    /**
     * @param imageWidth Width of image in pixels
     * @param imageHeight Height of image in pixels
     * @param vFov Vertical field of view in degrees
     */
    Camera(int imageWidth, int imageHeight, double vFov);

    Ray shootRay(int i, int j) const;
};