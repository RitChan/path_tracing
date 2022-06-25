#include <mathutils.h>
#include <surface.hpp>

bool AABB::ray_hit(const Ray &ray, float t0, float t1, HitRecord *hit_record) const {
    float t_x0, t_x1, t_y0, t_y1, t_z0, t_z1;
    int num_solution = 0;
    t_x0 = solve_one_variable_linear(ray.d[0], p0[0] - ray.o[0], &num_solution);
    if (num_solution != 1)
        return false;
    t_x1 = solve_one_variable_linear(ray.d[0], p1[0] - ray.o[0], &num_solution);
    if (num_solution != 1)
        return false;
    t_y0 = solve_one_variable_linear(ray.d[1], p0[1] - ray.o[1], &num_solution);
    if (num_solution != 1)
        return false;
    t_y1 = solve_one_variable_linear(ray.d[1], p1[1] - ray.o[1], &num_solution);
    if (num_solution != 1)
        return false;
    t_z0 = solve_one_variable_linear(ray.d[2], p0[2] - ray.o[2], &num_solution);
    if (num_solution != 1)
        return false;
    t_z1 = solve_one_variable_linear(ray.d[2], p1[2] - ray.o[2], &num_solution);
    if (num_solution != 1)
        return false;

    float enter_x, exit_x, enter_y, exit_y, enter_z, exit_z;
    if (t_x0 < t_x1) {
        enter_x = t_x0;
        exit_x = t_x1;
    } else {
        enter_x = t_x1;
        exit_x = t_x0;
    }
    if (t_y0 < t_y1) {
        enter_y = t_y0;
        exit_y = t_y1;
    } else {
        enter_y = t_y1;
        exit_y = t_y0;
    }
    if (t_z0 < t_z1) {
        enter_z = t_z0;
        exit_z = t_z1;
    } else {
        enter_z = t_z1;
        exit_z = t_z0;
    }

    float max_enter = enter_x > enter_y ? (enter_x > enter_z ? enter_x : enter_z) : (enter_y > enter_z ? enter_y : enter_z);
    if (max_enter > t1)
        return false;
    float min_exit = enter_x < enter_y ? (enter_x < enter_z ? enter_x : enter_z) : (enter_y < enter_z ? enter_y : enter_z);
    if (min_exit < t0 || min_exit < max_enter)
        return false;
    return true;
}
