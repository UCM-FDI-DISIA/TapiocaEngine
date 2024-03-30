#version 330 core

/*
Un SHADER es un conjunto de instrucciones a la GPU que se ejecutan todos a la vez para todos
los pixeles de la pantalla (o los indicados). Por lo tanto, permite cambiar el color de estos pixeles.
De modo que se puede conseguir diferentes efectos especiales (diferentes tipos de iluminacion,
fuego, niebla...).
Segun esta definicion, es obvio entender que un shader puede tomar una textura o los vertices de
una malla y realizar modificaciones en ellos, puesto que tambien forman pixeles.
*/

/*
En el vertex shader se pasaran las coordenadas de los vertices,
que el rasterizador interpolara y generara fragmentos.
Estos fragmentos se le pasaran al fragment shader y son los que
se pintaran

Cada ejecucion se procesa 1 vertice y se genera un fragmento.
Se ejecuta multiples veces
*/

// hay 3 tipos de variables:
// - in ---> valores de entrada
//      En el caso del vertex shader son los valores de un vertice
//      de la malla
// - uniform ---> datos globales del programa
//              Se indican sus valores iniciales en .material y se
//              transfieren a la GPU cada vez que se utiliza un programa
// - out ---> informacion que se pasara a otro shader (directa
//              o indirectamente) o al sistema
//              Si se definen hay que darles valor
// X ej, out vec4 gl_Position es un atributo de salida del vertex
// shader, solo que esta predifinido y es obligatorio darle un valor

// coordenadas de posicion de cada vertice
in vec4 vertex;
// tiene que tener el mismo nombre que en .mat
uniform mat4 modelViewProjMat;

void main(void) {
    // recibe las coordenadas de los vertices en clip_space/coordenadas de recorte
    // es obligatorio definirlo para que se puedan crear fragmentos
    gl_Position = modelViewProjMat * vertex;
}