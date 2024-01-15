/*
 * Material.cpp
 *
 */
#include "Material.h"
#include "parsers/TextureReader.h"

namespace rt {


    Material::~Material() {

    }

    void Material::loadTexture(const std::string &filename) {
        if (TextureReader::read_jpeg_to_framebuffer(filename, framebuffer, width, height, channels)) {
            std::cout << "Successfully read " << filename << " with dimensions: " << width << "x" << height << " and "
                      << channels << " channels." << std::endl;
        } else {
            std::cerr << "Failed to read " << filename << std::endl;
        }
    }


} //namespace rt


