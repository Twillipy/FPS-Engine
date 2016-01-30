#include "defs.h"

int prepare_buffers(s_level* level)
{
  assert(level != NULL);
  assert(level->objects != NULL);

  /*
  int i;
  for(i = 0; i < level->num_objects; ++i)
  {
    float* matrices = malloc(16 * level->num_boxes * sizeof *matrices);
    if(matrices == NULL) {return -3;}
  
    s_mat4 translation = translation_xyz_mat4(level->boxes[i].x, level->boxes[i].y, level->boxes[i].z);
    s_mat4 rotation = rotation_x_mat4(DEG_TO_RAD(level->boxes[i].r_x));
    rotation = multiply_mat4(rotation, rotation_y_mat4(DEG_TO_RAD(level->boxes[i].r_y)));
    rotation = multiply_mat4(rotation, rotation_z_mat4(DEG_TO_RAD(level->boxes[i].r_z)));
    
    s_mat4 mat = multiply_mat4(rotation, translation);
    
    memcpy(&matrices[16*i], mat.m, 16*sizeof(float));
  }
  
  return 0;
  */
  
  int i;
  
  float* matrices = malloc(16 * level->num_boxes * sizeof *matrices);
  if(matrices == NULL) {return -3;}
  
  float* matrices2 = malloc(16 * level->num_teapots * sizeof *matrices2);
  if(matrices2 == NULL) {return -4;}
  
  float* matrices3 = malloc(16 * sizeof *matrices3);
  if(matrices2 == NULL) {return -5;}
  
  // Boxes
  for(i = 0; i < level->num_boxes; ++i)
  {
    s_mat4 translation = translation_xyz_mat4(level->boxes[i].x, level->boxes[i].y, level->boxes[i].z);
    s_mat4 rotation = rotation_x_mat4(DEG_TO_RAD(level->boxes[i].r_x));
    rotation = multiply_mat4(rotation, rotation_y_mat4(DEG_TO_RAD(level->boxes[i].r_y)));
    rotation = multiply_mat4(rotation, rotation_z_mat4(DEG_TO_RAD(level->boxes[i].r_z)));
    
    s_mat4 mat = multiply_mat4(rotation, translation);
    
    memcpy(&matrices[16*i], mat.m, 16*sizeof(float));
  }
  level->objects[0].num_instances = level->num_boxes;
  glBindBuffer(GL_ARRAY_BUFFER, level->objects[0].mmbo);
  glBufferData(GL_ARRAY_BUFFER, 16*(level->num_boxes)*sizeof *matrices, matrices, GL_STATIC_DRAW);

  // Teapots
  for(i = 0; i < level->num_teapots; ++i)
  {
    s_mat4 translation = translation_xyz_mat4(level->teapots[i].x, level->teapots[i].y, level->teapots[i].z);
    s_mat4 rotation = rotation_x_mat4(DEG_TO_RAD(level->teapots[i].r_x));
    rotation = multiply_mat4(rotation, rotation_y_mat4(DEG_TO_RAD(level->teapots[i].r_y)));
    rotation = multiply_mat4(rotation, rotation_z_mat4(DEG_TO_RAD(level->teapots[i].r_z)));

    s_mat4 mat = scale_xyz_mat4(25.0, 25.0, 25.0);
    //s_mat4 mat = scale_xyz_mat4(0.25, 0.25, 0.25);
    mat = multiply_mat4(mat, rotation);
    mat = multiply_mat4(mat, translation);

    memcpy(&matrices2[16*i], mat.m, 16*sizeof(float));
  }
  level->objects[1].num_instances = level->num_teapots;
  glBindBuffer(GL_ARRAY_BUFFER, level->objects[1].mmbo);
  glBufferData(GL_ARRAY_BUFFER, 16*(level->num_teapots)*sizeof *matrices2, matrices2, GL_STATIC_DRAW);
  
  // Map
  s_mat4 mat = scale_xyz_mat4(300.0, 300.0, 300.0); //identity_matrix();
  memcpy(&matrices3[0], mat.m, 16*sizeof(float));
  level->objects[2].num_instances = 1;
  glBindBuffer(GL_ARRAY_BUFFER, level->objects[2].mmbo);
  glBufferData(GL_ARRAY_BUFFER, 16*(1)*sizeof *matrices3, matrices3, GL_STATIC_DRAW);
  
  // Heightmap
  mat = identity_matrix();
  memcpy(&matrices3[0], mat.m, 16*sizeof(float));
  level->objects[3].num_instances = 1;
  glBindBuffer(GL_ARRAY_BUFFER, level->objects[3].mmbo);
  glBufferData(GL_ARRAY_BUFFER, 16*(1)*sizeof *matrices3, matrices3, GL_STATIC_DRAW);

  free(matrices);
  free(matrices2);
  free(matrices3);
  
  return 0;
}

int main()
{
  int r;
  GLenum err;
  
  #ifndef NDEBUG
  printf("DEBUG BUILD\n");
  printf("%s %s\n\n", __DATE__, __TIME__);
  getchar();
  #endif
  
  err = GL_NO_ERROR;
  r = load_settings("settings.ini");
  if(r != 0)
  {
    print_log("ERROR: Failed to load settings.ini\n");
    // Default values
    window_width = 640;
    window_height = 480;
    window_fullscreen = 0;
    max_fps = 60;
  }

  // Start GL context and O/S window using the GLFW helper library
  if(!glfwInit())
  {
    print_log("ERROR: could not start GLFW3\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(window_width, window_height, "OpenGL Test", NULL, NULL);
  if(!window)
  {
    print_log("ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, glfw_window_size_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
  glfwSetScrollCallback(window, glfw_mouse_scroll_callback);
  glfwSetKeyCallback(window, glfw_keyboard_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSwapInterval(1);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  r = glewInit();
  while((err = glGetError()) != GL_NO_ERROR)
  {
    print_log("ERROR: glewInit() (%i)\n", err);
  }
  
  // get version info
  #ifndef NDEBUG
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);
  printf("\n");
  #endif
  
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  s_texture texture_atlas;
  r = load_bmp(&texture_atlas, "textures//main.bmp");
  if(r != 0) {print_log("ERROR: Failed to load textures//main.bmp\n");}

  s_texture texture_skybox;
  r = load_bmp(&texture_skybox, "textures//skybox.bmp");
  if(r != 0) {print_log("ERROR: Failed to load textures//skybox.bmp\n");}

  s_texture texture_terrain;
  r = load_bmp(&texture_terrain, "models//landscapes//map_001_texture.bmp");
  if(r != 0) {print_log("ERROR: Failed to load models//landscapes//map_001_texture.bmp\n");}

  // Make the terrain texture green
  int i;
  for(i = 0; i < texture_terrain.width*texture_terrain.height; ++i)
  {
    /*
    texture_terrain.data[3*i+0] = 100;
    texture_terrain.data[3*i+1] = 100;
    texture_terrain.data[3*i+2] = 100;
    continue;
    */
    
    texture_terrain.data[3*i+0] = 10;
    texture_terrain.data[3*i+2] = 10;
    
         if(texture_terrain.data[3*i+1] < 50)  {texture_terrain.data[3*i+1] = 50;}
    else if(texture_terrain.data[3*i+1] > 200) {texture_terrain.data[3*i+1] = 200;}
  }
  
  // Vertex Shader
  const char* vertex_shader = load_shader("shaders//test_vs.glsl");
  if(vertex_shader == NULL)
  {
    print_log("ERROR: Failed to load test_vs.glsl\n");
    return -1;
  }

  int params = GL_TRUE;
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
  if(GL_TRUE != params)
  {
    print_log("ERROR: GL shader index %i did not compile.\n", vs);
    print_log_shader_info(vs);
    return -1;
  }

  // Fragment Shader
  const char* fragment_shader = load_shader("shaders//test_fs.glsl");
  if(fragment_shader == NULL)
  {
    print_log("ERROR: Failed to load test_fs.glsl\n");
    return -1;
  }

  params = GL_TRUE;
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
  if(GL_TRUE != params)
  {
    print_log("ERROR: GL shader index %i did not compile.\n", fs);
    print_log_shader_info(fs);
    return -1;
  }

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);
  
  GLint ret;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &ret);
  print_log("Program link status: %i\n", ret);

  /*
  // Does this even do anything?
  glBindAttribLocation(shader_program, 0, "vertex_position");
  glBindAttribLocation(shader_program, 1, "vertex_uv");
  glBindAttribLocation(shader_program, 2, "voffset");
  glBindAttribLocation(shader_program, 3, "rot1");
  glBindAttribLocation(shader_program, 4, "rot2");
  glBindAttribLocation(shader_program, 5, "rot3");
  */

  GLint loc_vp_matrix = glGetUniformLocation(shader_program, "vp_matrix");
  if(loc_vp_matrix < 0) {print_log("ERROR: Could not find uniform vp_matrix\n"); return -1;}

  // Load texture atlas
  GLuint texture_atlas_id = 0;
  glGenTextures(1, &texture_atlas_id);
  glBindTexture(GL_TEXTURE_2D, texture_atlas_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texture_atlas.width, texture_atlas.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_atlas.data);

  // Load texture skybox
  GLuint texture_skybox_id = 0;
  glGenTextures(1, &texture_skybox_id);
  glBindTexture(GL_TEXTURE_2D, texture_skybox_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texture_skybox.width, texture_skybox.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_skybox.data);

  // Load texture Terrain
  GLuint texture_terrain_id = 0;
  glGenTextures(1, &texture_terrain_id);
  glBindTexture(GL_TEXTURE_2D, texture_terrain_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texture_terrain.width, texture_terrain.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_terrain.data);
  
  camera.pitch = 0;
  camera.yaw = PI;
  camera.pos[0] = 0.0;  // 0.0
  camera.pos[1] = 25.0; // 0.0
  camera.pos[2] = 0.0;  // -4.0

  printf("Creating level\n");
  s_level* level = malloc(1 * sizeof *level);
  if(level == NULL) {return -1;}
  r = load_level(level, "levels\\level_1.dat");
  if(r != 0)
  {
    print_log("Load level failed\n");
    return -1;
  }
  
  //double end, begin = glfwGetTime(), time_spent;
  glClearColor(0.6, 0.6, 0.8, 1.0);
  while(!glfwWindowShouldClose(window))
  {
    r = simulate(level);
    if(r != 0)
    {
      print_log("ERROR: Simulation (%i)\n", r);
      break;
    }
    
    r = prepare_buffers(level);
    if(r != 0)
    {
      print_log("ERROR: Prepare Buffers (%i)\n", r);
      break;
    }
    
    // Limit FPS
    /*
    end = glfwGetTime();
    time_spent = (end - begin);
    if(time_spent < (1.0/max_fps))
    {
      Sleep((1.0/max_fps - time_spent)*1000);
    }
    begin = glfwGetTime();
    */
    
    // Create camera matrix
    s_mat4 proj_matrix = perspective_matrix(45.0, (float)window_width/window_height, 0.1, 1000.0);
    s_mat4 view_matrix = fps_view_rh(vec3(camera.pos[0], camera.pos[1], camera.pos[2]), camera.pitch, camera.yaw);
    s_mat4 vp_matrix = multiply_mat4(proj_matrix, view_matrix);
    
    glUniformMatrix4fv(loc_vp_matrix, 1, GL_TRUE, vp_matrix.m);
    
    update_fps_counter(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shader_program);
    
    //glActiveTexture(GL_TEXTURE1);
    int i;
    for(i = 0; i < level->num_objects; ++i) // level->num_objects
    {
      if(i == 0) {glBindTexture(GL_TEXTURE_2D, texture_atlas_id);}
      if(i == 2) {glBindTexture(GL_TEXTURE_2D, texture_skybox_id);}
      if(i == 3) {glBindTexture(GL_TEXTURE_2D, texture_terrain_id);}
      
      glBindVertexArray(level->objects[i].vao);
      glDrawElementsInstanced(GL_TRIANGLES, level->objects[i].num_indices, GL_UNSIGNED_INT, (void*)(0+0*sizeof(GL_UNSIGNED_INT)), level->objects[i].num_instances);
    }
    
    // Check for errors
    #ifndef NDEBUG
    while((err = glGetError()) != GL_NO_ERROR)
    {
      print_log("ERROR: glGetError() code %i\n", err);
    }
    #endif
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  // Check for errors
  while((err = glGetError()) != GL_NO_ERROR)
  {
    print_log("ERROR: on closing %i\n", err);
  }
  
  glfwTerminate();
  return 0;
}
