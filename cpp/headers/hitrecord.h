/**
 * @file hitrecord.h
 * @brief 实现HitRecord
 */
#ifndef __HITRECORD_H__
#define __HITRECORD_H__

/**
 * @brief (Has Pointer)
 */
class HitRecord {
public:
    bool hit;
    float t;

    HitRecord(bool hit, float t) : hit(hit), t(t) {}
};

#endif // __HITRECORD_H__