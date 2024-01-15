/*
 * AreaLight.cpp
 *
 *
 */
#include "AreaLight.h"
#include "core/RayHitStructs.h"

namespace rt {

    AreaLight::AreaLight(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Vec3f intensity_specular, Vec3f diffuse_intensity) :
            LightSource(intensity_specular, diffuse_intensity) {
        // ------upper_right
        // ^               |
        // |(v)            |
        // |               |
        // lower_left-(u)--->

        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;

        this->u_vector = v1 - v0;
        this->v_vector = v2 - v0;
    }

    Vec3f AreaLight::getRandomPointOnLight() {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(0.0, 1.0);
        float u = dist(gen);
        float v = dist(gen);
        return v0 + u * u_vector + v * v_vector;
    }

    Vec3f AreaLight::getJitterSampling(int total_number_of_samples, int j) {
        int root_samples = sqrt(total_number_of_samples);
        int grid_x = j % root_samples;
        int grid_y = j / root_samples;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.0, 1.0);

        float u = (grid_x + dist(gen)) / root_samples;
        float v = (grid_y + dist(gen)) / root_samples;

        return v0 + u * u_vector + v * v_vector;
    }

} //namespace rt
