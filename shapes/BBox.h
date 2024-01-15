//
// Created by moon9 on 17/03/2023.
//

#ifndef RAY_TRACER_BBOX_H
#define RAY_TRACER_BBOX_H

#include "geometry.h"
#include "core/Shape.h"

namespace rt{

struct BBox{
    Vec3f min;
    Vec3f max;
    Vec3f center;
    BBox():min(INFINITY, INFINITY, INFINITY), max(-INFINITY, -INFINITY, -INFINITY), center((min+max)/=2){};
    BBox(Vec3f min, Vec3f max):min(min), max(max), center((min+max)/=2){};
    };
}

#endif //RAY_TRACER_BBOX_H
