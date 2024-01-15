/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include <utility>

#include "core/Shape.h"
#include "BBoxNode.h"

namespace rt{

class BVH: public Shape{
public:
    BVH();
    BVH(std::vector<Shape*> objects):Shape(){
        buildBVH(std::move(objects));
    }
    ~BVH(){};

    //
    // Functions that need to be implemented, since Sphere is a subclass of Shape
    //
    Hit intersect(Ray ray);
    Vec3f getNormal(Vec3f point);
    Vec2f getUVMappings(Vec3f point);
    BBox getBBox();
private:
    BBoxNode* root;
    void buildBVH(std::vector<Shape*> objects);
    void buildBVHNode(std::vector<BBoxNode*> bbox_nodes, size_t start, size_t end, BBoxNode& parent);
    bool hitBBox(BBox bbox, Ray ray, float t_min, float t_max);
    bool intersectNode(BBoxNode *node, Ray ray, Shape*& shape, float& hit_distance_t);
};



} //namespace rt



#endif /* BVH_H_ */
