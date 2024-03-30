#define PCF
//#define FUZZY_TEST

//#ifdef PCF
uniform float inverseShadowmapSize;
//#endif

uniform sampler2D shadowMap;

uniform float fixedDepthBias;
uniform float gradientClamp;
uniform float gradientScaleBias;

// vertice desde el punto de vista de la luz
in vec4 oUv;
in vec4 outColor;

out vec4 fFragColor;
 
void main()
{
    vec4 shadowUV = oUv;
    // punto en el depth shadow map
    // se usa para obtener la profundida del vertice desde el punto de vista de la luz
    // convertir coordenadas de vista en NCD (coordenadas para la textura)
    shadowUV = shadowUV / shadowUV.w;
    float centerdepth = texture2D(shadowMap, shadowUV.xy).x;
 
#ifndef OGRE_REVERSED_Z
    // convertir -1...1 a 0...1 (normalizarlo)
    shadowUV.z = shadowUV.z * 0.5 + 0.5;
#endif
 
    // shadowUV.z contiene la posicion del fragmento en el espacio de la luz
//#ifdef PCF
    float pixeloffset = inverseShadowmapSize;
    vec4 depths = vec4(
        texture2D(shadowMap, shadowUV.xy + vec2(-pixeloffset, 0.0)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(+pixeloffset, 0.0)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(0.0, -pixeloffset)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(0.0, +pixeloffset)).x);
    
    /*
    vec2 differences = abs(depths.yw - depths.xz);
	float gradient = min(gradientClamp, max(differences.x, differences.y));
	float gradientFactor = gradient * gradientScaleBias;

	// visibility function
	float depthAdjust = gradientFactor + (fixedDepthBias * centerdepth);
	float finalCenterDepth = centerdepth + depthAdjust;
    */

#if FUZZY_TEST
    // fuzzy test - introduces some ghosting in result and doesn't appear to be needed?
    //float visibility = saturate(1 + delta_z / (gradient * shadowFuzzyWidth));
    float visibility = saturate(1 + (finalCenterDepth - shadowUV.z) * shadowFuzzyWidth * shadowUV.w);

    fFragColor = vertexColour * visibility;
#else
    // hard test

#ifdef PCF
    // utilizar las profundiades anteriores, calcular la diferencia
    float final = (centerdepth > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.x > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.y > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.z > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.w > shadowUV.z) ? 1.0 : 0.0;
    
    final *= 0.2;
 
    fFragColor = vec4(outColor.xyz * final, 1.0);
    
#else
    // se compara la posicion del vertice en el mundo (desde el punto de vista de la camara)
    // y la posicion del vertice desde el punto de vista de la luz
    // La posicion del verticia del punto se ha tenido que converitr al espacio de la luz para poder usar el mismo sistema de referencia
    fFragColor = (centerdepth > shadowUV.z) ? vec4(outColor.xyz, 1.0) : vec4(0,0,0,1);
#endif

#endif

}