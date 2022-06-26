/**
 * @file bvh.h
 * @brief 实现BVHTree
 */
#ifndef __BVH_H__
#define __BVH_H__

#include <surface.h>

/**
 * @brief (Has Pointer)
 */
class BVHTree {
public:
    AABB aabb = AABB(0, 0, 0, 0, 0, 0);
    BVHTree *left = nullptr;
    BVHTree *right = nullptr;
    Surface *surface = nullptr;
};

/**
 * @brief center.axis of "a" is less than center.axis of "b"
 * @details 如果a和b的中心在axis上的投影相同, 返回false. 如果在axis上a的中心坐标比b小, 返回true. 其它情况返回false.
 * @param axis x=0, y=1, z=2. If not one of {0, 1, 2}, return false.
 */
bool aabb_a_lt_b_along_axis(const AABB &a, const AABB &b, int axis) {
    if (axis < 0 || axis > 2)
        return false;
    return (a.p0[axis] + a.p1[axis]) < (b.p0[axis] + b.p1[axis]);
}

/**
 * @brief 等价于aabb_a_lt_b_along_axis(a, b, 0)
 */
bool aabb_a_lt_b_along_x(const AABB &a, const AABB &b) { return aabb_a_lt_b_along_axis(a, b, 0); }

/**
 * @brief 等价于aabb_a_lt_b_along_axis(a, b, 1)
 */
bool aabb_a_lt_b_along_y(const AABB &a, const AABB &b) { return aabb_a_lt_b_along_axis(a, b, 1); }

/**
 * @brief 等价于aabb_a_lt_b_along_axis(a, b, 2)
 */
bool aabb_a_lt_b_along_z(const AABB &a, const AABB &b) { return aabb_a_lt_b_along_axis(a, b, 2); }

#endif // __BVH_H__