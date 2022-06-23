#ifndef __RAY_H__
#define __RAY_H__

#include <eigen3/Eigen/Eigen>

class Ray {
public:
  Eigen::Vector3f o;
  Eigen::Vector3f d;

  Eigen::Vector3f at(float t) { return o + t * d; }
};

class HitRecord {
public:
  bool hit;
  float t;

  HitRecord(bool hit, float t) : hit(hit), t(t) {}
};

#endif // __RAY_H__