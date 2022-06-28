/**
 * @file modeling.cpp
 * @brief
 */
// #include <algorithm>
#include <modeling.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/**
 * @brief 从verts和faces搜集Model数据, 加载至m
 *
 * @param m 搜集的数据的容器
 * @param verts Vertices
 * @param faces [[v/vt/n, v/vt/n, ..., v/vt/n], [v/vt/n, v/vt/n, ..., v/vt/n], ...]; 索引从0开始; 如果vt, n不存在, 用-1代替
 * @return int 状态码: \n
 *  [0] succeeded \n
 *  [1] m == nullptr \n
 *  [2] verts == nullptr \n
 *  [3] faces == nullptr
 */
int collect_model_data(Model *m, const vector<Vertex> *verts, const vector<vector<Eigen::Vector3i>> *faces);

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
    vector<Vertex> verts;               // vertices
    vector<Eigen::Vector2f> tex_coords; // texture coordinates
    vector<Eigen::Vector3f> normals;
    // [[v/vt/n, v/vt/n, ..., v/vt/n], [v/vt/n, v/vt/n, ..., v/vt/n], ...]; 索引从0开始; 如果vt, n不存在, 用-1代替
    vector<vector<Eigen::Vector3i>> faces;
    stringstream ss(obj_file);
    string line;
    Model *m = model; // 当前正在解析的的object/group
    while (getline(ss, line, '\n')) {
        if (line.compare(0, 2, "v ", 2) == 0) {
            stringstream tmp_ss(line.c_str() + 2); // temporary stringstream
            size_t offset = 2;
            size_t parsed_len = 0;                             // parsed length
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
            Eigen::Vector3f vert_co_div(vert_co[0] / vert_co[3], vert_co[1] / vert_co[3],
                                        vert_co[2] / vert_co[3]); // vertex coordinates divided (by w)
            if (std::isinf(vert_co_div[0]) || std::isinf(vert_co_div[1]) || std::isinf(vert_co_div[2]) || std::isnan(vert_co_div[0]) ||
                std::isnan(vert_co_div[1]) || std::isnan(vert_co_div[2])) {
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
            int num_verts_parsed = 0; // number of parsed vertices
            while (getline(line_ss, word, ' ')) {
                stringstream word_ss(word.c_str());                        // word stringstream
                string idx_string;                                         // index string of vertex, texture coordinate, normal
                int num_prop_parsed = 0;                                   // number of properties parsed for current vertex
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
                num_verts_parsed += 1;
                face.push_back(vertex_prop);
            }
            if (num_verts_parsed >= 3) {
                faces.push_back(std::move(face));
            } else {
                return 10;
            }
        } else if (line.compare(0, 2, "o ", 2) == 0 || line.compare(0, 2, "g ", 2) == 0) {
            collect_model_data(m, &verts, &faces);
            m = new Model();
            add_submodel(model, m);
            faces.clear();
        }
    }
    if (!faces.empty()) {
        collect_model_data(m, &verts, &faces);
    }
    int ret = calc_pairs(model, true);
    if (ret != 0) {
        return ret + 100;
    }
    return 0;
}

int collect_model_data(Model *m, const vector<Vertex> *verts, const vector<vector<Eigen::Vector3i>> *faces) {
    if (m == nullptr) {
        return 1;
    } else if (verts == nullptr) {
        return 2;
    } else if (faces == nullptr) {
        return 3;
    }
    // 这里将全局的vertices映射到当前object所需的vertices, 减少存储空间
    unordered_map<int, int> old2new; // old index to new index
    for (int i = 0; i < faces->size(); i++) {
        for (int j = 0; j < (*faces)[i].size(); j++) {
            if (old2new.find((*faces)[i][j][0]) == old2new.end()) {
                // not already seen
                old2new.insert({(*faces)[i][j][0], old2new.size()});
            }
        }
    }
    m->num_verts = old2new.size();
    m->num_faces = faces->size();
    if (m->num_verts > 0) {
        m->verts = new Vertex[m->num_verts];
        for (auto p = old2new.begin(); p != old2new.end(); p++) {
            m->verts[p->second] = (*verts)[p->first];
            m->verts[p->second].index = p->second;
        }
    }
    if (m->num_faces > 0) {
        m->faces = new Face[m->num_faces];
    }
    for (int i = 0; i < m->num_faces; i++) {
        // for every face
        m->faces[i].index = i;
        HEdge *cur_hedge = nullptr; // current HEdge
        int hedge_index = 0;
        for (int j = 0; j < (*faces)[i].size(); j++) {
            // for every vertex
            if (cur_hedge == nullptr) {
                cur_hedge = new HEdge();
                m->faces[i].h = cur_hedge;
            } else {
                cur_hedge->next = new HEdge();
                cur_hedge->next->prev = cur_hedge;
                cur_hedge = cur_hedge->next;
            }
            cur_hedge->index = hedge_index++;
            int vid = old2new.find((*faces)[i][j][0])->second; // (New) vertex index
            cur_hedge->v = &m->verts[vid];
        }
        if (cur_hedge != nullptr) {
            cur_hedge->next = m->faces[i].h;
            cur_hedge->next->prev = cur_hedge;
        }
    }
    return 0;
}

int calc_pairs(Model *model, bool recursive) {
    if (model == nullptr) {
        return 1;
    }
    for (int i = 0; i < model->num_faces; i++) {
        // for every face[i]
        for (int j = i + 1; j < model->num_faces; j++) {
            // for every face[j]
            Face *fi = model->faces + i; // face[i]
            Face *fj = model->faces + j; // face[j]
            HEdge *ej = fj->h;           // edge of face[j]
            HEdge *ei = fi->h;           // edge of face[i]
            if (ei == nullptr || ej == nullptr) {
                continue;
            }
            do {
                // for every edge of face[i]
                do {
                    // for every edge of face[j]
                    if (ei->v->index != -1 && ej->prev->v->index != -1 && ei->v->index == ej->prev->v->index && ei->prev->v->index != -1 &&
                        ej->v->index != -1 && ei->prev->v->index == ej->v->index) {
                        ei->num_paris += 1;
                        HEdge **pairs = new HEdge *[ei->num_paris];
                        memcpy(pairs, ei->pairs, sizeof(HEdge *) * (ei->num_paris - 1));
                        delete[] ei->pairs;
                        ei->pairs = pairs;
                        ej->num_paris += 1;
                        pairs = new HEdge *[ej->num_paris];
                        memcpy(pairs, ej->pairs, sizeof(HEdge *) * (ej->num_paris - 1));
                        delete[] ej->pairs;
                        ej->pairs = pairs;
                    }
                    ej = ej->next;
                } while (ej != fj->h);
                ei = ei->next;
            } while (ei != fi->h);
        }
    }
    if (recursive) {
        ModelList *model_list = model->submodels;
        while (model_list != nullptr) {
            calc_pairs(model_list->model, recursive);
            model_list = model_list->next;
        }
    }
    return 0;
}