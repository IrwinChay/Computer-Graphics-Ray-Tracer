/*
 * Pinhole.h
 *
 *
 */

#ifndef PINHOLE_H_
#define PINHOLE_H_

#include "core/Camera.h"
#include "geometry.h"
#include "core/RayHitStructs.h"

namespace rt{

class Pinhole:public Camera{
public:

	//
	// Constructors
	//
	Pinhole();
	Pinhole(int width, int height, int fov, Vec3f position, Vec3f direction, Vec3f up);

	//
	//Destructor
	//
	~Pinhole(){};

	//
	// print function (implementing abstract function of base class)
	//
	void printCamera();

    Ray generateRay(float pixel_i_shift, float pixel_j_shift);
};



} //namespace rt




#endif /* PINHOLE_H_ */
