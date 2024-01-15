//
// Created by moon9 on 19/03/2023.
//

#ifndef RAY_TRACER_BBOXNODE_H
#define RAY_TRACER_BBOXNODE_H

#include "core/Shape.h"

namespace rt {
    struct BBoxNode {
        BBox bbox;
        Shape *shape;
        BBoxNode *left;
        BBoxNode *right;

        BBoxNode(BBox bbox, Shape *shape) : bbox(bbox), shape(shape), left(nullptr), right(nullptr) {};

        BBoxNode(BBox bbox, BBoxNode *left, BBoxNode *right) : bbox(bbox),
                                                               left(left),
                                                               right(right),
                                                               shape(nullptr) {};
    };
}

#endif //RAY_TRACER_BBOXNODE_H
