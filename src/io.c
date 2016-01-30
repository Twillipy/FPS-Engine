#include "defs.h"

int load_settings(const char *filename)
{
  if(filename == NULL) {return -1;}

  FILE *file = fopen(filename, "r");
  if(file == NULL) {return -2;}

  char line[256];
  while(fgets(line, sizeof(line), file))
  {
    char *setting = strtok(line, "=");
    char *value = strtok(NULL, "\n\0");

    if(setting[0] == '#') {continue;}

    if(strncmp(setting, "width", 5) == 0)
    {
      window_width = atoi(value);
    }
    else if(strncmp(setting, "height", 6) == 0)
    {
      window_height = atoi(value);
    }
    else if(strncmp(setting, "fullscreen", 10) == 0)
    {
      if(strncmp(value, "TRUE", 4) == 0)
      {
        window_fullscreen = 1;
      }
      else
      {
        window_fullscreen = 0;
      }
    }
    else if(strncmp(setting, "max_fps", 7) == 0)
    {
      max_fps = atoi(value);
    }
    else
    {
      print_log("ERROR: Unknown setting (%s) value (%s)\n", setting, value);
    }
  }

  fclose(file);
  return 0;
}

char* load_shader(const char *filename)
{
  if(filename == NULL) {return NULL;}

  FILE *file = fopen(filename, "rb");
  if(!file) {return NULL;}

  fseek(file, 0, SEEK_END);
  long lSize = ftell(file);
  rewind(file);

  char *buffer = calloc(1, lSize+1);
  if(!buffer)
  {
    fclose(file);
    return NULL;
  }

  if(1 != fread(buffer, lSize, 1, file))
  {
    fclose(file);
    free(buffer);
    return NULL;
  }

  fclose(file);
  return buffer;
}

int load_bmp(s_texture* texture, const char *filename)
{
  if(filename == NULL) {return -1;}
  FILE *file = fopen(filename, "rb");
  if(file == NULL) {return -2;}

  unsigned char header[54];
  if(fread(header, 1, 54, file) != 54) {fclose(file); return -3;}
  if(header[0] != 'B' || header[1] != 'M') {fclose(file); return -4;}

  int data_pos   = *(int*)&(header[0x0A]);
  int image_size = *(int*)&(header[0x22]);
  int width      = *(int*)&(header[0x12]);
  int height     = *(int*)&(header[0x16]);
  //int bpp        = *(int*)&(header[0x1C]); // bits per pixel

  if(image_size == 0) {image_size = 3*width*height;}
  if(data_pos == 0) {data_pos = 54;}

  unsigned char* data = malloc(image_size * sizeof *data);
  if(data == NULL) {fclose(file); return -5;}
  int r = fread(data, 1, image_size, file);
  if(r != image_size) {fclose(file); return -6;}
  
  // Swap from BGR to RGB
  int i;
  for(i = 0; i < image_size; i += 3)
  {
    unsigned char temp = data[i+0];
    data[i+0] = data[i+2];
    data[i+2] = temp;
  }
  
  fclose(file);

  texture->width = width;
  texture->height = height;
  texture->data = data;

  return 0;
}

void print_log_shader_info(GLuint shader_index)
{
  int max_length = 2048;
  int actual_length = 0;
  char log[2048];
  glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
  print_log("Shader info log for GL index %i: %s\n", shader_index, log);
}
