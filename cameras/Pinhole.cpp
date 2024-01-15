/*
 * Pinhole.cpp
 *
 *
 */
#include <iostream>

#include "Pinhole.h"
#include "core/RayHitStructs.h"



namespace rt{

	//
	// Pinhole constructor (example)
	//
	Pinhole::Pinhole(int width, int height, int fov, Vec3f position, Vec3f direction, Vec3f up):Camera(width, height, fov){
        this->position = position;
        this->direction = direction;
        this->up = up;
        this->right = direction.crossProduct(up);
        // build transformation matrix from camera coordinate system to world coordinate system
        auto right_norm = right.normalize();
        auto up_norm = up.normalize();
        auto direction_norm = direction.normalize();
        this->cameraToWorld = Matrix44f(
                right_norm.x, right_norm.y, right_norm.z, 0,
                up_norm.x, up_norm.y, up_norm.z, 0,
                direction_norm.x, direction_norm.y, direction_norm.z, 0,
                position.x, position.y, position.z, 1);

	}


	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void Pinhole::printCamera(){
		printf("I am a pinhole camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
	}

    Ray Pinhole::generateRay(float pixel_i_shift, float pixel_j_shift){
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
        // Transform direction vector from camera coordinate system to world coordinate system
        auto direction_vector_in_world = Vec3f();
        cameraToWorld.multVecMatrix(direction_vector, direction_vector_in_world);
        // Subtract position to get direction vector
        direction_vector_in_world = direction_vector_in_world - position;
        direction_vector_in_world.normalize();
        return Ray(PRIMARY, position, direction_vector_in_world);
    }


} //namespace rt

