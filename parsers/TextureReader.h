//
// Created by moon9 on 15/03/2023.
//

#ifndef RAY_TRACER_TEXTUREREADER_H
#define RAY_TRACER_TEXTUREREADER_H

#include <string>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "geometry.h"
#include <vector>

namespace TextureReader {
    bool
    read_jpeg_to_framebuffer(const std::string &filename, std::vector<unsigned char> &framebuffer, unsigned int &width,
                             unsigned int &height, unsigned int &channels) {
        int w, h, c;
        unsigned char *data = stbi_load(filename.c_str(), &w, &h, &c, 0);
        if (!data) {
            return false;
        }

        width = static_cast<unsigned int>(w);
        height = static_cast<unsigned int>(h);
        channels = static_cast<unsigned int>(c);

        framebuffer.assign(data, data + width * height * channels);
        stbi_image_free(data);
        return true;
    }
};

#endif //RAY_TRACER_TEXTUREREADER_H
