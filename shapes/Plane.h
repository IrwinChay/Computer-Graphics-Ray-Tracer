//
// Created by moon9 on 04/03/2023.
//

#ifndef RAY_TRACER_PLANE_H
#define RAY_TRACER_PLANE_H

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"

namespace rt{

class Plane:public Shape{

public:
    Plane();
    Plane(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, BlinnPhong* material1, std::string id):v0(v0), v1(v1), v2(v2), v3(v3), Shape(material1, id){};

    ~Plane(){};


    //
    // Functions that need to be implemented, since Sphere is a subclass of Shape
    //
    Hit intersect(Ray ray);
    Vec3f getNormal(Vec3f point);
    Vec2f getUVMappings(Vec3f point);
    BBox getBBox();

private:

    Vec3f v0;
    Vec3f v1;
    Vec3f v2;
    Vec3f v3;

};

} //namespace rt


#endif //RAY_TRACER_PLANE_H
