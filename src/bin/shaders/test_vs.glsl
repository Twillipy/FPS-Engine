#version 430

in vec3 v;
in vec2 vt;
in vec3 vn;
// Model matrix
in vec4 rot1;
in vec4 rot2;
in vec4 rot3;
in vec4 rot4;

out vec2 uv;
out vec4 frag_normal;

uniform mat4 vp_matrix;

void main()
{
  mat4 rotation_matrix = mat4(rot1, rot2, rot3, rot4);
  gl_Position = vp_matrix * rotation_matrix * vec4(v, 1.0);
  uv = vt;
  frag_normal = normalize(vec4(vn, 1.0)*rotation_matrix);
}