#version 330 core

in vec2 oDepth;

out vec4 fFragColor;

void main(void){
    float finalDepth = oDepth.x / oDepth.y;
    
    // just smear across all components 
	// therefore this one needs high individual channel precision
	fFragColor = vec4(vec3(finalDepth), 1.0);

    // se guarda la profundidad de la sombra
    //fFragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}