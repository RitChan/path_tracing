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
 * @param m 待加载的模型
 * @param verts Vertices
 * @param faces [[v/vt/n, v/vt/n, ..., v/vt/n], [v/vt/n, v/vt/n, ..., v/vt/n], ...]; 索引从0开始; 如果vt, n不存在, 用-1代替
 * @return int 状态码: \n
 *  [0] succeeded \n
 *  [1] m == nullptr \n
 *  [2] verts == nullptr \n
 *  [3] faces == nullptr
 */
int load_data_to_model(Model *m, const vector<Vertex> *verts, const vector<vector<Eigen::Vector3i>> *faces);

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
            load_data_to_model(m, &verts, &faces);
            m = new Model();
            int name_len = line.length() - 2;
            char *name = new char[name_len + 1];
            memcpy(name, line.c_str() + 2, sizeof(char) * name_len);
            name[name_len] = '\0';
            m->name = name;
            add_submodel(model, m);
            faces.clear();
        }
    }
    if (!faces.empty()) {
        load_data_to_model(m, &verts, &faces);
    }
    int ret = calc_pairs(model, true);
    if (ret != 0) {
        return ret + 100;
    }
    model->name = "root";
    return 0;
}

int load_data_to_model(Model *m, const vector<Vertex> *verts, const vector<vector<Eigen::Vector3i>> *faces) {
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
            cur_hedge->v = m->verts + vid;
            cur_hedge->f = m->faces + i;
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
    vector<vector<HEdge *>> vid2edges(model->num_verts);
    for (int i = 0; i < model->num_faces; i++) {
        Face *fi = model->faces + i; // face[i]
        HEdge *ei = fi->h;           // edge of face[i]
        if (ei == nullptr) {
            continue;
        }
        do {
            vid2edges[ei->v->index].push_back(ei);
            vid2edges[ei->prev->v->index].push_back(ei);
            ei = ei->next;
            if (ei == nullptr) {
                return 2;
            }
        } while (ei != fi->h);
    }
    for (int i = 0; i < model->num_faces; i++) {
        // for every face[i]
        Face *fi = model->faces + i; // face[i]
        HEdge *ei = fi->h;           // edge of face[i]
        if (ei == nullptr || ei->v == nullptr || ei->v->index == -1) {
            continue;
        }
        do {
            // for every edge of face[i]
            for (auto ej : vid2edges[ei->v->index]) {
                int ret = match_pair(ei, ej);
                if (ret != 0 && ret != 3 && ret != 7) {
                    return 3;
                }
            }
            for (auto ej : vid2edges[ei->prev->v->index]) {
                int ret = match_pair(ei, ej);
                if (ret != 0 && ret != 3 && ret != 7) {
                    return 3;
                }
            }
            ei = ei->next;
            if (ei == nullptr) {
                continue;
            }
        } while (ei != fi->h);
    }
    if (recursive) {
        ModelList *model_list = model->submodels;
        int submodel_index = 0;
        while (model_list != nullptr) {
            int ret = calc_pairs(model_list->model, recursive);
            if (ret != 0) {
                return submodel_index;
            }
            submodel_index++;
            model_list = model_list->next;
        }
    }
    return 0;
}

int match_pair(HEdge *a, HEdge *b) {
    if (a == nullptr) {
        return 1;
    } else if (b == nullptr) {
        return 2;
    }
    if (a->f == nullptr || b->f == nullptr) {
        return 6;
    }
    if (a->f == b->f) {
        return 7;
    }
    for (int i = 0; i < a->num_paris; i++) {
        if (a->pairs[i] == b) {
            return 3;
        }
    }
    for (int i = 0; i < b->num_paris; i++) {
        if (b->pairs[i] == a) {
            return 3;
        }
    }
    if (a->v == nullptr || a->prev == nullptr || a->prev->v == nullptr || b->v == nullptr || b->prev == nullptr || b->prev->v == nullptr) {
        return 5;
    }
    if (a->v->index == -1 || b->prev->v->index == -1 || a->prev->v->index == -1 || b->v->index == -1) {
        return 4;
    }
    if ((a->v->index == b->prev->v->index && a->prev->v->index == b->v->index) ||
        (a->v->index == b->v->index && a->prev->v->index == b->prev->v->index)) {
        a->num_paris += 1;
        HEdge **pairs = new HEdge *[a->num_paris];
        memcpy(pairs, a->pairs, sizeof(HEdge *) * (a->num_paris - 1));
        delete[] a->pairs;
        a->pairs = pairs;
        a->pairs[a->num_paris - 1] = b;
        b->num_paris += 1;
        pairs = new HEdge *[b->num_paris];
        memcpy(pairs, b->pairs, sizeof(HEdge *) * (b->num_paris - 1));
        delete[] b->pairs;
        b->pairs = pairs;
        b->pairs[b->num_paris - 1] = a;
    }
    return 0;
}