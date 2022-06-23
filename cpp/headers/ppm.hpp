#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <eigen3/Eigen/Eigen>
#include <cstdio>

void write_ppm(FILE *file, uint8_t *buffer, int width, int height);
#endif // __IMAGE_H__