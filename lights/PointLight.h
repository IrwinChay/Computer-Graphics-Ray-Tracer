/*
 * PointLight.h
 *
 *
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "core/LightSource.h"


namespace rt{

class PointLight : public LightSource {
public:
    PointLight(Vec3f position, Vec3f intensity_specular, Vec3f diffuse_intensity) : position(position),
    LightSource(intensity_specular,diffuse_intensity) {};
    Vec3f getRandomPointOnLight() override;
    Vec3f getJitterSampling(int total_number_of_samples, int j) override;
private:
    Vec3f position;
};


} //namespace rt




#endif /* POINTLIGHT_H_ */
