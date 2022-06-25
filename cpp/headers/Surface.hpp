#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <ray.hpp>

class AABB;

class Surface {
public:
    /**
     * @brief Ray与Surface求交
     *
     * @param ray 射线
     * @param t0 射线起点
     * @param t1 射线终点
     * @param hit_record 相交点数据（如果为nullptr，自动忽略）
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

class AABB : public Surface {
public:
    Eigen::Vector3f p0; // negative corner
    Eigen::Vector3f p1; // positive corner

    bool ray_hit(const Ray &ray, float t0, float t1, HitRecord *hit_record) const override;
    AABB aabb() const override { return *this; }
};

#endif // __SURFACE_H__