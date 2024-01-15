/*
 * ThinLens.cpp
 *
 *
 */
#include <iostream>


#include "ThinLens.h"


namespace rt {

    //
    // ThinLens constructor (example)
    //

    ThinLens::ThinLens(int width, int height, int fov, float focus_distance, float aperture_size, Vec3f position,
                       Vec3f direction, Vec3f up) : Camera(width, height, fov) {
        this->position = position;
        this->direction = direction;
        this->up = up;
        this->right = direction.crossProduct(up);

        this->focus_distance = focus_distance;
        this->aperture_size = aperture_size;

        // build transformation matrix from camera coordinate system to world coordinate system
        auto right_norm = right.normalize();
        auto up_norm = up.normalize();
        auto direction_norm = direction.normalize();
        this->cameraToWorld = Matrix44f(
                right_norm.x, right_norm.y, right_norm.z, 0,
                up_norm.x, up_norm.y, up_norm.z, 0,
                direction_norm.x, direction_norm.y, direction_norm.z, 0,
                position.x, position.y, position.z, 1);
    };

    /**
     * Prints camera data
     * this function implements the abstract function of the base Camera class
     */
    void ThinLens::printCamera() {
        printf("I am a thin lens camera! \n");
        printf("width: %dpx, height: %dpx, fov:%d, focus_distance:%f, aperture_size:%f\n", width, height, fov,
               focus_distance, aperture_size);
    }

    Vec2f ThinLens::sampleDiskUniform() {
        // Generate random starting point coordinte from the lens using uniform distribution on polar coordinates
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(0.0, 1.0);
        //todo check if jitter should be added here
        auto r = sqrt(dist(gen)) * aperture_size;
        auto theta = dist(gen) * 2 * M_PI;

        return {static_cast<float>(r * cos(theta)), static_cast<float>(r * sin(theta))};
    }

    Ray ThinLens::generateRay(float pixel_i_shift, float pixel_j_shift) {
        // Simple ThinLens Camera Model where Ray shooting from lens
        //         (Start here)     | u -> i (y)
        //                   --------------> v -> j (x)
        //                          |           (End here)
        // pixel_i_shift: 0 -> width changes on x-axis from left to right
        // pixel_j_shift: 0 -> height changes on y-axis from top to bottom
        // FOV calculation
        auto vertical_fov = ((height * fov) / width) * (M_PI / 180.0);
        auto horizontal_fov = fov * (M_PI / 180.0);
        auto v = (2 * tan(horizontal_fov / 2)) / width;
        auto u = (2 * tan(vertical_fov / 2)) / height;
        // Changes on x and y
        auto direction_vector_x = (pixel_j_shift - (width / 2)) * u;
        auto direction_vector_y = ((height / 2) - pixel_i_shift) * v;
        auto direction_vector_z = 1; // Screen at front direction
        // Assemble direction vector
        auto direction_vector = Vec3f(direction_vector_x, direction_vector_y, direction_vector_z).normalize();
        auto direction_vector_in_world = Vec3f();
        cameraToWorld.multVecMatrix(direction_vector, direction_vector_in_world);
        direction_vector_in_world = direction_vector_in_world - position;

        // Warning This is a Thin lens approximation model
        // Generate random starting point coordinate from the lens (disk)
        // Adjust lens size by aperture_size
        auto lens_sample_point = sampleDiskUniform();
        auto lens_position_offset_in_camera = right * lens_sample_point.x + up * lens_sample_point.y;
        auto lens_position_offset_in_world = Vec3f();
        cameraToWorld.multVecMatrix(lens_position_offset_in_camera, lens_position_offset_in_world);

        // Calculate focus point in world coordinate
        Vec3f focus_point_in_world = position + direction_vector_in_world* focus_distance;

        // Create a ray from the lens point to the focal point
        auto final_direction_vector_in_world = (focus_point_in_world - lens_position_offset_in_world).normalize();

        return Ray(PRIMARY, lens_position_offset_in_world, final_direction_vector_in_world);
    }

} //namespace rt
