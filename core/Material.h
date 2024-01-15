/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector>
#include "geometry.h"

namespace rt {

    class Material {
    public:
        Material(Vec3f diffuse_color) {
            this->diffuse_color = diffuse_color;
            this->has_texture = false;
        };

        Material(Vec3f diffuse_color, const std::string &texture_path) {
            this->diffuse_color = diffuse_color;
            this->has_texture = true;
            loadTexture(texture_path);
        };

        virtual ~Material();

        bool getHasTexture() {
            return has_texture;
        }

        Vec3f getDiffuseColor() {
            return diffuse_color;
        }

        Vec3f getTextureColor(Vec2f uv) {
            if (has_texture) {
                float u = uv.x;
                float v = uv.y;
                int x = (int) (u * width);
                int y = (int) (v * height);
                int index = (y * width + x) * channels;
                float r = static_cast<float>(framebuffer[index]) / 255;
                float g = static_cast<float>(framebuffer[index + 1]) / 255;
                float b = static_cast<float>(framebuffer[index + 2]) / 255;
                return Vec3f(r, g, b);
            } else {
                return diffuse_color;
            }
        }

    private:
        Vec3f diffuse_color;
        bool has_texture;
        std::vector<unsigned char> framebuffer;
        unsigned int width, height, channels;

        void loadTexture(const std::string &filename);
    };


} //namespace rt



#endif /* MATERIAL_H_ */
