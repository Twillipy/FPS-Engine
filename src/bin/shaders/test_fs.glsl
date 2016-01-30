#version 430

in vec2 uv;
in vec4 frag_normal;

out vec4 frag_colour;

vec3 surfaceToLight = vec3(0.0, 1.0, 1.0);

uniform sampler2D my_texture;

void main()
{
  float brightness = dot(frag_normal.xyz, surfaceToLight) / (length(surfaceToLight) * length(frag_normal.xyz));
  brightness = clamp(brightness, 0.2, 1.0);
  frag_colour = vec4(brightness * texture(my_texture, uv).rgb, 1.0);
}