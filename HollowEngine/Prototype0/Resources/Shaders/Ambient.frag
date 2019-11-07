#version 430

#define PI 3.14159265

out vec4 color;

in vec2 texCoords;

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform vec3 viewPosition;

// IBL
uniform sampler2D irradianceMap;
uniform sampler2D hdrMap;
uniform float exposure;
uniform float contrast;
uniform float hdrWidth;
uniform float hdrHeight;

uniform HammersleyBlock{
	float NumDirections;
	float hammersley[2*100];
};

// Distribution term for BRDF
float DistributionTerm(float a, vec3 H, float HN)
{
	return ((a + 2.0)/(2.0*PI))*(pow(HN,a));
}

// Geometry term for BRDF
float GeometryTerm(vec3 L, vec3 V, vec3 H)
{
	return 1.0;
	 //return 1.0 / (pow(dot(L, H), 2));
}

// Fresnel term for BRDF
vec3 FresnelTerm(vec3 Ks, vec3 L, vec3 H)
{
	return Ks + (1.0 - Ks)*(pow(1.0 - dot(L, H), 5.0));
}

// Calculate irradiance from irradiance map for IBL
vec3 Irradiance(vec3 N)
{
	vec2 uv = vec2(0.5 - (atan(N.z, N.x))/(2.0*PI), acos(-N.y)/PI);
	vec3 diffuseIrradiance = texture(irradianceMap, uv).rgb; 
	return diffuseIrradiance;
}

// Calculate specular IBL from random samples
vec3 SpecularIBL(vec3 N, vec3 V, vec3 ks, float a)
{
	// Form rotation that takes Z-Axis to reflection direction R  2*(NdotV)*N - V
	// N is normal vector, V is viewing vector
	vec3 R = (2.0*(max(dot(N, V), 0.0))*N) - V;
	vec3 Zaxis = vec3(0.0, 0.0, 1.0);
	vec3 A = normalize(vec3(-R.y, R.x, 0.0));
	vec3 B = normalize(cross(R, A));

	// Loop through directions
	int numPoints = int(NumDirections);
	float WIDTH = hdrWidth;
	float HEIGHT = hdrHeight;
	vec3 specular = vec3(0.0, 0.0, 0.0);
	for(int curWk = 0; curWk < numPoints; curWk += 2)
	{
		// Skew random points to match distribution D(H)
		float u = hammersley[curWk];
		float v = acos(pow(hammersley[curWk+1], (1.0/(a + 1.0))))/PI;

		// Form direction vector L
		vec3 L = vec3(cos(2.0*PI*(0.5-u))*sin(PI*v), sin(2.0*PI*(0.5-u))*sin(PI*v), cos(PI*v));

		// Calculate light direction vector
		vec3 wk = normalize(L.x*A + L.y*B + L.z*R);
		// Calculate new H vector
		vec3 H = normalize(wk+V);

		// For each wk, evaluate incoming light by accessing HDR image with wk
		float DH = DistributionTerm(a, H, dot(H,N));
		float level = 0.5*log2((WIDTH*HEIGHT) / numPoints) - 0.5*log2(DH);
		// Get UV coordinates to grab based on wk
		vec2 uv = vec2(0.5 - ((atan(wk.z, wk.x))/(2.0*PI)), acos(-wk.y)/PI);
		vec3 Liwk = textureLod(hdrMap, uv, level).rgb;
	
		// Evaluate Monte-Carlo estimator for each pair wk and Li(wk)
		specular += (1.0/numPoints)*((GeometryTerm(wk, V, H)*FresnelTerm(ks, wk, H))/(4.0))*Liwk*max(dot(N, wk), 0.0);
	}

	return specular;
}

void main()
{
	// Get data from G-Buffer
	vec4 fragmentPosition = texture(gPosition, texCoords);
	vec3 normalVector = texture(gNormal, texCoords).rgb;
	vec3 diffuseColor = texture(gDiffuse, texCoords).rgb;
	vec3 specularColor = texture(gSpecular, texCoords).rgb;
	float shininess = texture(gSpecular, texCoords).a;

	// Get normal, light, and view vectors
	vec3 N = normalize(normalVector);
	vec3 V = normalize(viewPosition - fragmentPosition.xyz);

	vec3 Kd = diffuseColor;
	vec3 Ks = specularColor;

	// Calculate ambient specular and diffuse colors
	vec3 diffuse = (Kd / PI) * Irradiance(N);
	vec3 specular = SpecularIBL(N, V, Ks, shininess);

	vec3 BRDF = diffuse + specular;
	vec3 result = BRDF;

	// Convert back to sRGB color space
	vec3 eC = exposure*result;
	result = eC / (eC + vec3(1.0));
	result = pow(result, vec3(contrast/2.2));

	color = vec4(result, 1.0);
}