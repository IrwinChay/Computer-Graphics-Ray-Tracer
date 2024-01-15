/*
 * main.cpp
 *
 */

#include <iostream>
#include <fstream>
#include <chrono>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "math/geometry.h"
#include "parsers/PPMWriter.h"
//#include "parsers/TextureReader.h"


#include "core/RayTracer.h"
#include "core/Camera.h"
#include "core/Scene.h"


using namespace rt;
using namespace rapidjson;

int main(int argc, char *argv[]) {

    //parse commandline arguments
    char *inputFile = argv[1];    //first command line argument holds the path to the json input file
    char *outputFile = argv[2];   //second command line argument holds the path to the output image file
    int sample_number_override = -1;
    if (argc > 3){
        sample_number_override = std::atoi(argv[3]);
        std::cout << "Using sample number overridde:" << sample_number_override << std::endl;
    }


    std::printf("Input file: %s\n", inputFile);

    //parse json input file using rapidjson
    std::ifstream ifs(inputFile);
    IStreamWrapper is(ifs);
    Document d;     // d holds the complete json object
    d.ParseStream(is);

    //generate a camera according to the input file
    Camera *camera = Camera::createCamera(d["camera"]);

    //print camera data (based on the input file provided)
    camera->printCamera();

    //generate the scene according to the input file
    Scene *scene = new Scene();
    scene->createScene(d["scene"]);

    //
    // Main function, render scene
    //

    int n_bounces = d["nbounces"].GetInt();
    int sample_number = 1;
    if (d.HasMember("sample_number")) {
        sample_number = d["sample_number"].GetInt();
    }
    if (sample_number_override > 0){
        sample_number = sample_number_override;
    }
    bool use_jitter_sample = true;
    if (d.HasMember("use_jitter_sample")) {
        use_jitter_sample = d["use_jitter_sample"].GetBool();
    }
    bool use_bvh = true;
    if (d.HasMember("use_bvh")) {
        use_bvh = d["use_bvh"].GetBool();
    }
    scene->set_n_bound(n_bounces);
    scene->set_sample(sample_number);
    scene->set_use_jitter_sample(use_jitter_sample);
    scene->set_use_bvh(use_bvh);

    auto start = std::chrono::high_resolution_clock::now();
    Vec3f *pixelbuffer = RayTracer::render(camera, scene);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Render execution time: " << duration << " milliseconds" << std::endl;

    //convert linear RGB pixel values [0-1] to range 0-255
    RayTracer::tonemap(pixelbuffer, camera->getWidth() * camera->getHeight());


    std::printf("Output file: %s\n", outputFile);

    //write rendered scene to file (pixels RGB values must be in range 0255)
    PPMWriter::PPMWriter(camera->getWidth(), camera->getHeight(), pixelbuffer, outputFile);

    //free resources when rendering is finished
    delete pixelbuffer;
    delete camera;
    delete scene;
}


