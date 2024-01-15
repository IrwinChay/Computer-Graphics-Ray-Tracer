/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include <vector>
#include "geometry.h"

namespace rt {

    class LightSource {
    public:
        LightSource(Vec3f intensity_specular, Vec3f diffuse_intensity) {
            this->intensity_specular = intensity_specular;
            this->diffuse_intensity = diffuse_intensity;
        };

        Vec3f getIntensitySpecular() {
            return intensity_specular;
        }

        Vec3f getDiffuseIntensity() {
            return diffuse_intensity;
        }

        virtual Vec3f getRandomPointOnLight() = 0;

        virtual Vec3f getJitterSampling(int total_number_of_samples, int j) = 0;

    private:
        Vec3f intensity_specular; // shine ness of the light
        Vec3f diffuse_intensity;
    };

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
