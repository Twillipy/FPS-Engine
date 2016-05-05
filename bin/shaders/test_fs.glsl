#version 430

in vec2 uv;
in vec4 frag_normal;

out vec4 frag_colour;

vec3 surfaceToLight = vec3(0.0, 1.0, 1.0);

uniform sampler2D my_texture;

uniform struct
{
  vec4 pos;
  vec3 colour;
  // spotlight
  float angle;
  vec3 dir;
} s_light;

/*
vec3 apply_light(s_light light, vec3 surface_colour, vec3 surface_normal, vec3 surface_pos)
{
  // 
  if(light.w == 0.0)
  {
  }
  else
  {
  }
}
*/

void main()
{
  float brightness = dot(frag_normal.xyz, surfaceToLight) / (length(surfaceToLight) * length(frag_normal.xyz));
  brightness = clamp(brightness, 0.2, 1.0);
  frag_colour = vec4(brightness * texture(my_texture, uv).rgb, 1.0);
}