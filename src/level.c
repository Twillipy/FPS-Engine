#include "defs.h"

int load_landscape(s_model* model, const char *path)
{
  assert(model != NULL);
  assert(path != NULL);
  
  int r;
  
  s_texture heightmap;
  r = load_bmp(&heightmap, path);
  if(r != 0) {return r;}
  
  assert(heightmap.width >= 2);
  assert(heightmap.height >= 2);
  
  #ifndef NDEBUG
  printf("Terrain path: %s\n", path);
  printf("w: %i\n", heightmap.width);
  printf("h: %i\n", heightmap.height);
  printf("\n");
  #endif
  
  int num_triangles = 2 * (heightmap.width-1) * (heightmap.height-1);
  
  model->num_indices = 3*num_triangles;
  model->num_v  = 3*num_triangles;
  model->num_vt = 3*num_triangles;
  model->num_vn = 3*num_triangles;
  //model->num_vp = 0;
  
  model->indices = malloc(3 * model->num_indices * sizeof *model->indices);
  model->v  = malloc(3 * model->num_v  * sizeof *model->v );
  model->vt = malloc(2 * model->num_vt * sizeof *model->vt);
  model->vn = malloc(3 * model->num_vn * sizeof *model->vn);
  //model->vp = malloc(3 * model->num_vp * sizeof *model->vp);
  
  int current;
  int h;
  int w;
  int i = 0;
  
  // v
  current = 0;
  for(h = 0; h < heightmap.height; ++h)
  {
    for(w = 0; w < heightmap.width; ++w)
    {
      model->v[3*current+0] = h - heightmap.height/2;
      model->v[3*current+1] = 20.0*(float)heightmap.data[3*(heightmap.height*h + w) + 0]/255.0;
      model->v[3*current+2] = -w + heightmap.width/2;      
      current++;
    }
  }
  
  // vt
  current = 0;
  for(h = 0; h < heightmap.height; ++h)
  {
    for(w = 0; w < heightmap.width; ++w)
    {
      model->vt[2*current+0] = 32.0*(float)w/heightmap.width;
      model->vt[2*current+1] = 32.0*(float)h/heightmap.height;
      current++;
    }
  }
  
  // vn
  current = 0;
  for(h = 0; h < heightmap.height-1; ++h)
  {
    for(w = 0; w < heightmap.width-1; ++w)
    {      
      s_vec3 d1;
      d1.i = model->v[3*(heightmap.height*(h+1) + w) + 0] - model->v[3*(heightmap.height*h + w) + 0];
      d1.j = model->v[3*(heightmap.height*(h+1) + w) + 1] - model->v[3*(heightmap.height*h + w) + 1];
      d1.k = model->v[3*(heightmap.height*(h+1) + w) + 2] - model->v[3*(heightmap.height*h + w) + 2];
      
      s_vec3 d2;
      d2.i = model->v[3*(heightmap.height*h + (w+1)) + 0] - model->v[3*(heightmap.height*h + w) + 0];
      d2.j = model->v[3*(heightmap.height*h + (w+1)) + 1] - model->v[3*(heightmap.height*h + w) + 1];
      d2.k = model->v[3*(heightmap.height*h + (w+1)) + 2] - model->v[3*(heightmap.height*h + w) + 2];
      
      s_vec3 dir = cross_vec3(d1, d2);
      model->vn[3*current+0] = dir.i;
      model->vn[3*current+1] = dir.j;
      model->vn[3*current+2] = dir.k;
      
      /*
      model->vn[3*current+0] = 0.0;
      model->vn[3*current+1] = 1.0;
      model->vn[3*current+2] = 1.0;
      */
      
      current++;
    }
    current++;
  }
  
  current = 0;
  for(h = 0; h < heightmap.height-1; ++h)
  {
    for(w = 0; w < heightmap.width-1; ++w)
    {
      // indices
      model->indices[6*i+0] = heightmap.height*(h  ) + (w    );
      model->indices[6*i+1] = heightmap.height*(h+1) + (w    );
      model->indices[6*i+2] = heightmap.height*(h  ) + (w + 1);
      
      model->indices[6*i+3] = heightmap.height*(h  ) + (w + 1);
      model->indices[6*i+4] = heightmap.height*(h+1) + (w    );
      model->indices[6*i+5] = heightmap.height*(h+1) + (w + 1);
      i++;
    }
  }
  
  /*
  while(i < 16)
  {
         if(heightmap.data[3*h+w] < 10)  {printf("[  %i]", heightmap.data[3*h+w]);}
    else if(heightmap.data[3*h+w] < 100) {printf("[ %i]", heightmap.data[3*h+w]);}
    else                                 {printf("[%i]", heightmap.data[3*h+w]);}
    
    i++;
  }
  */
  /*
  printf("Heights:\n");
  for(h = 0; h < heightmap.height; ++h)
  {
    for(w = 0; w < heightmap.width; ++w)
    {
           if(heightmap.data[3*h+w] < 10)  {printf("[  %i]", heightmap.data[3*h+w]);}
      else if(heightmap.data[3*h+w] < 100) {printf("[ %i]", heightmap.data[3*h+w]);}
      else                                 {printf("[%i]", heightmap.data[3*h+w]);}
    }
    printf("\n");
  }
  printf("\n");
  */
  return 0;
}

int load_level(s_level* level, const char *path)
{
  assert(level != NULL);
  assert(path != NULL);
  
  FILE* file = fopen(path, "r");
  if(file == NULL) {return -1;}
  
  int r;
  level->num_objects = 0;
  level->num_models = 0;
  
  // Boxes
  level->num_boxes = 0;
  level->boxes = malloc(16 * sizeof *level->boxes);
  level_add_box(level,  0.0, 0.0, 0.0,  0.0, 0.0, 0.0);
  level_add_box(level,  2.0, 0.1,-2.0,  0.0, 0.0, 55.0);
  level_add_box(level,  3.2, 0.0,-2.0,  0.0, 0.0, 180.0);
  level_add_box(level,  0.2, 1.0, 0.1,  0.0, 45.0, 0.0);
  
  // Teapots
  level->num_teapots = 0;
  level->teapots = malloc(16 * sizeof *level->teapots);
  level_add_teapot(level,  3.2, 0.45,-2.0,  0.0, 0.0, 0.0);
  level_add_teapot(level,  0.0,-0.45,-2.0,  0.0, 20.0, 0.0);
  
  char line[512];
  while(fgets(line, sizeof(line), file))
  {
    if(line[0] == '#' || strlen(line) < 5) {continue;}
    char *ptr = line;
    
    if(line[strlen(line)-1] == '\n')
    {
      line[strlen(line)-1] = '\0';
    }
    
    if(strncmp(line, "heightmap", 9) == 0)
    {
      while(*ptr != '=' && *ptr != '\n' && *ptr != '\0') {ptr++;} ptr++;
      
      r = load_landscape(&level->models[level->num_models], ptr);      
      r = create_object(&level->objects[level->num_objects], &level->models[level->num_objects]);
      level->objects[level->num_objects].num_instances = 1;
      level->num_models++;
      level->num_objects++;
    }
    else if(strncmp(line, "model", 5) == 0)
    {
      while(*ptr != '=' && *ptr != '\n' && *ptr != '\0') {ptr++;} ptr++;
      r = load_obj(&level->models[level->num_models], ptr);
      create_object(&level->objects[level->num_objects], &level->models[level->num_objects]);
      level->num_models++;
      level->num_objects++;
    }
    else if(strncmp(line, "width", 5) == 0)
    {
      while(*ptr != '=' && *ptr != '\n' && *ptr != '\0') {ptr++;} ptr++;
      level->width = atof(ptr);
    }
    else if(strncmp(line, "height", 6) == 0)
    {
      while(*ptr != '=' && *ptr != '\n' && *ptr != '\0') {ptr++;} ptr++;
      level->height = atof(ptr);
    }
  }

  #ifndef NDEBUG
  printf("Level: %s\n", path);
  printf("Width: %.4g\n", level->width);
  printf("Height: %.4g\n", level->height);
  printf("Models: %i\n", level->num_models);
  
  printf("Items:\n");
  printf(" %i - Boxes\n", level->num_boxes);
  printf(" %i - Teapots\n", level->num_teapots);
  printf("\n");
  #endif
  
  assert(level->num_models == level->num_objects);
  
  fclose(file);
  return 0;
}