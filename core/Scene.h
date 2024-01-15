/*
 * Scene.h
 *
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "rapidjson/document.h"

#include "core/LightSource.h"
#include "core/Shape.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"
#include "shapes/Sphere.h"
#include "shapes/Plane.h"
#include "shapes/Triangle.h"
#include "shapes/BVH.h"
#include "shapes/TriMesh.h"
#include "materials/BlinnPhong.h"

using namespace rapidjson;

namespace rt {

    class Scene {
    public:

        Scene() {};

        void createScene(Value &scenespecs);

        std::vector<LightSource *> getLightSources() { return lightSources; }

        std::vector<Shape *> getShapes() { return shapes; }

        Vec3f getBackgroundColor() { return backgroundColor; }

        BVH *getBVH() { return bvh; }

        void set_n_bound(int n) { n_bound = n; }

        int get_n_bound() { return n_bound; }

        void set_sample(int s) { sample = s; }

        int get_sample() { return sample; }

        void set_use_jitter_sample(bool b) { use_jitter_sample = b; }

        bool get_use_jitter_sample() { return use_jitter_sample; }

        void set_use_bvh(bool b) { use_bvh = b; }

        bool get_use_bvh() { return use_bvh; }

    private:
        int n_bound;
        int sample;
        bool use_jitter_sample;
        bool use_bvh;
        std::vector<LightSource *> lightSources;
        std::vector<Shape *> shapes;
        Vec3f backgroundColor;
        BVH *bvh;
    };

} //namespace rt



#endif /* SCENE_H_ */
