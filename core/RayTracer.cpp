/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"
#include <random>

namespace rt {


/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */
    Vec3f *RayTracer::render(Camera *camera, Scene *scene) {

        Vec3f *pixelbuffer = new Vec3f[camera->getWidth() * camera->getHeight()];

        // Add random number generator for super sampling
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dist(0.0, 1.0);

        int total_rows = camera->getHeight();
        int total_pixels = total_rows * camera->getWidth();
        int progress_interval = total_pixels / 100; // Update progress every 1%
        int next_progress_update = progress_interval;
        int processed_pixels = 0;
        float progress_percentage;

        int sample_number = scene->get_sample();
        int nbounces = scene->get_n_bound();
        bool use_jitter = scene->get_use_jitter_sample();

        //----------main rendering function to be filled------
        for (int i = 0; i < camera->getHeight(); i++) {
            for (int j = 0; j < camera->getWidth(); j++) {
                Vec3f pixel;
                // todo implement super sampling with jitter
                if (sample_number > 0) {
                    Vec3f pixel_sum = Vec3f(0, 0, 0);
                    if (use_jitter) {
                        // Super sampling - jitter
                        int root_sample = sqrt(sample_number);
                        float i_unit = 1.0f / root_sample;
                        float j_unit = 1.0f / root_sample;
                        for (int k = 0; k < root_sample; k++) {
                            for (int l = 0; l < root_sample; l++) {
                                float jitter_i = i_unit * (k + dist(gen));
                                float jitter_j = j_unit * (l + dist(gen));

                                Ray ray = camera->generateRay((float) i + jitter_i, (float) j + jitter_j);
                                Vec3f sample_pixel = trace(ray, scene, nbounces);
                                pixel_sum = pixel_sum + sample_pixel;
                            }
                        }
                        pixel_sum = pixel_sum /= (root_sample * root_sample);
                        pixel = pixel_sum;
                    } else {
                        // Super sampling - random
                        for (int k = 0; k < sample_number; k++) {
                            float jitter_i = dist(gen);
                            float jitter_j = dist(gen);

                            Ray ray = camera->generateRay((float) i + jitter_i, (float) j + jitter_j);
                            Vec3f sample_pixel = trace(ray, scene, nbounces);
                            pixel_sum = pixel_sum + sample_pixel;
                        }
                        pixel_sum = pixel_sum /= (float) sample_number;
                        pixel = pixel_sum;
                    }
                } else {
                    // Normal Single Ray Trace
                    Ray ray = camera->generateRay(i, j);
                    pixel = trace(ray, scene, nbounces);
                }
                pixelbuffer[i * camera->getWidth() + j] = pixel;


                // Progress report
                processed_pixels++;
                if (processed_pixels >= next_progress_update) {
                    progress_percentage = static_cast<float>(processed_pixels) / total_pixels * 100;
                    std::cout << "Render progress: " << progress_percentage << "%" << std::endl;
                    next_progress_update += progress_interval;
                }
            }
        }

        return pixelbuffer;

    }

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
    Vec3f *RayTracer::tonemap(Vec3f *pixelbuffer, int totalPixels) {

        //---------tonemapping function to be filled--------
        for (int i = 0; i < totalPixels; i++) {
            pixelbuffer[i] = pixelbuffer[i] * 255;
        }
        return pixelbuffer;

    }

    Vec3f RayTracer::trace(Ray ray, Scene *scene, int nbounces) {
        // check ray with all objects in scene to see if there is any intersection
        bool enable_bvh = scene->get_use_bvh();
        Hit closestHit;
        if (enable_bvh) {
            Hit bvh_hit = scene->getBVH()->intersect(ray);
            closestHit = bvh_hit;
        } else {
            for (int i = 0; i < scene->getShapes().size(); i++) {
                Hit hit = scene->getShapes()[i]->intersect(ray);
                if (hit.hit && hit.t < closestHit.t) {
                    closestHit = hit;
                    //std::cout << "closest hit shape id: " << scene->getShapes()[i]->getId() << std::endl;
                } else {
                    continue;
                }
            }
        }

        if (!closestHit.hit) {
            return scene->getBackgroundColor();
        } else {
            Vec3f final_colour;
            if (ray.inside && (ray.raytype == REFRACTION)) {
                final_colour = {0, 0, 0};
            } else {
                final_colour = getLocalIllumination(ray, scene, closestHit);
            }
            // check if the ray has reflection
            if (nbounces > 0 && closestHit.material->getKr() > 0) {
                Vec3f l = -ray.direction;
                Vec3f n = closestHit.normal;
                Vec3f reflection_direction = (2 * n.dotProduct(l) * n - l).normalize();
                Vec3f reflection_origin = closestHit.point + 0.001 * n; // avoid self intersection
                Ray reflection_ray = Ray(REFLECTION, reflection_origin, reflection_direction, ray.inside);
                Vec3f reflection_colour = trace(reflection_ray, scene, nbounces - 1);
                final_colour = final_colour + closestHit.material->getKr() * reflection_colour;
            }
            // check if the ray has refraction
            if (nbounces > 0 && closestHit.material->getIor() > 0) {
                float eta = (ray.inside) ? closestHit.material->getIor() : 1 / closestHit.material->getIor();
                Vec3f l = -ray.direction;
                Vec3f n = (ray.inside) ? -closestHit.normal : closestHit.normal;
                float cosThetaI = l.dotProduct(n);
                // (n1/n2)^2 = (1-cos1^2)/(1-cos2^2)
                float k = 1 - eta * eta * (1 - cosThetaI * cosThetaI);

                if (k >= 0) { // No total internal reflection, produce a refraction ray
                    Vec3f refractedDirection = eta * -l + (eta * cosThetaI - sqrt(k)) * -n;
                    refractedDirection = refractedDirection.normalize();
                    Vec3f refractedOrigin = closestHit.point + 0.001 * -n; // Avoid self-intersection
                    Ray refractedRay = Ray(REFRACTION, refractedOrigin, refractedDirection, !ray.inside);
                    Vec3f refractedColor = trace(refractedRay, scene, nbounces - 1);
                    final_colour = final_colour + closestHit.material->getKt() * refractedColor;
                } else { // Total internal reflection, produce a reflection ray
                    Vec3f reflectionDirection = (2 * n.dotProduct(l) * n - l).normalize();
                    Vec3f reflectionOrigin = closestHit.point + 0.001f * n; // Avoid self-intersection
                    Ray reflectionRay = Ray(REFLECTION, reflectionOrigin, reflectionDirection, ray.inside);
                    Vec3f reflectionColor = trace(reflectionRay, scene, nbounces - 1);
                    final_colour = final_colour + closestHit.material->getKt() * reflectionColor;
                }
            }
            for (int j = 0; j < 3; j++) {
                if (final_colour[j] > 1) {
                    final_colour[j] = 1;
                }
            }
            return final_colour;
        }
    }

    Vec3f RayTracer::getLocalIllumination(Ray ray, Scene *scene, Hit hit) {
        // get normal and hit position
        Vec3f normal = hit.normal;
        Vec3f hit_position = hit.point;
        BlinnPhong *hit_material = hit.material;
        int sample_number = scene->get_sample();
        bool enable_bvh = scene->get_use_bvh();
        bool use_jitter = scene->get_use_jitter_sample();

        Vec3f I(0, 0, 0);
        for (int i = 0; i < scene->getLightSources().size(); i++) {
            // shadow contribution check
            float shadow_factor = 1e-8f;
            Vec3f avg_l = {1e-8f, 1e-8f, 1e-8f};
            for (int j = 0; j < sample_number; j++) {
                Vec3f random_point_on_light;
                if (use_jitter) {
                    random_point_on_light = scene->getLightSources()[i]->getJitterSampling(sample_number, j);
                } else {
                    random_point_on_light = scene->getLightSources()[i]->getRandomPointOnLight();
                }
                Vec3f line_light_hit = hit.point - random_point_on_light;
                avg_l = avg_l + (-line_light_hit); // invert direction vector
                Ray shadow_ray = Ray(SHADOW, random_point_on_light, line_light_hit.normalize());

                Hit closestHit;
                if (enable_bvh) {
                    Hit bvh_hit = scene->getBVH()->intersect(shadow_ray);
                    closestHit = bvh_hit;
                } else {
                    for (int k = 0; k < scene->getShapes().size(); k++) {
                        Shape *shape = scene->getShapes()[k];
                        Hit shadow_hit = shape->intersect(shadow_ray);
                        if (shadow_hit.hit && (!closestHit.hit || (shadow_hit.point - shadow_ray.origin).length() <
                                                                  (closestHit.point - shadow_ray.origin).length())) {
                            closestHit = shadow_hit;
                        }
                    }
                }
                if (closestHit.hit && (closestHit.point - hit.point).length() < 0.001) {
                    shadow_factor += 1; // not shadow
                } else {
                    shadow_factor += 0; // is shadow
                }
            }
            shadow_factor = shadow_factor / sample_number;
            avg_l = avg_l /= sample_number;

            float distance_factor = 1 / (4 * M_PI * pow(avg_l.length(), 3));
            Vec3f ls = scene->getLightSources()[0]->getIntensitySpecular() * distance_factor * shadow_factor;
            Vec3f ld = scene->getLightSources()[0]->getDiffuseIntensity() * distance_factor * shadow_factor;

            // get material info
            Vec3f diffuseColor;
            if (hit.material->getHasTexture()) {
                diffuseColor = hit.material->getTextureColor(hit.uv);
            } else {
                diffuseColor = hit.material->getDiffuseColor();
            }

            float kd = hit_material->getKd();
            float ks = hit_material->getKs();
            float specular_exponent = hit_material->getSpecularExponent();

            // calculate notation
            Vec3f l = avg_l;
            Vec3f n = normal;
            Vec3f r = (2 * n.dotProduct(l) * n - l).normalize();
            Vec3f v = (ray.origin - hit_position).normalize();

            // calculate diffuse reflection
            Vec3f I_d = kd * ld * std::max(0.0f, l.dotProduct(n)) * diffuseColor;

            // calculate specular reflection
            Vec3f I_s = ks * ls * std::max(0.0f, (float) pow(r.dotProduct(v), specular_exponent));

            // calculate ambient reflection
            // None ambient refection
            Vec3f I_a = Vec3f(0, 0, 0);

            // calculate total reflection
            I = I_d + I_s + I_a;
        }

        Vec3f final_colour = I;

        return final_colour;
    }

} //namespace rt


