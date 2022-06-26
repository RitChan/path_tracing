/**
 * @file modeling.h
 * @brief Implements half edge data structure and format converter
 */
#ifndef __MODELING_H__
#define __MODELING_H__
#include <cstdint>
#include <eigen3/Eigen/Eigen>

class Vertex;

/**
 * @brief (Has Pointer) Half edge. Each edge is local to some Face.
 * 
 */
class HEdge {
public:
    HEdge *pair = nullptr; // Half edge of neighbouring face.
    HEdge *next = nullptr; // Next half edge of the face in counter-clockwise order.
    Vertex *v = nullptr; // Common vertex of this and the next HEdge.
};

/**
 * @brief (Has Pointer)
 */
class Face {
public:
    HEdge *h = nullptr; // Any half edge of this face.
};

/**
 * @brief (Has Pointer)
 */
class Vertex {
public:
    Eigen::Vector3f co;
    HEdge *h = nullptr; // Any half edge connecting to this vertex.
};

class Model;

/**
 * @brief (Has Pointer) A linked list of submodels
 */
class ModelList {
public:
    ModelList *next = nullptr;
    Model *model = nullptr;
    Eigen::Matrix3f rotation = Eigen::Matrix3f::Identity();
    Eigen::Vector3f translation = Eigen::Vector3f::Zero();
};

/**
 * @brief (Has Pointer)
 */
class Model {
public:
    Vertex *verts = nullptr; // vertices
    Face *faces = nullptr;
    uint32_t num_verts = 0; // number of vertices
    uint32_t num_faces = 0; // number of faces
    ModelList *submodels = nullptr;
};

/**
 * @brief 将submodel添加到model->submodels的头部
 * 
 * @param model (Not Free)
 * @param submodel (Not Free)
 * @return error code
 *  [0] succeeded
 *  [1] model是nullptr
 *  [2] submodel是nullptr
 */
int add_submodel(Model *model, Model *submodel);

/**
 * @brief 将obj文件解析为Model, 仅支持v, vt, n, f, o, g六种命令
 * @details Specifications:
 *  (1) 其它不以"v ", "vt ", "n ", "f ", "o ", "g "开头的行会被忽略
 *  (2) model是obj文件中的根节点, obj中不属于任一group/object的点线面将直接加入model, obj中定义的group/object会加入model的submodels
 * @param obj_file (Not Free) Wavefront obj file. Should end with null character. Otherwise the program will crash.
 * @param model (Not Free) Objects的根节点
 * @return 错误码:
 *  [0] succeeded
 *  [1] obj_file是nullptr
 *  [2] model是nullptr
 *  [3] 解析顶点坐标失败: 一行的顶点数量不足
 *  [4] 解析顶点坐标失败: 顶点坐标超过IEEE745浮点数范围
 */
int parse_obj(const char *obj_file, Model *model);

#endif // __MODELING_H__