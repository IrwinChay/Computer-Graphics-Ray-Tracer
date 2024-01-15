/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"



namespace rt{


    Vec3f PointLight::getRandomPointOnLight(){
        return position;
    }

    Vec3f PointLight::getJitterSampling(int total_number_of_samples, int j) {
        return position;
    }
} //namespace rt
