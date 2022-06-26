/**
 * @file ppm.h
 * @brief 实现ppm图片的写入
 */
#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <eigen3/Eigen/Eigen>
#include <cstdio>

/**
 * @param file (Not Free)
 * @param buffer (Not Free)
 */
void write_ppm(FILE *file, uint8_t *buffer, int width, int height);
#endif // __IMAGE_H__