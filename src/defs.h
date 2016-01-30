#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"

#define PI 3.14159
#define RAND_BETWEEN(a, b) ((b-a)*rand()/RAND_MAX + a)
#define DEG_TO_RAD(x) (x*0.0174532925)
#define RAD_TO_DEG(x) (x/0.0174532925)

typedef struct
{
  int num_indices;
  unsigned int* indices;
  
  int num_v;
  int num_vt;
  int num_vn;
  int num_vp;
  
  float* v;
  float* vt;
  float* vn;
  float* vp;
} s_model;

typedef struct
{
  int width;
  int height;
  unsigned char *data;
} s_texture;

typedef struct
{
  char name[64];
  GLfloat ka;
  GLfloat kd;
  GLfloat ks;
} s_material;

typedef struct
{
  float pitch; // Up/Down
  float yaw;   // Left/Right
  float pos[3];
} s_camera;

typedef struct
{
  GLuint num_vertices;
  GLuint num_instances;
  GLuint num_indices;
  // Handles
  GLuint vao;
  GLuint vbo, vtbo, vnbo, mmbo, ibo;
} s_object;

typedef struct
{
  float x;
  float y;
  float z;
  float r_x;
  float r_y;
  float r_z;
  float v_x;
  float v_y;
  float v_z;
} s_box, s_teapot;

typedef struct
{
  float width;
  float height;
  
  int num_objects;
  s_object objects[64];
  
  int num_models;
  s_model models[64];
  
  int num_boxes;
  s_box* boxes;
  
  int num_teapots;
  s_teapot* teapots;
} s_level;

typedef struct
{
  // Entities
  int num_objects;
  s_object* objects;
  // Models
  int num_models;
  s_model* models;
  // Textures
  int num_textures;
  s_texture *textures;
} s_rendering;

s_camera camera;
int window_width;
int window_height;
int window_fullscreen;
int max_fps;

// level.c
int simulate(s_level* level);
int level_add_box(s_level* level, float x, float y, float z, float r_x, float r_y, float r_z);
int level_add_teapot(s_level* level, float x, float y, float z, float r_x, float r_y, float r_z);

// object.c
int create_object(s_object* object, s_model* model);

// io.c
int load_settings(const char *filename);
char* load_shader(const char *filename);
int print_log(const char *format, ...);
void print_log_shader_info(GLuint shader_index);
int load_bmp(s_texture* texture, const char* filename);
int load_landscape(s_model* model, const char *path);

// level.c
int load_level(s_level* level, const char *path);

// models.c
int load_obj(s_model* model, const char* path);

// callbacks.c
void glfw_window_size_callback(GLFWwindow* window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// other.c
void update_fps_counter(GLFWwindow* window);

#endif // DEFS_H_INCLUDED
