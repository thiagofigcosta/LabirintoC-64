#ifndef NTLOADER_H
#define NTLOADER_H
#include "../Classes/Util.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "tinyLoader.h"

typedef struct {
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texcoords;
  std::vector<int> v_indices;
  std::vector<int> vn_indices;
  std::vector<int> vt_indices;

  std::vector<tinyobj::material_t> materials;

} nTMesh;

typedef struct {
  tinyobj::attrib_t attrib;
  vector<tinyobj::shape_t> shapes;
  vector<tinyobj::material_t> materials;
} nTShape;

inline void vertex_cb(void *user_data, float x, float y, float z, float w) {
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);
  //printf("v[%ld] = %f, %f, %f (w %f)\n", mesh->vertices.size() / 3, x, y, z, w);

  mesh->vertices.push_back(x);
  mesh->vertices.push_back(y);
  mesh->vertices.push_back(z);
  // Discard w
}

inline void normal_cb(void *user_data, float x, float y, float z) {
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);
  //printf("vn[%ld] = %f, %f, %f\n", mesh->normals.size() / 3, x, y, z);

  mesh->normals.push_back(x);
  mesh->normals.push_back(y);
  mesh->normals.push_back(z);
}

inline void texcoord_cb(void *user_data, float x, float y, float z) {
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);
  //printf("vt[%ld] = %f, %f, %f\n", mesh->texcoords.size() / 3, x, y, z);

  mesh->texcoords.push_back(x);
  mesh->texcoords.push_back(y);
  mesh->texcoords.push_back(z);
}

inline void index_cb(void *user_data, tinyobj::index_t *indices, int num_indices) {
  // NOTE: the value of each index is raw value.
  // For example, the application must manually adjust the index with offset
  // (e.g. v_indices.size()) when the value is negative(whic means relative
  // index).
  // Also, the first index starts with 1, not 0.
  // See fixIndex() function in tiny_obj_loader.h for details.
  // Also, 0 is set for the index value which
  // does not exist in .obj
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);

  for (int i = 0; i < num_indices; i++) {
    tinyobj::index_t idx = indices[i];
    //printf("idx[%ld] = %d, %d, %d\n", mesh->v_indices.size(), idx.vertex_index,idx.normal_index, idx.texcoord_index);

    if (idx.vertex_index != 0) {
      mesh->v_indices.push_back(idx.vertex_index);
    }
    if (idx.normal_index != 0) {
      mesh->vn_indices.push_back(idx.normal_index);
    }
    if (idx.texcoord_index != 0) {
      mesh->vt_indices.push_back(idx.texcoord_index);
    }
  }
}

inline void usemtl_cb(void *user_data, const char *name, int material_idx) {
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);
  if ((material_idx > -1) && (material_idx < mesh->materials.size())) {
    //printf("usemtl. material id = %d(name = %s)\n", material_idx,           mesh->materials[material_idx].name.c_str());
  } else {
    //printf("usemtl. name = %s\n", name);
  }
}

inline void mtllib_cb(void *user_data, const tinyobj::material_t *materials,
               int num_materials) {
  nTMesh *mesh = reinterpret_cast<nTMesh *>(user_data);
  //printf("mtllib. # of materials = %d\n", num_materials);

  for (int i = 0; i < num_materials; i++) {
    mesh->materials.push_back(materials[i]);
  }
}

inline void group_cb(void *user_data, const char **names, int num_names) {
  // nTMesh *mesh = reinterpret_cast<nTMesh*>(user_data);
  //printf("group : name = \n");

  //for (int i = 0; i < num_names; i++) {
  //  printf("  %s\n", names[i]);
  //}
}

inline void object_cb(void *user_data, const char *name) {
  // nTMesh *mesh = reinterpret_cast<nTMesh*>(user_data);
  //printf("object : name = %s\n", name);
}
#endif /* NTLOADER_H */

