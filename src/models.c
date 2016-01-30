#include "defs.h"

typedef struct
{
  int v;
  int vt;
  int vn;
} s_point;

int load_mtl()
{
  return 0;
}

int point_find(s_point* points, int num_points, int index_v, int index_vt, int index_vn)
{
  assert(points != NULL);
  assert(num_points >= 0);
  
  int i;
  for(i = 0; i < num_points; ++i)
  {
    if(points[i].v == index_v && points[i].vt == index_vt && points[i].vn == index_vn)
    {
      return i;
    }
  }
  return -1;
}

int point_add(s_point* points, int pos, int index_v, int index_vt, int index_vn)
{
  assert(points != NULL);
  assert(pos >= 0);
  
  points[pos].v = index_v;
  points[pos].vt = index_vt;
  points[pos].vn = index_vn;
  
  return pos;
}

int points_print(s_point* points, int num_points)
{
  assert(points != NULL);
  assert(num_points >= 0);
  
  int i;
  for(i = 0; i < num_points && i < 16; ++i)
  {
    printf("%i) %i %i %i\n", i, points[i].v, points[i].vt, points[i].vn);
  }
  
  return 0;
}

int points_check_unique(s_point* points, int num_points)
{
  assert(points != NULL);
  assert(num_points >= 0);
  
  int r;
  int i;
  for(i = 0; i < num_points; ++i)
  {
    r = point_find(points, num_points, points[i].v, points[i].vt, points[i].vn);
    if(r != -1 && r != i) {return 0;}
  }
  return 1;
}

int load_obj(s_model* model, const char *path)
{
  if(path == NULL) {return -1;}
  if(model == NULL) {return -2;}
  FILE *file = fopen(path, "rb");
  if(file == NULL) {return -3;}

  int r;
  int i;
  float *temp = NULL;
  int index = 0;
  
  // v
  int num_v = 0;
  int max_v = 2048;
  float *v = malloc(3 * max_v * (sizeof *v));
  
  // vt
  int num_vt = 0;
  int max_vt = 2048;
  float *vt = malloc(2 * max_vt * (sizeof *vt));
  
  // vn
  int num_vn = 0;
  int max_vn = 2048;
  float *vn = malloc(3 * max_vn * (sizeof *vn));
  
  int num_points = 0;
  s_point points[2048];
  model->indices = malloc(4096 * 3 * (sizeof *model->indices));

  printf("Parsing file\n");
  // Gather model data
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    if(line[0] == '#') {continue;}
    char *value = strtok(line, " ");

    if(strcmp(value, "v") == 0)
    {
      // Increase storage if necessary
      if(num_v >= max_v)
      {
        temp = realloc(v, 2 * 3 * max_v * (sizeof *v)); // Double
        if(temp == NULL) {return -4;}
        free(v);
        v = temp;
        temp = NULL;
        max_v = max_v * 2;
      }
      
      // Store values
      value = strtok(NULL, " \n\0");
      v[3*num_v+0] = atof(value);

      value = strtok(NULL, " \n\0");
      v[3*num_v+1] = atof(value);

      value = strtok(NULL, " \n\0");
      v[3*num_v+2] = atof(value);

      num_v++;
    }
    else if(strcmp(value, "vt") == 0)
    {
      // Increase storage if necessary
      if(num_vt >= max_vt)
      {
        temp = realloc(vt, 2 * 3 * max_vt * (sizeof *vt)); // Double
        if(temp == NULL) {return -5;}
        free(vt);
        vt = temp;
        temp = NULL;
        max_vt = max_vt * 2;
      }
      
      // Store values
      value = strtok(NULL, " \n\0");
      vt[2*num_vt+0] = atof(value);

      value = strtok(NULL, " \n\0");
      vt[2*num_vt+1] = atof(value);

      num_vt++;
    }
    else if(strcmp(value, "vn") == 0)
    {
      // Increase storage if necessary
      if(num_vn >= max_vn)
      {
        temp = realloc(vn, 2 * 3 * max_vn * (sizeof *vn)); // Double
        if(temp == NULL) {return -6;}
        free(vn);
        vn = temp;
        temp = NULL;
        max_vn = max_vn * 2;
      }
      
      // Store values
      value = strtok(NULL, " \n\0");
      vn[3*num_vn+0] = atof(value);

      value = strtok(NULL, " \n\0");
      vn[3*num_vn+1] = atof(value);

      value = strtok(NULL, " \n\0");
      vn[3*num_vn+2] = atof(value);

      num_vn++;
    }
    else if(strcmp(value, "f") == 0)
    {
      int counter = 0;
      
      int index_origin = 0;
      int index_previous = 0;
      
      int index_v = -1;
      int index_vt = -1;
      int index_vn = -1;
      
      char* pch = NULL;
      
      while((value = strtok(NULL, " \n\0")) != NULL)
      {
        pch = value;
        
        index_v = atoi(pch)-1;
        if(index_v < 0) {break;}
        
        // Move along
        while(*pch != '/' && *pch != ' ' && *pch != '\n' && *pch != '\0') {pch++;}
        
        if(*pch == '/')
        {
          pch++;
          if(*pch != '/')
          {
            index_vt = atoi(pch)-1;
          }
          
          // Move along
          while(*pch != '/' && *pch != ' ' && *pch != '\n' && *pch != '\0') {pch++;}
          
          if(*pch == '/')
          {
            pch++;
            index_vn = atoi(pch)-1;
          }
        }
        
        // Add point
        r = point_find(points, num_points, index_v, index_vt, index_vn);
        if(r < 0)
        {
          r = point_add(points, num_points, index_v, index_vt, index_vn);
          num_points++;
        }
        
        if(counter == 0)
        {
          index_origin = r;
        }
        
        if(counter >= 2)
        {
          model->indices[index  ] = index_origin;
          model->indices[index+1] = index_previous;
          model->indices[index+2] = r;
          index += 3;
        }
        
        index_previous = r;
        counter++;
      }
    }
  }
  
  printf("Creating data arrays\n");
  // Create data arrays
  model->num_indices = index;
  model->num_v = num_v;
  model->num_vt = num_vt;
  model->num_vn = num_vn;
  
  model->v = malloc(3 * model->num_indices * (sizeof *model->v));
  model->vt = malloc(2 * model->num_indices * (sizeof *model->vt));
  model->vn = malloc(3 * model->num_indices * (sizeof *model->vn));
  
  if(model->v == NULL) {};
  if(model->vt == NULL) {};
  if(model->vn == NULL) {};
  
  printf("Dismantling points\n");
  for(i = 0; i < num_points; ++i)
  {
    if(points[i].v >= 0)
    {
      model->v[3*i + 0] = v[3*points[i].v + 0];
      model->v[3*i + 1] = v[3*points[i].v + 1];
      model->v[3*i + 2] = v[3*points[i].v + 2];
    }
    if(points[i].vt >= 0)
    {
      model->vt[2*i + 0] = vt[2*points[i].vt + 0];
      model->vt[2*i + 1] = vt[2*points[i].vt + 1];
    }
    if(points[i].vn >= 0)
    {
      model->vn[3*i + 0] = vn[3*points[i].vn + 0];
      model->vn[3*i + 1] = vn[3*points[i].vn + 1];
      model->vn[3*i + 2] = vn[3*points[i].vn + 2];
    }
  }
  
  #ifndef NDEBUG
  printf("File: %s\n", path);
  printf("Vertices: %i\n", model->num_v);
  printf("Texture coords: %i\n", model->num_vt);
  printf("Normals: %i\n", model->num_vn);
  printf("Indices: %i\n", model->num_indices);
  printf("Index: %i\n", index);
  printf("Points: %i\n", num_points);
  
  if(points_check_unique(points, num_points))
  {
    printf("Points unique\n");
  }
  else
  {
    printf("Points not unique!\n");
  }
  printf("\n");
  #endif
  
  fclose(file);
  free(v);
  free(vt);
  free(vn);
  return 0;
}

int print_log(const char *format, ...)
{
  FILE *file = fopen("log.txt", "a");
  if(!file) {return -1;}

  static int first = 0;
  if(first == 0)
  {
    fprintf(file, "---- %s %s ----\n", __DATE__, __TIME__);
  }
  first = 1;

  va_list args;
  va_start(args, format);
  vfprintf(file, format, args);
  vprintf(format, args);
  va_end(args);

  fclose(file);
  return 0;
}