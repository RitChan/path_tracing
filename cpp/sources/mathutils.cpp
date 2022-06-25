#include <mathutils.h>
#include <cmath>

float solve_one_variable_linear(float A, float B, int *num_solution) {
    if (A == 0 && B == 0) {
        if (num_solution != nullptr)
            *num_solution = 2;
        return 0;
    }
    if (A == 0) {
        if (num_solution != nullptr)
            *num_solution = 0;
        return NAN;
    }
    return -B / A;
}
