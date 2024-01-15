/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <utility>

#include "core/RayHitStructs.h"
#include "core/Material.h"
#include "shapes/BBox.h"
namespace rt{

class Shape{
public:

	//
	// Constructors
	//
	Shape():material(nullptr){};
    Shape(BlinnPhong* material, std::string id):material(material), id(std::move(id)){};

	//
	// Destructor (must be overriden in subclass)
	//
	virtual ~Shape();

	//
	// Shape abstract methods (to be implemented by subclasses)
	//
	virtual Hit intersect(Ray)=0;

    virtual Vec3f getNormal(Vec3f point)=0;

    virtual Vec2f getUVMappings(Vec3f point)=0;

    virtual BBox getBBox()=0;

    BlinnPhong* getMaterial(){
        return material;
    }

	std::string getId(){
		return id;
	}


protected:
    BlinnPhong * material;
	std::string id;

};


} //namespace rt


#endif /* SHAPE_H_ */
