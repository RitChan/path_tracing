/**
 * @file collider.cpp
 * @brief 
 */
#include <collider.h>
#include <cmath>


AABB aabb_merge(const AABB &a, const AABB &b) {
    return AABB(
        std::min(a.p0[0], b.p0[0]), std::max(a.p1[0], b.p1[0]), 
        std::min(a.p0[1], b.p0[1]), std::max(a.p1[1], b.p1[1]), 
        std::min(a.p0[2], b.p0[2]), std::max(a.p1[2], b.p1[2])
    );
}


bool aabb_collide(const AABB &a, const AABB &b) {
    return !(
        (a.p1[0] < b.p0[0] || b.p1[0] < a.p0[0]) && 
        (a.p1[1] < b.p0[1] || b.p1[1] < a.p0[1]) && 
        (a.p1[2] < b.p0[2] || b.p1[2] < a.p0[2])
    );
}
