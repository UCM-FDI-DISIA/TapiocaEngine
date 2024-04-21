#version 330 core

uniform float shadowColor;

uniform mat4 modelMat;
uniform mat4 normalModelMat;
uniform mat4 worldViewProjMatrix;
uniform vec4 lightPosition;
uniform vec4 lightColour;

// PSM
uniform mat4 texViewProj0;
uniform mat4 texViewProj1;
uniform mat4 texViewProj2;

//uniform mat4 texViewProj;
 
layout (location = 0) in vec4 vertex;
layout (location = 2) in vec3 normal;

out float outShadowColor;
out vec4 outLightColour;

out vec4 outColor;

// PSSM
out float depth;

out vec4 oUv0;
out vec4 oUv1;
out vec4 oUv2;
//out vec4 oUv;
  
void main()
{
    gl_Position = worldViewProjMatrix * vertex;
    
    depth = gl_Position.z;
    
    // transformar el vertice a espacio de mundo
    vec4 worldPos = modelMat * vertex;
    // transformar la normal a espacio de mundo
    vec3 worldNorm = normalize((normalModelMat * vec4(normal, 0.0)).xyz);

    // CALCULO DE LAS AUTOSOMBRAS

    // calcular la luz (como en IG)
    // lightPosition.w = 0 -> luz dir   lightPosition.w = 1 -> luz pos
    // sacar la dir de la luz
    // si es dir, se saca directamente
    // si es posicional, se calcula el vector desde el vertice hasta la luz
    vec3 lightDir = normalize(lightPosition.xyz - (normalize(worldPos.xyz) * lightPosition.w));

    // comprobar si la luz esta debajo del objeto o no
    outColor = lightColour * max(dot(lightDir, worldNorm), shadowColor);    // antes 0.0
    
    outShadowColor = shadowColor;
    outLightColour = lightColour;

    // calculate shadow map coords
    oUv0 = texViewProj0 * worldPos;
    oUv1 = texViewProj1 * worldPos;
    oUv2 = texViewProj2 * worldPos;

    // vertice desde el punto de vista de la luz
    //oUv = texViewProj * worldPos;
}