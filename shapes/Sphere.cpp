/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"


namespace rt {


    /**
     * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
     *
     * @param ray cast ray to check for intersection with shape
     *
     * @return hit struct containing intersection information
     *
     */
    Hit Sphere::intersect(Ray ray) {
        Vec3f ray_to_sphere = center - ray.origin;
        float ray_to_sphere_project_to_ray_dir = ray_to_sphere.dotProduct(ray.direction);

        // check if ray are going away from the sphere
        if (ray_to_sphere_project_to_ray_dir < 0) {
            return Hit();
        }

        float min_distance_sphere_ray = sqrt(ray_to_sphere.dotProduct(ray_to_sphere) -
                                             ray_to_sphere_project_to_ray_dir * ray_to_sphere_project_to_ray_dir);
        if (min_distance_sphere_ray > radius || min_distance_sphere_ray < 0) {
            return Hit();
        }
        float half_chord_length = sqrt(radius * radius - min_distance_sphere_ray * min_distance_sphere_ray);
        float d_factor_0 = ray_to_sphere_project_to_ray_dir - half_chord_length;
        float d_factor_1 = ray_to_sphere_project_to_ray_dir + half_chord_length;

        if (d_factor_0 == d_factor_1 || (d_factor_0 < 0 && d_factor_1 < 0)) {
            return Hit();
        }

        d_factor_0 = d_factor_0 < 0 ? INFINITY : d_factor_0;
        d_factor_1 = d_factor_1 < 0 ? INFINITY : d_factor_1;
        float t = d_factor_0 < d_factor_1 ? d_factor_0 : d_factor_1;

        Vec3f hit_point = ray.origin + t * ray.direction;

        return Hit(true, hit_point, this->getNormal(hit_point), this->getUVMappings(hit_point), material, t);
    }

    Vec3f Sphere::getNormal(Vec3f point) {
        return (point - center).normalize();
    }

    Vec2f Sphere::getUVMappings(Vec3f point) {
        Vec3f normal = getNormal(point);
        float u = 0.5 + atan2(normal.z, normal.x) / (2 * M_PI);
        float v = 0.5 - asin(normal.y) / M_PI;
        return Vec2f(u, v);
    }

    BBox Sphere::getBBox() {
        return BBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }


} //namespace rt


