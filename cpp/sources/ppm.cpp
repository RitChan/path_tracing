/**
 * @file ppm.cpp
 * @brief ppm.h的具体实现
 */
#include <ppm.h>

void write_ppm(FILE *file, uint8_t *buffer, int width, int height) {
  fprintf(file, "P6\n%i %i 255\n", width, height);
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      int base = j * width * 3 + i * 3;
      fputc(buffer[base], file);
      fputc(buffer[base + 1], file);
      fputc(buffer[base + 2], file);
    }
  }
  fputc(-1, file);
}
