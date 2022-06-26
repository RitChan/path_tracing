/**
 * @file surface.h
 * @brief 实现各种各样的Surface
 */
#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <ray.h>
#include <hitrecord.h>

class AABB;

/**
 * @brief (Has Pointer)
 */
class Surface {
public:
    /**
     * @brief Ray与Surface求交
     *
     * @param ray 射线
     * @param t0 射线起点
     * @param t1 射线终点
     * @param hit_record 相交点数据 (如果为nullptr, 自动忽略)
     * @return true 相交
     * @return false 不相交
     */
    virtual bool ray_hit(const Ray &ray, float t0, float t1, HitRecord *hit_record) const = 0;

    /**
     * @brief 获得该Surface的AABB
     *
     * @return AABB 该Surface的AABB
     */
    virtual AABB aabb() const = 0;
};

/**
 * @brief (No Pointer)
 */
class AABB : public Surface {
public:
    Eigen::Vector3f p0; // point0, negative corner
    Eigen::Vector3f p1; // point1, positive corner

    /**
     * @brief Construct a new AABB object. Automatically fix inversed inequality.
     */
    AABB(float x_low, float x_high, float y_low, float y_high, float z_low, float z_high);

    /**
     * @param hit_record (Not Free)
     */
    bool ray_hit(const Ray &ray, float t0, float t1, HitRecord *hit_record) const override;

    AABB aabb() const override { return *this; }

    float volumn() {
        auto v = p1 - p0;
        return v[0] * v[1] * v[2];
    }
};

#endif // __SURFACE_H__