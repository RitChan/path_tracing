/**
 * @file modeling.cpp
 * @brief 
 */
#include <modeling.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

int add_submodel(Model *model, Model *submodel) {
    if (model == nullptr) {
        return 1;
    } else if (submodel == nullptr) {
        return 2;
    }
    ModelList *model_list = new ModelList();
    model_list->model = submodel;
    if (model->submodels == nullptr) {
        model->submodels = model_list;
    } else {
        model_list->next = model->submodels;
        model->submodels = model_list;
    }
    return 0;
}

int parse_obj(const char *obj_file, Model *model) {
    if (obj_file == nullptr) {
        return 1;
    } else if (model == nullptr) {
        return 2;
    }
    vector<Vertex> verts; // vertices
    vector<Eigen::Vector2f> tex_coords; // texture coordinates
    vector<Eigen::Vector3f> normals;
    vector<vector<Eigen::Vector3i>> faces; // [[v/vt/n, v/vt/n, ..., v/vt/n], [v/vt/n, v/vt/n, ..., v/vt/n], ...], 如果vt, n不存在, 用-1代替
    stringstream ss(obj_file);
    string line;
    Model *m = model; // 当前正在解析的的object/group
    while (getline(ss, line, '\n')) {
        if (line.compare(0, 2, "v ", 2) == 0) {
            stringstream tmp_ss(line.c_str() + 2); // temporary stringstream
            size_t offset = 2;
            size_t parsed_len = 0; // parsed length
            Eigen::Vector4f vert_co = Eigen::Vector4f::Ones(); // vertex coordinates
            for (int i = 0; i < 4; i++) {
                try {
                    vert_co[i] = std::stof(line.c_str() + offset, &parsed_len);
                    offset += parsed_len;
                } catch (std::invalid_argument e) {
                    if (i == 3)
                        break;
                    return 3;
                } catch (std::out_of_range e) {
                    return 4;
                }
            }
            Eigen::Vector3f vert_co_div(vert_co[0] / vert_co[3], vert_co[1] / vert_co[3], vert_co[2] / vert_co[3]); // vertex coordinates divided (by w)
            if (std::isinf(vert_co_div[0]) || std::isinf(vert_co_div[1]) || std::isinf(vert_co_div[2]) || std::isnan(vert_co_div[0]) || std::isnan(vert_co_div[1]) ||
                std::isnan(vert_co_div[2])) {
                return 4;
            }
            Vertex v;
            v.co = vert_co_div;
            verts.push_back(v);
        } else if (line.compare(0, 3, "vt ", 3) == 0) {
            // printf("Found tex_coord: %s\n", line.c_str());
        } else if (line.compare(0, 2, "n ", 2) == 0) {
            // printf("Found normal: %s\n", line.c_str());
        } else if (line.compare(0, 2, "f ", 2) == 0) {
            // printf("Found face: %s\n", line.c_str());
            stringstream line_ss(line.c_str() + 2); // line stringstream
            string word;
            vector<Eigen::Vector3i> face;
            int num_faces_parsed; // number of parsed faces
            while (getline(line_ss, word, ' ')) {
                stringstream word_ss(word.c_str()); // word stringstream
                string idx_string; // index string of vertex, texture coordinate, normal
                int num_prop_parsed = 0; // number of properties parsed for current vertex
                Eigen::Vector3i vertex_prop = Eigen::Vector3i(-1, -1, -1); // vertex properties
                while (getline(word_ss, idx_string, '/')) {
                    if (num_prop_parsed >= 3) {
                        // stop parsing if 3 properties has already been parsed
                        return 5;
                    }
                    try {
                        if (!idx_string.empty()) {
                            vertex_prop[num_prop_parsed] = std::atoi(idx_string.c_str()) - 1;
                        }
                    } catch (std::invalid_argument e) {
                        return 6;
                    } catch (std::out_of_range e) {
                        return 7;
                    }
                    num_prop_parsed += 1;
                }
                if (num_prop_parsed < 1) {
                    return 8;
                }
                num_faces_parsed += 1;
                face.push_back(vertex_prop);
            }
            if (num_faces_parsed > 0) {
                faces.push_back(std::move(face));
            }
        } else if (line.compare(0, 2, "o ", 2) == 0 || line.compare(0, 2, "g ", 2) == 0) {
            m->num_verts = verts.size();
            m->num_faces = faces.size();
            if (m->num_verts > 0) {
                m->verts = new Vertex[m->num_verts];
                memcpy(m->verts, verts.data(), sizeof(Vertex) * m->num_verts);
            }
            if (m->num_faces > 0) {
                m->faces = new Face[m->num_faces];
            }
            for (int i = 0; i < m->num_faces; i++) {
                // i: index of faces
                HEdge *cur_hedge = nullptr; // current HEdge
                for (int j = 0; j < faces[i].size(); j++) {
                    // j: index of face[i] vertices
                    if (cur_hedge == nullptr) {
                        cur_hedge = new HEdge();
                        m->faces[i].h = cur_hedge;
                    } else {
                        cur_hedge->next = new HEdge();
                        cur_hedge = cur_hedge->next;
                    }
                    int vid = faces[i][j][0]; // Vertex index
                    cur_hedge->v = &m->verts[vid];
                    m->verts[vid].h = cur_hedge;
                }
                if (cur_hedge != nullptr) {
                    cur_hedge->next = m->faces[i].h;
                }
            }
            Model *submodel = new Model();
            add_submodel(model, submodel);
            m = submodel;
            faces.clear();
            printf("Add submodel\n");
        }
    }
    return 0;
}