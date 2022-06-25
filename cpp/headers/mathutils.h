#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

/**
 * @brief Solve Ax + B = 0
 * 
 * @param num_solution 
 *  One of {0, 1, 2}. 0 for zero solution, 1 for one solution, 2 for infinitely many solutions. 
 *  If num_solution if nullptr, not operation will be done on it.
 * 
 * @return (1) zero solution, NaN (2) one solution, the solution (3) infinitely many solutions, 0
 */
float solve_one_variable_linear(float A, float B, int *num_solution);


#endif // __MATHUTILS_H__