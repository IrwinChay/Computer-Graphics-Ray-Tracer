/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"

namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere();
	Sphere(Vec3f center, float radius, BlinnPhong* material, std::string id):center(center), radius(radius), Shape(material, id){};

    ~Sphere(){};


	//
	// Functions that need to be implemented, since Sphere is a subclass of Shape
	//
	Hit intersect(Ray ray);
    Vec3f getNormal(Vec3f point);
    Vec2f getUVMappings(Vec3f point);
    BBox getBBox();

private:

	Vec3f center;
	float radius;

};



} //namespace rt




#endif /* SPHERE_H_ */
