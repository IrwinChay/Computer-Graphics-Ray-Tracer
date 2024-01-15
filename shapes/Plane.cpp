//
// Created by moon9 on 04/03/2023.
//

#include "Plane.h"

namespace rt {


    /**
     * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
     *
     * @param ray cast ray to check for intersection with shape
     *
     * @return hit struct containing intersection information
     *
     */
    Hit Plane::intersect(Ray ray) {
        // compute the intersection of the ray with the plane
        // if there is an intersection, fill the hit structure and return it
        // if there is no intersection, return the default hit structure (see default constructor of Hit)

        Vec3f line_0_1 = v1 - v0;
        Vec3f line_0_2 = v2 - v0;

        Vec3f line_1_2 = v2 - v1;
        Vec3f line_2_3 = v3 - v2;
        Vec3f line_3_0 = v0 - v3;

        Vec3f plane_normal = line_0_1.crossProduct(line_0_2);

        if (plane_normal.dotProduct(ray.direction) == 0) {
            return {}; // parallel, no intersection
        }

        float d = plane_normal.dotProduct(v1);
        float x = (d - plane_normal.dotProduct(ray.origin)) / plane_normal.dotProduct(ray.direction);
        if (x < 0) {
            return {}; // intersection is behind the ray
        }

        Vec3f intersection_p = ray.origin + x * ray.direction;

        // check if intersection is inside the plane
        Vec3f line_0_p = intersection_p - v0;
        Vec3f line_1_p = intersection_p - v1;
        Vec3f line_2_p = intersection_p - v2;
        Vec3f line_3_p = intersection_p - v3;

        // check if sign of dot product is the same for all 4 lines
        float sign_0_1 = line_0_1.crossProduct(line_0_p).dotProduct(plane_normal);
        float sign_1_2 = line_1_2.crossProduct(line_1_p).dotProduct(plane_normal);
        float sign_2_3 = line_2_3.crossProduct(line_2_p).dotProduct(plane_normal);
        float sign_3_0 = line_3_0.crossProduct(line_3_p).dotProduct(plane_normal);

        if (sign_0_1 < 0 || sign_1_2 < 0 || sign_2_3 < 0 || sign_3_0 < 0) {
            return {}; // intersection is outside the plane
        }

        Vec3f hit_point = ray.origin + x * ray.direction;

        return {true, hit_point, this->getNormal(hit_point), this->getUVMappings(hit_point), material, x};
    }

    Vec3f Plane::getNormal(Vec3f point) {
        Vec3f line_point_1 = v1 - point;
        Vec3f line_point_2 = v2 - point;
        return -line_point_1.crossProduct(line_point_2).normalize();
    }

    Vec2f Plane::getUVMappings(Vec3f point) {
        Vec3f relative_point = point - v0;
        Vec3f line_0_1 = v1 - v0;
        Vec3f line_0_2 = v2 - v0;
        Vec3f line_0_3 = v3 - v0;

        Vec3f width;
        Vec3f height;
        // find tangent vectors (since v0123 aren't necessarily orthogonal)
        if (line_0_1.normalize().dotProduct(line_0_2.normalize()) == 0) {
            width = v1 - v0;
            height = v2 - v0;
        } else if (line_0_2.normalize().dotProduct(line_0_3.normalize()) == 0) {
            width = v2 - v0;
            height = v3 - v0;
        } else {
            width = v3 - v0;
            height = v1 - v0;
        }

        float u = relative_point.dotProduct(width) / width.dotProduct(width);
        float v = relative_point.dotProduct(height) / height.dotProduct(height);

        return {u, v};
    }

    BBox Plane::getBBox() {
        // find min and max x, y, z values
        float min_x = std::min(std::min(v0.x, v1.x), std::min(v2.x, v3.x));
        float min_y = std::min(std::min(v0.y, v1.y), std::min(v2.y, v3.y));
        float min_z = std::min(std::min(v0.z, v1.z), std::min(v2.z, v3.z));

        float max_x = std::max(std::max(v0.x, v1.x), std::max(v2.x, v3.x));
        float max_y = std::max(std::max(v0.y, v1.y), std::max(v2.y, v3.y));
        float max_z = std::max(std::max(v0.z, v1.z), std::max(v2.z, v3.z));
        
        return BBox(Vec3f(min_x, min_y, min_z), Vec3f(max_x, max_y, max_z));
    }


} //namespace rt