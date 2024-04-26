#version 330 core

uniform mat4 modelMat;
uniform mat4 normalModelMat;
uniform mat4 worldViewProjMatrix;
uniform vec4 lightPosition;
uniform vec4 lightColour;

uniform mat4 texViewProj;
 
layout (location = 0) in vec4 vertex;
layout (location = 2) in vec3 normal;
 
out vec4 outColor;

out vec4 oUv0;
  
void main()
{
    gl_Position = worldViewProjMatrix * vertex;
        
    // transformar el vertice a espacio de mundo
    vec4 worldPos = modelMat * vertex;
    // transformar la normal a espacio de mundo
    vec3 worldNorm = normalize((normalModelMat * vec4(normal, 0.0)).xyz);

    // calcular la luz (como en IF)
    // lightPosition.w = 0 -> luz dir   lightPosition.w = 1 -> luz pos
    // sacar la dir de la luz
    // si es dir, se saca directamente
    // si es posicional, se calcula el vector desde el vertice hasta la luz
    vec3 lightDir = normalize(lightPosition.xyz - (normalize(worldPos.xyz) * lightPosition.w));

    // comprobar si la luz esta debajo del objeto o no
    outColor = lightColour * max(dot(lightDir, worldNorm), 0.0);

    // vertice desde el punto de vista de la luz
    oUv0 = texViewProj * worldPos;
}