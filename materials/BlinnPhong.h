/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"


namespace rt {

    class BlinnPhong : public Material {
    public:

        BlinnPhong(float ks, float kd, float kr, float specular_exponent, float ior, Vec3f diffuse_color) : Material(
                diffuse_color) {
            this->ks = ks;
            this->kd = kd;
            this->kr = kr;
            this->kt = 1 - kr; // use simple modeling
            this->ior = ior;
            this->specular_exponent = specular_exponent;
        };

        BlinnPhong(float ks, float kd, float kr, float specular_exponent, float ior, Vec3f diffuse_color,
                   const std::string &texture_path) : Material(diffuse_color, texture_path) {
            this->ks = ks;
            this->kd = kd;
            this->kr = kr;
            this->kt = 1 - kr; // use simple modeling
            this->ior = ior;
            this->specular_exponent = specular_exponent;
        };

        float getKs() {
            return ks;
        }

        float getKd() {
            return kd;
        }

        float getKr() {
            return kr;
        }

        float getKt() {
            return kt;
        }

        float getIor() {
            return ior;
        }

        float getSpecularExponent() {
            return specular_exponent;
        }

    private:
        float ks; // fraction of specular light reflected from surface
        float kd; // fraction of diffuse light reflected from surface
        float kr; // fraction of light reflected from surface
        float kt; // fraction of light transmitted through surface
        float ior; // index of refraction
        float specular_exponent; // shine ness of the surface
    };


} //namespace rt



#endif /* BLINNPHONG_H_ */
