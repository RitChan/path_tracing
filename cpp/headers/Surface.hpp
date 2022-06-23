#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <Ray.hpp>

class Surface {
public:
  virtual bool ray_hit(const Ray &ray, HitRecord *hit_record) = 0;
};

#endif // __SURFACE_H__