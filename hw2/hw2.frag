#version 120

varying float LightIntensity;
varying vec2  ModelPos;

//The following block is adapted from https://github.com/sol-prog/Perlin_Noise/blob/master/PerlinNoise.cpp
vec2 fade(vec2 t)
{
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec3 lerp(float w, float a, float b)
{
	return ((1.0-w)*a) + (w*b);
}

float grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	// Convert lower 4 bits of hash inot 12 gradient directions
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float pnoise(vec3 P)
{
	vec3 V = floor(P);
	P -= floor(P);
	vec3 f = vec3(fade(P.x), fade(P.y), fade(P.z));
	
	int A = p[V.x] + V.y;
	int AA = p[A] + V.z;
	int AB = p[A + 1] + V.z;
	int B = p[V.x + 1] + V.y;
	int BA = p[B] + V.z;
	int BB = p[B + 1] + V.z;
	
	double res = lerp(f.z, lerp(f.y, lerp(f.x, grad(p[AA], P.x, P.y, P.z), grad(p[BA], P.x-1, P.y, P.z)),
						        lerp(f.x, grad(p[AB], P.x, P.y-1, P.z), grad(p[BB], P.x-1, P.y-1, P.z))),
						   lerp(f.y, lerp(f.x, grad(p[AA+1], x, y, z-1), grad(p[BA+1], P.x-1, P.y, P.z-1)),
						        lerp(f.x, grad(p[AB+1], P.x, P.y-1, P.z-1),	grad(p[BB+1], P.x-1, P.y-1, P.z-1))));
	
	return (res + 1.0)/2.0;
}
//end cited code block


void main()
{
    //float intensity = fbm(ModelPos, 4, 2, 2) * 2.0;
	float intensity = pnoise(vec3(ModelPos.xy, 0) + .03125;
    vec3 color  = vec3(intensity, intensity, intensity) * 1.5;//LightIntensity;
    //  Adjust color intensity for lighting (interpolated from vertex shader values)
    //  Pad color with alpha
    gl_FragColor = vec4(color,1.0);
}
