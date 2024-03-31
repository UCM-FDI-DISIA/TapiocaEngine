#version 330 core

uniform sampler2D textura;

in vec2 oUv;
in vec4 colour;

out vec4 fFragColor;

void main(void) {
  vec4 texturaColor = texture(textura, oUv);
  fFragColor = texturaColor * colour;
}