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

using namespace std;

int main() {
    FILE *file = fopen("C:\\Users\\chenh\\Desktop\\untitled5.obj", "r");
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
    char *obj_file = new char[file_len + 1];
    fseek(file, 0, 0);
    for (int i = 0; i < file_len; i++) {
        obj_file[i] = (char)fgetc(file);
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
        for (int i = 0; i < model_list->model->num_faces; i++) {
            Face *f = model_list->model->faces + i;
            HEdge *e = f->h;
            if (f->num_edges() > 0) {
                printf("%d 15 ", f->num_edges());
                do {
                    printf("%f %f %f ", e->v->co[0], e->v->co[1], e->v->co[2]);
                    e = e->prev;
                } while (e != nullptr && e != f->h);
            }
            printf("\n");
        }
        for (int i = 0; i < model_list->model->num_faces; i++) {
            Face *f = model_list->model->faces + i;
            HEdge *e = f->h;
            if (f->num_edges() > 0) {
                do {
                    if (e->num_paris > 0) {
                        printf("model(%s).face(%d).edge(%d), num pairs = %d\n", model_list->model->name, f->index, e->index, e->num_paris);
                        printf("2 4 ");
                        printf("%f %f %f ", e->v->co[0], e->v->co[1], e->v->co[2]);
                        printf("%f %f %f\n", e->prev->v->co[0], e->prev->v->co[1], e->prev->v->co[2]);
                    }
                    e = e->prev;
                } while (e != nullptr && e != f->h);
            }
        }
        model_list = model_list->next;
    }
    // printf("Num vertices = %d\n", vert_count);
    // printf("Num models = %d\n", model_count);
    return 0;
}
