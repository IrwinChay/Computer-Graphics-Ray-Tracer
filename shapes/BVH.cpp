/*
 * BVH.cpp
 *
 *
 */
#include "BVH.h"
#include <algorithm>

namespace rt {
    // Bounding Volume Hierarchy
    Hit BVH::intersect(Ray ray) {
        Shape *hit_object = nullptr;
        float t_max = std::numeric_limits<float>::max();
        if (intersectNode(root, ray, hit_object, t_max)) {
            return hit_object->intersect(ray);
        }
        return {};
    }

    Vec3f BVH::getNormal(Vec3f point) {
        return Vec3f();
    }

    Vec2f BVH::getUVMappings(Vec3f point) {
        return Vec2f();
    }

    BBox BVH::getBBox() {
        return BBox();
    }

    void BVH::buildBVH(std::vector<Shape *> objects) {
        // Get the bounding box node of all the objects
        std::vector<BBoxNode *> bbox_nodes;
        for (auto object: objects) {
            BBox bbox = object->getBBox();
            auto *bbox_node = new BBoxNode(bbox, object);
            bbox_nodes.push_back(bbox_node);
        }
        // Build the BVH
        root = new BBoxNode(BBox(), nullptr);
        buildBVHNode(bbox_nodes, 0, bbox_nodes.size(), *root);
        std::cout << "BVH built success" << std::endl;
    }

    void BVH::buildBVHNode(std::vector<BBoxNode *> bbox_nodes, size_t start, size_t end, BBoxNode &parent_node) {
        // Get the bounding box of all the objects
        BBox bbox;
        for (size_t i = start; i < end; i++) {
            auto object_bbox = bbox_nodes[i]->bbox;

            bbox.min.x = std::min(bbox.min.x, object_bbox.min.x);
            bbox.min.y = std::min(bbox.min.y, object_bbox.min.y);
            bbox.min.z = std::min(bbox.min.z, object_bbox.min.z);

            bbox.max.x = std::max(bbox.max.x, object_bbox.max.x);
            bbox.max.y = std::max(bbox.max.y, object_bbox.max.y);
            bbox.max.z = std::max(bbox.max.z, object_bbox.max.z);

            // Update center
            bbox.center = (bbox.min + bbox.max) /= 2;
        }

        parent_node.bbox = bbox;

        // Get the longest axis
        Vec3f diag = parent_node.bbox.max - parent_node.bbox.min;
        int axis = 0;
        if (diag[1] > diag[axis]) {
            axis = 1;
        }
        if (diag[2] > diag[axis]) {
            axis = 2;
        }

        // Sort the objects along the longest axis
        std::sort(bbox_nodes.begin() + start, bbox_nodes.begin() + end, [axis](BBoxNode *a, BBoxNode *b) {
            return a->bbox.center[axis] < b->bbox.center[axis];
        });

        // Get the median
        size_t median = (start + end) / 2;

        if (start == end - 2) {
            parent_node.left = bbox_nodes[start];
            parent_node.right = bbox_nodes[end - 1];
            // can be improve with just if (start == end - 1)
        } else if (start == end - 1) {
            parent_node = *bbox_nodes[start];
        } else {
            parent_node.left = new BBoxNode(BBox(), nullptr);
            parent_node.right = new BBoxNode(BBox(), nullptr);
            buildBVHNode(bbox_nodes, start, median, *parent_node.left);
            buildBVHNode(bbox_nodes, median, end, *parent_node.right);
        }
    }

    bool BVH::hitBBox(BBox bbox, Ray ray, float t_min, float t_max) {
        for (int i = 0; i < 3; i++) {
            float inv_d = 1.0 / ray.direction[i];
            float t0 = (bbox.min[i] - ray.origin[i]) * inv_d;
            float t1 = (bbox.max[i] - ray.origin[i]) * inv_d;

            if (inv_d < 0.0) {
                std::swap(t0, t1);
            }
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;

            if (t_max <= t_min) {
                return false;
            }
        }
        return true;
    }

    bool BVH::intersectNode(BBoxNode *node, Ray ray, Shape *&shape, float &hit_distance_t) {
        // traverse the tree
        if (!hitBBox(node->bbox, ray, 0, hit_distance_t) && node->shape == nullptr) {
            return false;
        }

        if (node->shape != nullptr) {
            Hit hit = node->shape->intersect(ray);
            if (hit.hit && hit.t < hit_distance_t) {
                shape = node->shape;
                hit_distance_t = hit.t;
                return true;
            }
        }

        bool hit_left, hit_right;

        if (node->left == nullptr) {
            hit_left = false;
        } else {
            hit_left = intersectNode(node->left, ray, shape, hit_distance_t);
        }
        if (node->right == nullptr) {
            hit_right = false;
        } else {
            hit_right = intersectNode(node->right, ray, shape, hit_distance_t);
        }

        return hit_left || hit_right;
    }


} //namespace rt


