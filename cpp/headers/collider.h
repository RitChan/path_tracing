/**
 * @file collider.h
 * @brief 实现碰撞体的碰撞检测函数以及碰撞体相关的几何操作
 */
#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <surface.h>

/**
 * @brief 返回同时包围a和b的最小AABB
 */
AABB aabb_merge(const AABB &a, const AABB &b);

/**
 * @brief 如果三条分别与x, y, z轴平行的线能够将a和b分开, 则返回false, 否则返回true.
 */
bool aabb_collide(const AABB &a, const AABB &b);


#endif // __COLLIDER_H__