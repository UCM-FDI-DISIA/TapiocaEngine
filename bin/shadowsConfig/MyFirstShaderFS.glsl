#version 330 core

/*
El fragment shader se encarga de colorear los fragmentos que se han
definido a partir del vertex shader. Cada ejecucion procesa un fragmento
Tambien recibe otra informacion de vertex shader que puede
ser de utilidad para su configuracion.

in ---> recibe los valores de un fragmento, que se han pasado como
        out en el vertex shader
*/

// color del fragmento/pixel, que al tratarse de un atributo de salida
// procesara el sistema
out vec4 fFragColor;

void main() {
    // el vec4 representa el color naranja
    fFragColor = vec4(1.0, 0.5, 0.2, 1.0);
}