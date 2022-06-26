/**
 * @file main.cpp
 * @brief 用于调试代码
 */
#include <cstdio>
#include <eigen3/Eigen/Eigen>
#include <modeling.h>
#include <ppm.h>
#include <ray.h>
#include <stdexcept>
#include <string>
#include <surface.h>

int main() {
    FILE *file = fopen("C:\\Users\\chenh\\Desktop\\untitled.obj", "r");
    if (file == nullptr) {
        printf("Cannot open file\n");
        return 2;
    }
    int error_code = fseek(file, 0, SEEK_END);
    if (error_code != 0) {
        printf("Error, code = %d\n", error_code);
        return 1;
    }
    long file_len = ftell(file);
    char *obj_file = new char[file_len+1];
    fseek(file, 0, 0);
    for (int i = 0; i < file_len; i++) {
        obj_file[i] = (char) fgetc(file);
    }
    obj_file[file_len] = '\0';
    Model model = Model();
    error_code = parse_obj(obj_file, &model);
    if (error_code != 0) {
        printf("Error: %d\n", error_code);
        return error_code;
    }
    ModelList *model_list = model.submodels;
    int model_count = 0;
    int vert_count = 0;
    while (model_list != nullptr) {
        model_count += 1;
        if (model_list->model != nullptr) {
            vert_count += model_list->model->num_verts;
        }
        model_list = model_list->next;
    }
    printf("Num vertices = %d\n", model.num_verts);
    printf("Num models = %d\n", model_count);
    return 0;
}
