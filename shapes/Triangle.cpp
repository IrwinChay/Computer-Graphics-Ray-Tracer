/*
 * Triangle.cpp
 *
 *
 */
#include "Triangle.h"


namespace rt {

    Hit Triangle::intersect(Ray ray) {
        Vec3f line_0_1 = v1 - v0;
        Vec3f line_0_2 = v2 - v0;
        Vec3f line_1_2 = v2 - v1;
        Vec3f line_2_0 = v0 - v2;

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

        // check if sign of dot product is the same for all 4 lines
        float sign_0_1 = line_0_1.crossProduct(line_0_p).dotProduct(plane_normal);
        float sign_1_2 = line_1_2.crossProduct(line_1_p).dotProduct(plane_normal);
        float sign_2_0 = line_2_0.crossProduct(line_2_p).dotProduct(plane_normal);

        if (sign_0_1 < 0 || sign_1_2 < 0 || sign_2_0 < 0) {
            return {}; // intersection is outside the plane
        }

        Vec3f hit_point = ray.origin + x * ray.direction;

        return {true, hit_point, this->getNormal(hit_point), this->getUVMappings(hit_point), material, x};
    }

    Vec3f Triangle::getNormal(Vec3f point) {
        if (hasNormals) {
            // Barycentric coordinates calculation
            Vec3f line_0_1 = v1 - v0;
            Vec3f line_0_2 = v2 - v0;
            Vec3f line_0_p = point - v0;

            Vec3f line_1_p = point - v1;
            Vec3f line_2_p = point - v2;

            float denominator = line_0_1.crossProduct(line_0_2).length(); // normal of surface length

            float alpha = line_1_p.crossProduct(line_2_p).length() / denominator;
            float beta = line_2_p.crossProduct(line_0_p).length() / denominator;
            float gamma = 1 - alpha - beta;

            return (alpha * n0 + beta * n1 + gamma * n2).normalize();
        } else {
            Vec3f line_point_1 = v1 - point;
            Vec3f line_point_2 = v2 - point;
            return line_point_1.crossProduct(line_point_2).normalize();
        }
    }

    Vec2f Triangle::getUVMappings(Vec3f point) {
        if (hasUV) {
            // Barycentric coordinates calculation
            Vec3f line_0_1 = v1 - v0;
            Vec3f line_0_2 = v2 - v0;
            Vec3f line_0_p = point - v0;

            Vec3f line_1_p = point - v1;
            Vec3f line_2_p = point - v2;

            float denominator = line_0_1.crossProduct(line_0_2).length(); // normal of surface length

            float alpha = line_1_p.crossProduct(line_2_p).length() / denominator;
            float beta = line_2_p.crossProduct(line_0_p).length() / denominator;
            float gamma = 1 - alpha - beta;

            return alpha * t0 + beta * t1 + gamma * t2;
        } else {
            Vec3f line_0_1 = v1 - v0;
            Vec3f line_1_2 = v2 - v1;
            Vec3f line_2_0 = v0 - v2;

            Vec3f width;
            Vec3f height;
            Vec3f relative_point;
            // find tangent vectors (since v012 aren't necessarily orthogonal)
            if (line_0_1.length() > line_1_2.length() && line_0_1.length() > line_2_0.length()) {
                width = line_1_2;
                height = line_2_0;
                relative_point = point - v2;
            } else if (line_1_2.length() > line_0_1.length() && line_1_2.length() > line_2_0.length()) {
                width = line_1_2;
                height = line_0_1;
                relative_point = point - v0;
            } else {
                width = line_0_1;
                height = line_1_2;
                relative_point = point - v1;
            }

            float u = relative_point.dotProduct(width) / width.dotProduct(width);
            float v = relative_point.dotProduct(height) / height.dotProduct(height);

            return {u, v};
        }
    }

    BBox Triangle::getBBox() {
        float min_x = std::min(std::min(v0.x, v1.x), v2.x);
        float min_y = std::min(std::min(v0.y, v1.y), v2.y);
        float min_z = std::min(std::min(v0.z, v1.z), v2.z);
        float max_x = std::max(std::max(v0.x, v1.x), v2.x);
        float max_y = std::max(std::max(v0.y, v1.y), v2.y);
        float max_z = std::max(std::max(v0.z, v1.z), v2.z);

        return {Vec3f(min_x, min_y, min_z), Vec3f(max_x, max_y, max_z)};
    }

} //namespace rt
