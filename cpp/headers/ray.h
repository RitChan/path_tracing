/**
 * @file Ray.h
 * @brief 实现Ray
 */
#ifndef __RAY_H__
#define __RAY_H__

#include <cmath>
#include <eigen3/Eigen/Eigen>

/**
 * @brief (No Pointer)
 */
class Ray {
public:
    Eigen::Vector3f o = Eigen::Vector3f::Zero(); // origin
    Eigen::Vector3f d = Eigen::Vector3f::Zero(); // direction

    Ray(const Eigen::Vector3f &o, const Eigen::Vector3f &d) : o(o), d(d) {}
    Ray(Eigen::Vector3f &&o, Eigen::Vector3f &&d) : o(o), d(d) {}

    Eigen::Vector3f at(float t) { return o + t * d; }
};

#endif // __RAY_H__