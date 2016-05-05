#include "defs.h"

int create_object(s_object* object, s_model* model)
{
  assert(object != NULL);
  assert(model != NULL);

  glGenVertexArrays(1, &object->vao);
  glBindVertexArray(object->vao);
  
  // v buffer
  glGenBuffers(1, &object->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, 3*(model->num_indices)*(sizeof *model->vn), model->v, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
  // vt buffer
  glGenBuffers(1, &object->vtbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->vtbo);
  glBufferData(GL_ARRAY_BUFFER, 2*(model->num_indices)*(sizeof *model->vt), model->vt, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  
  // vn buffer
  glGenBuffers(1, &object->vnbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->vnbo);
  glBufferData(GL_ARRAY_BUFFER, 3*(model->num_indices)*(sizeof *model->vn), model->vn, GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
  // Model matrix buffer
  glGenBuffers(1, &object->mmbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->mmbo);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(0 +  0*sizeof(float)));
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(0 +  4*sizeof(float)));
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(0 +  8*sizeof(float)));
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(0 + 12*sizeof(float)));
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  
  // Index buffer
  glGenBuffers(1, &object->ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (model->num_indices) * sizeof *model->indices, model->indices, GL_STATIC_DRAW);

  object->num_vertices = model->num_v;
  object->num_instances = 0;
  object->num_indices = model->num_indices;

  return 0;
}
