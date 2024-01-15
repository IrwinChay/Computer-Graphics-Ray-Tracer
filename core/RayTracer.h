/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "math/geometry.h"
#include "core/Camera.h"
#include "core/Scene.h"

namespace rt {


/*
 * Raytracer class declaration
 */
    class RayTracer {
    public:

        RayTracer();

        static Vec3f *render(Camera *camera, Scene *scene);

        static Vec3f *tonemap(Vec3f *pixelbuffer, int totalPixels);

    private:
        static Vec3f trace(Ray ray, Scene *scene, int nbounces);

        static Vec3f getLocalIllumination(Ray ray, Scene *scene, Hit hit);

    }; //namespace rt


}
#endif /* RAYTRACER_H_ */