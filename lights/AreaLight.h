/*
 * AreaLight.h
 *
 *
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include <random>
#include "core/LightSource.h"
#include "core/RayHitStructs.h"

namespace rt {

    class AreaLight : public LightSource {
    public:
        AreaLight(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Vec3f intensity_specular, Vec3f diffuse_intensity);

        Vec3f getRandomPointOnLight() override;

        Vec3f getJitterSampling(int total_number_of_samples, int j) override;

    private:
        // v2------v3
        // |        |
        // |        |
        // |        |
        // v0------v1
        Vec3f v0;
        Vec3f v1;
        Vec3f v2;
        Vec3f v3;

        Vec3f u_vector;
        Vec3f v_vector;
    };


} //namespace rt




#endif /* AREALIGHT_H_ */
