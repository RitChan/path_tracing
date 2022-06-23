#include <cstdio>
#include <eigen3/Eigen/Eigen>

#include "Ray.hpp"
#include "Surface.hpp"
#include "ppm.hpp"


int main() {
    printf("Hello World\n");
    printf("Hello Again\n");
    printf("Hello World!\n");
    FILE *f = fopen("out.ppm", "wb");
    uint8_t buffer[100 * 100 * 3];
    for (int i = 0; i < 100 * 100 * 3; i++) {
        buffer[i] = 0;
    }
    write_ppm(f, buffer, 100, 100);
    fclose(f);
    return 0;
}
