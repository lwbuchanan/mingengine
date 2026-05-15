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
      float64 f1 = strtod(token, NULL);
      token = strtok_r(NULL, " \t", &saveptr);
      float64 f2 = strtod(token, NULL);
      token = strtok_r(NULL, " \t", &saveptr);
      float64 f3 = strtod(token, NULL);
      m->vertices[m->n_vertices++] = v4f{f1, f2, f3, 1};
    } break;
    case 't': { // Vertex texture
    } break;
    case 'n': { // Vertex normal
    } break;
    }
  } break;

  case 'f': { // Face
    int n_verts = 0;
    int v1 = 0, v2 = 0, v3 = 0;
    while ((token = strtok_r(NULL, " \t", &saveptr))) {
      token2 = strtok_r(token, "/", &saveptr2);
      n_verts++;

      if (n_verts == 1) {
        v1 = atoi(token2) - 1;
      } else if (n_verts == 2) {
        v2 = atoi(token2) - 1;
      } else if (n_verts == 3) {
        v3 = atoi(token2) - 1;
        m->faces[m->n_faces++] = {v1, v2, v3};
      } else if (n_verts > 3) {
        v2 = v3;
        v3 = atoi(token2) - 1;
        m->faces[m->n_faces++] = {v1, v2, v3};
      }
    }
  } break;

  case '#': {
    // Comment
  } break;
  }
}

Model load_model(const char *obj_file, v4f *vertex_buf, usize max_vertices,
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
