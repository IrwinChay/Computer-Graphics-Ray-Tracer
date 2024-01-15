/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"
#include "Material.h"
#include "materials/BlinnPhong.h"

namespace rt {

/*
 * Ray structure definition
 */
    enum RayType {
        PRIMARY, REFLECTION, REFRACTION, SHADOW
    };

    struct Ray {

        RayType raytype;
        Vec3f origin;
        Vec3f direction;
        bool inside;

        Ray() {};

        Ray(RayType type, Vec3f origin, Vec3f direction, bool inside = false)
                : raytype(type), origin(origin), direction(direction), inside(inside) {}
    };


    struct Hit {
        //----------Hit variables to be filled------
        bool hit;
        float t; //distance from ray origin to hit point
        Vec3f point; //point where ray hits a shape
        Vec3f normal; //normal of the hit point
        Vec2f uv; //uv coordinates of the hit point
        BlinnPhong *material; //material of the hit point


        Hit() : hit(false), point(Vec3f(0, 0, 0)), material(), t(INFINITY) {};

        Hit(bool hit, Vec3f point, Vec3f normal, Vec2f uv, BlinnPhong *material, float t) : hit(hit), point(point),
                                                                                            normal(normal), uv(uv),
                                                                                            material(material), t(t) {};
    };

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
