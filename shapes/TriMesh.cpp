/*
 * TriMesh.cpp
 *
 *
 */

#include "TriMesh.h"
#include "happly.h"

namespace rt {

    TriMesh::TriMesh() : Shape() {}

    TriMesh::TriMesh(BlinnPhong *material, std::string &id, std::string &filename, Vec3f position, float scale) : Shape(
            material, id) {
        this->position = position;
        this->scale = scale;
        loadTriangles(filename);
        loadBBox();
    }

    void TriMesh::loadTriangles(const std::string &filename) {
        try {
            happly::PLYData plyIn(filename);

            std::vector<std::array<double, 3>> vertices = plyIn.getVertexPositions();
            std::vector<std::vector<size_t>> faces = plyIn.getFaceIndices<size_t>();

            bool hasNormals =
                    plyIn.getElement("vertex").hasProperty("nx") && plyIn.getElement("vertex").hasProperty("ny") &&
                    plyIn.getElement("vertex").hasProperty("nz");
            std::vector<double> nxs;
            std::vector<double> nys;
            std::vector<double> nzs;
            if (hasNormals) {
                nxs = plyIn.getElement("vertex").getProperty<double>("nx");
                nys = plyIn.getElement("vertex").getProperty<double>("ny");
                nzs = plyIn.getElement("vertex").getProperty<double>("nz");
            }

            bool hasTexture =
                    plyIn.getElement("vertex").hasProperty("s") && plyIn.getElement("vertex").hasProperty("t");
            std::vector<double> s;
            std::vector<double> t;
            if (hasTexture) {
                s = plyIn.getElement("vertex").getProperty<double>("s");
                t = plyIn.getElement("vertex").getProperty<double>("t");
            }

            for (const auto &face: faces) {
                Vec3f v0((float) (vertices[face[0]][0] * scale), (float) (vertices[face[0]][1] * scale),
                         (float) (vertices[face[0]][2] * scale));
                Vec3f v1((float) (vertices[face[1]][0] * scale), (float) (vertices[face[1]][1] * scale),
                         (float) (vertices[face[1]][2] * scale));
                Vec3f v2((float) (vertices[face[2]][0] * scale), (float) (vertices[face[2]][1] * scale),
                         (float) (vertices[face[2]][2] * scale));

                Vec3f n0, n1, n2;
                if (hasNormals) {
                    n0 = Vec3f((float) nxs[face[0]], (float) nys[face[0]], (float) nzs[face[0]]);
                    n1 = Vec3f((float) nxs[face[1]], (float) nys[face[1]], (float) nzs[face[1]]);
                    n2 = Vec3f((float) nxs[face[2]], (float) nys[face[2]], (float) nzs[face[2]]);
                }

                Vec2f uv0, uv1, uv2;
                if (hasTexture) {
                    uv0 = Vec2f((float) s[face[0]], (float) t[face[0]]);
                    uv1 = Vec2f((float) s[face[1]], (float) t[face[1]]);
                    uv2 = Vec2f((float) s[face[2]], (float) t[face[2]]);
                }

                if (face.size() == 4) {
                    Vec3f v3, n3;
                    Vec2f uv3;
                    v3 = Vec3f((float) (vertices[face[3]][0] * scale), (float) (vertices[face[3]][1] * scale),
                               (float) (vertices[face[3]][2] * scale));
                    if (hasNormals) {
                        n3 = Vec3f((float) nxs[face[3]], (float) nys[face[3]], (float) nzs[face[3]]);
                    }
                    if (hasTexture) {
                        uv3 = Vec2f((float) s[face[3]], (float) t[face[3]]);
                    }

                    if (hasNormals && hasTexture) {
                        triangles.push_back(new Triangle(v0, v2, v3, n0, n2, n3, uv0, uv2, uv3, material, id));
                    } else if (hasNormals) {
                        triangles.push_back(new Triangle(v0, v2, v3, n0, n2, n3, material, id));
                    } else {
                        triangles.push_back(new Triangle(v0, v2, v3, material, id));
                    }
                }

                if (hasNormals && hasTexture) {
                    triangles.push_back(new Triangle(v0, v1, v2, n0, n1, n2, uv0, uv1, uv2, material, id));
                } else if (hasNormals) {
                    triangles.push_back(new Triangle(v0, v1, v2, n0, n1, n2, material, id));
                } else {
                    triangles.push_back(new Triangle(v0, v1, v2, material, id));
                }

            }
        } catch (const std::runtime_error &e) {
            std::cerr << "Error loading ply file: " << e.what() << std::endl;
        }
        std::cout << "Loaded " << triangles.size() << " triangles in mesh" << std::endl;

        // Build BVH
        tri_mesh_bvh = new BVH(triangles);
    }

    void TriMesh::loadBBox() {
        BBox bbox_local;
        for (auto triangle: triangles) {
            BBox triangle_bbox = triangle->getBBox();
            bbox_local.min.x = std::min(bbox_local.min.x, triangle_bbox.min.x);
            bbox_local.min.y = std::min(bbox_local.min.y, triangle_bbox.min.y);
            bbox_local.min.z = std::min(bbox_local.min.z, triangle_bbox.min.z);

            bbox_local.max.x = std::max(bbox_local.max.x, triangle_bbox.max.x);
            bbox_local.max.y = std::max(bbox_local.max.y, triangle_bbox.max.y);
            bbox_local.max.z = std::max(bbox_local.max.z, triangle_bbox.max.z);

            // Update center
            bbox_local.center = (bbox_local.min + bbox_local.max) /= 2;
        }

        // Transform bbox_local to world space
        bbox_local.min = bbox_local.min + position;
        bbox_local.max = bbox_local.max + position;
        bbox_local.center = bbox_local.center + position;

        bbox = bbox_local;
    }

    Hit TriMesh::intersect(Ray ray) {
        Hit closest_hit;
        float closest_distance = std::numeric_limits<float>::max();

        Ray ray_in_mesh_space = Ray(PRIMARY, ray.origin - position, ray.direction);

        bool enable_bvh = true;
        if (enable_bvh) {
            Hit hit = tri_mesh_bvh->intersect(ray_in_mesh_space);
            if (hit.hit && hit.t < closest_distance) {
                closest_distance = hit.t;
                Hit hit_in_world_space = Hit(hit.hit, hit.point + position, hit.normal, hit.uv, hit.material, hit.t);
                closest_hit = hit_in_world_space;
            }
        } else {
            for (auto triangle: triangles) {
                Hit hit = triangle->intersect(ray_in_mesh_space);
                if (hit.hit && hit.t < closest_distance) {
                    closest_distance = hit.t;
                    Hit hit_in_world_space = Hit(hit.hit, hit.point + position, hit.normal, hit.uv, hit.material,
                                                 hit.t);
                    closest_hit = hit_in_world_space;
                }
            }
        }
        return closest_hit;
    }

    Vec3f TriMesh::getNormal(Vec3f point) {
        // This function should not be called directly
        // Normal information is provided by the intersecting triangle
        return Vec3f();
    }

    Vec2f TriMesh::getUVMappings(Vec3f point) {
        // This function should not be called directly
        // UV mapping information is provided by the intersecting triangle
        return Vec2f();
    }

    BBox TriMesh::getBBox() {
        return bbox;
    }

} //
