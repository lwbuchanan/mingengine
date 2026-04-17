#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ming_print.h"
#include "renderer.h"

void parse_obj_line(char *line, Model *m) {
  char *saveptr;
  char *symbol = strtok_r(line, " \t", &saveptr);

  switch (symbol[0]) {
    char *token;
    char *token2;
    char *saveptr2;
  case 'v': { // Vertex
    switch (symbol[1]) {
    case '\0': { // Vertex
      token = strtok_r(NULL, " \t", &saveptr);
      float f1 = strtof(token, NULL);
      token = strtok_r(NULL, " \t", &saveptr);
      float f2 = strtof(token, NULL);
      token = strtok_r(NULL, " \t", &saveptr);
      float f3 = strtof(token, NULL);
      m->vertices[m->n_vertices++] = v3f{f1, f2, f3};
    } break;
    case 't': { // Vertex texture
    } break;
    case 'n': { // Vertex normal
    } break;
    }
  } break;

  case 'f': { // Face
    int n_verts = 0;
    v3i face_verts = {};
    while ((token = strtok_r(NULL, " \t", &saveptr)) != NULL) {
      token2 = strtok_r(token, "/", &saveptr2);
      face_verts.data[n_verts++] = atoi(token2) - 1;

      // token2 = strtok_r(NULL, "/", &saveptr2);
      // if (token2 != NULL) {
      //   da_i32_append(m->face_vnormal_idxs, atoi(token2));
      //   token2 = strtok_r(NULL, "/", &saveptr2);
      //   if (token2 != NULL) {
      //     da_i32_append(m->face_vtexture_idxs, atoi(token2));
      //   }
      // }

      assert(n_verts <= 3);
      if (n_verts >= 3) {
        m->faces[m->n_faces++] = face_verts;
      }
    }
  } break;

  case '#': {
    // Comment
  } break;
  }
}

Model load_model(const char *obj_file, v3f *vertex_buf, usize max_vertices,
                 v3i *face_buf, usize max_faces) {
  Model model = {
      vertex_buf,
      face_buf,
      0,
      0,
  };

  FILE *fd = fopen(obj_file, "rb");
  if (fd == NULL) {
    bail("Failed to open file: %s", obj_file);
  }

  char line[64];
  while (fgets(line, 64, fd)) {
    parse_obj_line(line, &model);
    assert(model.n_faces <= max_faces);
    assert(model.n_vertices <= max_vertices);
  }

  fclose(fd);

  return model;
}
