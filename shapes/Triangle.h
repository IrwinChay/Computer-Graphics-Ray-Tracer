/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"

namespace rt {

    class Triangle : public Shape {
    public:

        //
        // Constructors
        //
        Triangle();

        Triangle(Vec3f v0, Vec3f v1, Vec3f v2, BlinnPhong *material, std::string id) : v0(v0), v1(v1), v2(v2),
                                                                                     Shape(material, id) {};

        Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f n0, Vec3f n1, Vec3f n2, BlinnPhong *material, std::string id) : v0(
                v0), v1(v1), v2(v2), hasNormals(true), n0(n0.normalize()), n1(n1.normalize()), n2(n2.normalize()), Shape(material, id) {};

        Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f n0, Vec3f n1, Vec3f n2, Vec2f t0, Vec2f t1, Vec2f t2,BlinnPhong *material, std::string id) : v0(
                v0), v1(v1), v2(v2), hasNormals(true), n0(n0.normalize()), n1(n1.normalize()), n2(n2.normalize()), hasUV(
                true),t0(t0),t1(t1),t2(t2),Shape(material, id) {};

        ~Triangle() {};

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
        bool hasNormals = false;
        Vec3f n0;
        Vec3f n1;
        Vec3f n2;
        bool hasUV = false;
        Vec2f t0;
        Vec2f t1;
        Vec2f t2;
    };


} //namespace rt




#endif /* TRIANGLE_H_ */
