/*
 * ThinLens.h
 *
 */

#ifndef THINLENS_H_
#define THINLENS_H_

#include "core/Camera.h"
#include <random>
#include <iostream>

namespace rt {

    class ThinLens : public Camera {
    public:

        //
        // Constructors
        //
        ThinLens() : Camera() {};

        ThinLens(int width, int height, int fov, float focus_distance, float aperture_size, Vec3f position, Vec3f direction, Vec3f up);

        //
        //Destructor
        //
        ~ThinLens() {};

        //
        // print function (implementing abstract function of base class)
        //
        void printCamera();

        Ray generateRay(float pixel_i_shift, float pixel_j_shift);

    private:
        float focus_distance;
        float aperture_size;

        Vec2f sampleDiskUniform();
    };


} //namespace rt




#endif /* THINLENS_H_ */
