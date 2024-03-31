#version 330 core

layout (location = 0) in vec4 vertex;

// matriz modelo-vista-proyeccion
// (se utiliza para obtener los vertices en el espacio de la pantalla)
uniform mat4 worldViewProjMatrix;
uniform vec4 texelOffsets;

out vec2 oDepth;

void main(void){
    // se convierten las coordenadas de los vertices en coordenadas de vista
    // (las que se ven en pantalla)
    gl_Position = worldViewProjMatrix * vertex;

    // fix pixel / texel alignment
	gl_Position.xy += texelOffsets.zw * gl_Position.w;
    
    oDepth.x = gl_Position.z;
	oDepth.y = gl_Position.w;
}