/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "rapidjson/document.h"
#include "core/RayHitStructs.h"


using namespace rapidjson;

namespace rt {

    class Camera {
    public:

        //
        // Constructors
        //
        Camera() {};

        Camera(int height, int width, int fov) : height(height), width(width), fov(fov) {};

        //
        // Destructor
        //
        virtual ~Camera();


        //
        // factory function : returns camera instance dynamically based on camera specifications
        //
        static Camera *createCamera(Value &cameraSpecs);


        //
        // print function (to be implemented by the subclasses )
        //
        virtual void printCamera() = 0;


        //
        // other camera functions (to complete)
        //
        virtual Ray generateRay(float pixel_i_shift, float pixel_j_shift) = 0;


        //
        // Getters and Setters
        //
        int getHeight() const {
            return height;
        }

        void setHeight(int height) {
            this->height = height;
        }

        int getWidth() const {
            return width;
        }

        void setWidth(int width) {
            this->width = width;
        }


    protected:
        //
        //camera members
        //
        int height;
        int width;
        int fov; //field of view
        Vec3f position;
        Vec3f direction;
        Vec3f up;
        Vec3f right;
        Matrix44f cameraToWorld;
    };

} //namespace rt



#endif /* CAMERA_H_ */
