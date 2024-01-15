/*
 * Scene.cpp
 *
 */
#include "Scene.h"
#include <string>


namespace rt {

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
    void Scene::createScene(Value &scenespecs) {

        //----------parse json object to populate scene-----------
        for (Value::ConstMemberIterator itr = scenespecs.MemberBegin(); itr != scenespecs.MemberEnd(); ++itr) {
            const auto section_name = itr->name.GetString();
            if (std::strcmp(section_name, "backgroundcolor") == 0) {
                printf("Initial Background Color\n");
                backgroundColor[0] = itr->value[0].GetFloat();
                backgroundColor[1] = itr->value[1].GetFloat();
                backgroundColor[2] = itr->value[2].GetFloat();
            } else if (std::strcmp(section_name, "lightsources") == 0) {
                printf("Initial Light Source\n");
                for (SizeType i = 0; i < itr->value.Size(); i++) {
                    if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "pointlight") == 0) {
                        printf("Got pointlight\n");
                        Vec3f position;
                        Vec3f intensity_specular;
                        Vec3f diffuse_intensity;
                        position[0] = itr->value[i].GetObject()["position"][0].GetFloat();
                        position[1] = itr->value[i].GetObject()["position"][1].GetFloat();
                        position[2] = itr->value[i].GetObject()["position"][2].GetFloat();
                        intensity_specular[0] = itr->value[i].GetObject()["is"][0].GetFloat();
                        intensity_specular[1] = itr->value[i].GetObject()["is"][1].GetFloat();
                        intensity_specular[2] = itr->value[i].GetObject()["is"][2].GetFloat();
                        diffuse_intensity[0] = itr->value[i].GetObject()["id"][0].GetFloat();
                        diffuse_intensity[1] = itr->value[i].GetObject()["id"][1].GetFloat();
                        diffuse_intensity[2] = itr->value[i].GetObject()["id"][2].GetFloat();
                        lightSources.push_back(new PointLight(position, intensity_specular, diffuse_intensity));
                    } else if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "arealight") == 0) {
                        printf("Got arealight\n");
                        Vec3f intensity_specular, diffuse_intensity;
                        Vec3f v0, v1, v2, v3;
                        intensity_specular[0] = itr->value[i].GetObject()["is"][0].GetFloat();
                        intensity_specular[1] = itr->value[i].GetObject()["is"][1].GetFloat();
                        intensity_specular[2] = itr->value[i].GetObject()["is"][2].GetFloat();
                        diffuse_intensity[0] = itr->value[i].GetObject()["id"][0].GetFloat();
                        diffuse_intensity[1] = itr->value[i].GetObject()["id"][1].GetFloat();
                        diffuse_intensity[2] = itr->value[i].GetObject()["id"][2].GetFloat();
                        v0[0] = itr->value[i].GetObject()["v0"][0].GetFloat();
                        v0[1] = itr->value[i].GetObject()["v0"][1].GetFloat();
                        v0[2] = itr->value[i].GetObject()["v0"][2].GetFloat();
                        v1[0] = itr->value[i].GetObject()["v1"][0].GetFloat();
                        v1[1] = itr->value[i].GetObject()["v1"][1].GetFloat();
                        v1[2] = itr->value[i].GetObject()["v1"][2].GetFloat();
                        v2[0] = itr->value[i].GetObject()["v2"][0].GetFloat();
                        v2[1] = itr->value[i].GetObject()["v2"][1].GetFloat();
                        v2[2] = itr->value[i].GetObject()["v2"][2].GetFloat();
                        v3[0] = itr->value[i].GetObject()["v3"][0].GetFloat();
                        v3[1] = itr->value[i].GetObject()["v3"][1].GetFloat();
                        v3[2] = itr->value[i].GetObject()["v3"][2].GetFloat();
                        lightSources.push_back(new AreaLight(v0, v1, v2, v3, intensity_specular, diffuse_intensity));
                    } else {
                        printf("unknown light source\n");
                    }
                }
            } else if (std::strcmp(section_name, "shapes") == 0) {
                printf("Initial Shapes\n");
                for (SizeType i = 0; i < itr->value.Size(); i++) {
                    float ks, kd, kr, ior, specular_exponent;
                    Vec3f diffuse_color;
                    BlinnPhong *material;
                    ks = itr->value[i].GetObject()["material"]["ks"].GetFloat();
                    kd = itr->value[i].GetObject()["material"]["kd"].GetFloat();
                    if (itr->value[i].GetObject()["material"].HasMember("kr")) {
                        kr = itr->value[i].GetObject()["material"]["kr"].GetFloat();
                    } else {
                        kr = 0;
                    }
                    if (itr->value[i].GetObject()["material"].HasMember("ior")) {
                        ior = itr->value[i].GetObject()["material"]["ior"].GetFloat();
                    } else {
                        ior = 0;
                    }
                    specular_exponent = itr->value[i].GetObject()["material"]["specularexponent"].GetFloat();
                    diffuse_color[0] = itr->value[i].GetObject()["material"]["diffusecolor"][0].GetFloat();
                    diffuse_color[1] = itr->value[i].GetObject()["material"]["diffusecolor"][1].GetFloat();
                    diffuse_color[2] = itr->value[i].GetObject()["material"]["diffusecolor"][2].GetFloat();
                    if (itr->value[i].GetObject()["material"].HasMember("tPath")) {
                        const char *texture_path = itr->value[i].GetObject()["material"]["tPath"].GetString();
                        material = new BlinnPhong(ks, kd, kr, specular_exponent, ior, diffuse_color, texture_path);
                    } else {
                        material = new BlinnPhong(ks, kd, kr, specular_exponent, ior, diffuse_color);
                    }
                    std::string id;
                    if (itr->value[i].GetObject().HasMember("id")) {
                        id = itr->value[i].GetObject()["id"].GetString();
                    } else {
                        id = itr->value[i].GetObject()["type"].GetString();
                    }
                    if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "sphere") == 0) {
                        printf("Got sphere\n");
                        Vec3f center;
                        float radius;
                        center[0] = itr->value[i].GetObject()["center"][0].GetFloat();
                        center[1] = itr->value[i].GetObject()["center"][1].GetFloat();
                        center[2] = itr->value[i].GetObject()["center"][2].GetFloat();
                        radius = itr->value[i].GetObject()["radius"].GetFloat();
                        shapes.push_back(new Sphere(center, radius, material, id));
                    } else if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "plane") == 0) {
                        printf("Got plane\n");
                        Vec3f v0;
                        Vec3f v1;
                        Vec3f v2;
                        Vec3f v3;
                        v0[0] = itr->value[i].GetObject()["v0"][0].GetFloat();
                        v0[1] = itr->value[i].GetObject()["v0"][1].GetFloat();
                        v0[2] = itr->value[i].GetObject()["v0"][2].GetFloat();
                        v1[0] = itr->value[i].GetObject()["v1"][0].GetFloat();
                        v1[1] = itr->value[i].GetObject()["v1"][1].GetFloat();
                        v1[2] = itr->value[i].GetObject()["v1"][2].GetFloat();
                        v2[0] = itr->value[i].GetObject()["v2"][0].GetFloat();
                        v2[1] = itr->value[i].GetObject()["v2"][1].GetFloat();
                        v2[2] = itr->value[i].GetObject()["v2"][2].GetFloat();
                        v3[0] = itr->value[i].GetObject()["v3"][0].GetFloat();
                        v3[1] = itr->value[i].GetObject()["v3"][1].GetFloat();
                        v3[2] = itr->value[i].GetObject()["v3"][2].GetFloat();
                        shapes.push_back(new Plane(v0, v1, v2, v3, material, id));
                    } else if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "triangle") == 0) {
                        printf("Got triangle\n");
                        Vec3f v0;
                        Vec3f v1;
                        Vec3f v2;
                        v0[0] = itr->value[i].GetObject()["v0"][0].GetFloat();
                        v0[1] = itr->value[i].GetObject()["v0"][1].GetFloat();
                        v0[2] = itr->value[i].GetObject()["v0"][2].GetFloat();
                        v1[0] = itr->value[i].GetObject()["v1"][0].GetFloat();
                        v1[1] = itr->value[i].GetObject()["v1"][1].GetFloat();
                        v1[2] = itr->value[i].GetObject()["v1"][2].GetFloat();
                        v2[0] = itr->value[i].GetObject()["v2"][0].GetFloat();
                        v2[1] = itr->value[i].GetObject()["v2"][1].GetFloat();
                        v2[2] = itr->value[i].GetObject()["v2"][2].GetFloat();
                        shapes.push_back(new Triangle(v0, v1, v2, material, id));
                    } else if (std::strcmp(itr->value[i].GetObject()["type"].GetString(), "mesh") == 0) {
                        printf("Got mesh\n");
                        Vec3f position;
                        float scale;
                        position[0] = itr->value[i].GetObject()["position"][0].GetFloat();
                        position[1] = itr->value[i].GetObject()["position"][1].GetFloat();
                        position[2] = itr->value[i].GetObject()["position"][2].GetFloat();
                        scale = itr->value[i].GetObject()["scale"].GetFloat();
                        std::string mesh_path = itr->value[i].GetObject()["path"].GetString();
                        shapes.push_back(new TriMesh(material, id, mesh_path, position, scale));
                    } else {
                        printf("unknown shape\n");
                    }
                }
            } else {
                printf("unknown section\n");
            }
        }
        // Build BVH
        bvh = new BVH(shapes);
    }


} //namespace rt
