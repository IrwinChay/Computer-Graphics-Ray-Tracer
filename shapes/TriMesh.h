/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "Triangle.h"
#include "happly.h"
#include "BVH.h"
#include <vector>

namespace rt {

    class TriMesh : public Shape {
    public:
        TriMesh();

        TriMesh(BlinnPhong *material, std::string &id, std::string &filename, Vec3f position, float scale);

        ~TriMesh() {};

        Hit intersect(Ray ray);

        Vec3f getNormal(Vec3f point);

        Vec2f getUVMappings(Vec3f point);

        BBox getBBox();

    private:
        std::vector<Shape *> triangles;
        Vec3f position;
        float scale;
        BVH* tri_mesh_bvh;
        BBox bbox;
        void loadTriangles(const std::string &filename);
        void loadBBox();
    };


} //namespace rt




#endif /* TRIMESH_H_ */
