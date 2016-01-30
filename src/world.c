#include "defs.h"

float dt = 1.0/60.0;

int simulate(s_level* level)
{
  if(level == NULL) {return -1;}

  int i;

  for(i = 0; i < level->num_boxes; ++i)
  {
    level->boxes[i].x += level->boxes[i].v_x*dt;
    level->boxes[i].y += level->boxes[i].v_y*dt;
    level->boxes[i].z += level->boxes[i].v_z*dt;
  }

  for(i = 0; i < level->num_teapots; ++i)
  {
    level->teapots[i].r_y += 1;
    level->teapots[i].x += level->teapots[i].v_x*dt;
    level->teapots[i].y += level->teapots[i].v_y*dt;
    level->teapots[i].z += level->teapots[i].v_z*dt;
  }

  return 0;
}

int level_add_box(s_level* level, float x, float y, float z, float r_x, float r_y, float r_z)
{
  if(level == NULL) {return -1;}
  if(level->num_boxes >= 16) {return -2;}
  if(level->boxes == NULL) {return -3;}

  int n = level->num_boxes;
  level->boxes[n].x = x;
  level->boxes[n].y = y;
  level->boxes[n].z = z;
  level->boxes[n].r_x = r_x;
  level->boxes[n].r_y = r_y;
  level->boxes[n].r_z = r_z;
  level->boxes[n].v_x = 0.0;
  level->boxes[n].v_y = 0.0;
  level->boxes[n].v_z = 0.0;
  level->num_boxes++;
  return 0;
}

int level_add_teapot(s_level* level, float x, float y, float z, float r_x, float r_y, float r_z)
{
  if(level == NULL) {return -1;}
  if(level->num_teapots >= 16) {return -2;}
  if(level->teapots == NULL) {return -3;}

  int n = level->num_teapots;
  level->teapots[n].x = x;
  level->teapots[n].y = y;
  level->teapots[n].z = z;
  level->teapots[n].r_x = r_x;
  level->teapots[n].r_y = r_y;
  level->teapots[n].r_z = r_z;
  level->teapots[n].v_x = 0.0;
  level->teapots[n].v_y = 0.0;
  level->teapots[n].v_z = 0.0;
  level->num_teapots++;
  return 0;
}