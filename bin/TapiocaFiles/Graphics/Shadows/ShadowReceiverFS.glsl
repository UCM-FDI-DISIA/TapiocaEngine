#version 330 core

uniform float inverseShadowmapSize;
uniform float fixedDepthBias;

// PSSM
uniform vec4 pssmSplitPoints;
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;

//uniform sampler2D shadowMap;
//uniform float gradientClamp;
//uniform float gradientScaleBias;

in float outShadowColor;
in vec4 outLightColour;

in vec4 outColor;

// PSSM
in float depth;
in vec4 oUv0;
in vec4 oUv1;
in vec4 oUv2;

// vertice desde el punto de vista de la luz
//in vec4 oUv;

out vec4 fFragColor;

// intepolacion lineal
float texture2DCompare(sampler2D depths, vec2 uv, float compare){
    float depth = texture2D(depths, uv).r;
    // depth bias
    return step(compare - fixedDepthBias, depth);
}

float texture2DShadowLerp(sampler2D depths, vec2 size, vec2 uv, float compare){
    vec2 texelSize = vec2(1.0)/size;
    vec2 f = fract(uv*size+0.5);
    vec2 centroidUV = floor(uv*size+0.5)/size;

    float lb = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 0.0), compare);
    float lt = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 1.0), compare);
    float rb = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 0.0), compare);
    float rt = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 1.0), compare);
    float a = mix(lb, lt, f.y);
    float b = mix(rb, rt, f.y);
    float c = mix(a, b, f.x);
    return c;
}
   

void shadowFilter( sampler2D shadowMap, vec4 oUv, vec3 splitColour )
{
    // Perform perspective divide
    vec4 shadowUV = oUv;
    shadowUV = shadowUV / shadowUV.w;
 
    // Transform [-1, 1] to [0, 1] range
    shadowUV.z = shadowUV.z * 0.5 + 0.5;
 
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //float centerdepth = texture2D(shadowMap, shadowUV.xy).x;

    // PCF 3X3
    int pcf = 3;
    float shadowFactor = 0.0;
	for (int y = 0 ; y < pcf; y++) {
        for (int x = 0 ; x < pcf; x++) {
            vec2 offsets = vec2(float(x-1) * inverseShadowmapSize, float(y-1) * inverseShadowmapSize);
            
            shadowFactor += texture2DShadowLerp(shadowMap, 1/vec2(inverseShadowmapSize, inverseShadowmapSize), shadowUV.xy + offsets, shadowUV.z);
        }
	}
    // shadowFactor va desde 0 hasta 1
	shadowFactor= shadowFactor*0.11111111;

    // SE HAN ACLARADO LAS SOMBRAS QUE REFLEJAN UNOS OBJETOS EN OTROS

    // SI SE QUIERE QUE EL COLOR DE LA SOMBRAS NO SEA DE UN TONO DE LA ESCALA DE GRISES
    // CAMBIAR EL COLOR DE LA LUZ Y HACER QUE EL shadowColor SEA UN GRIS
    // (si es negro, las partes totalmente sombreadas se van a seguir viendo negras)

    // hacer que las sombras que se proyectan cambien con el color de la luz
    vec4 lightShadowColor = outLightColour * outShadowColor;

    // mix hace una interpolacion lineal entre dos valores a partir de un tercer valor
    vec3 finalColor = mix(lightShadowColor.rgb, outColor.rgb, shadowFactor);
    fFragColor = vec4(finalColor, 1.0);

    //fFragColor = vec4(outColor.xyz * shadowFactor, 1.0);

    //fFragColor = (centerdepth > shadowUV.z) ? vec4(outColor.xyz * splitColour, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
 
void main()
{
    // CALCULO DE LAS SOMBRAS QUE PROYECTAN UNOS OBJETOS EN OTROS (PARA ESO ES EL SHADOW MAPPING)
    if( depth <= pssmSplitPoints.y )
    {
        shadowFilter(shadowMap0, oUv0, vec3( 1.0, 0.0, 0.0 ) );
    }
    else if( depth <= pssmSplitPoints.z )
    {
        shadowFilter(shadowMap1, oUv1, vec3( 0.0, 1.0, 0.0 ) );
    }
    else if( depth <= pssmSplitPoints.w )
    {
        shadowFilter(shadowMap2, oUv2, vec3( 0.0, 0.0, 1.0 ) );
    }
    else
    {
        discard;
    }

    /*
    vec4 shadowUV = oUv;
    // punto en el depth shadow map
    // se usa para obtener la profundida del vertice desde el punto de vista de la luz
    // convertir coordenadas de vista en NCD (coordenadas para la textura)
    shadowUV = shadowUV / shadowUV.w;
    
    float centerdepth = texture2D(shadowMap, shadowUV.xy).x;
 
    // convertir -1...1 a 0...1 (normalizarlo)
    shadowUV.z = shadowUV.z * 0.5 + 0.5;
 
    // shadowUV.z contiene la posicion del fragmento en el espacio de la luz
    float pixeloffset = inverseShadowmapSize;
    vec4 depths = vec4(
        texture2D(shadowMap, shadowUV.xy + vec2(-pixeloffset, 0.0)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(+pixeloffset, 0.0)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(0.0, -pixeloffset)).x,
        texture2D(shadowMap, shadowUV.xy + vec2(0.0, +pixeloffset)).x);
    
    vec2 differences = abs(depths.yw - depths.xz);
	float gradient = min(gradientClamp, max(differences.x, differences.y));
	float gradientFactor = gradient * gradientScaleBias;

	// visibility function
	float depthAdjust = gradientFactor + (fixedDepthBias * centerdepth);
	float finalCenterDepth = centerdepth + depthAdjust;

    // utilizar las profundiades anteriores, calcular la diferencia
    float final = (finalCenterDepth > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.x > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.y > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.z > shadowUV.z) ? 1.0 : 0.0;
    final += (depths.w > shadowUV.z) ? 1.0 : 0.0;
    
    final *= 0.2;
 
    fFragColor = vec4(outColor.xyz * final, 1.0);

    // se compara la posicion del vertice en el mundo (desde el punto de vista de la camara)
    // y la posicion del vertice desde el punto de vista de la luz
    // La posicion del verticia del punto se ha tenido que converitr al espacio de la luz para poder usar el mismo sistema de referencia
    //fFragColor = (finalCenterDepth > shadowUV.z) ? vec4(outColor.xyz, 1.0) : vec4(0,0,0,1);
*/
}