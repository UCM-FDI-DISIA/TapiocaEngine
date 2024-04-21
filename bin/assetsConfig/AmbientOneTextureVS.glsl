#version 330 core

// (da error en algunos assets)
//layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 uv0;
in vec4 position;
in vec2 uv0;

uniform mat4 worldViewProj;
uniform vec4 ambient;

out vec2 oUv;
out vec4 colour;

void main(void) {
  gl_Position = worldViewProj * position;
  oUv = uv0;
  colour = ambient;
}