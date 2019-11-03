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
uniform vec3 lightPosition;

// Shadow maps
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
uniform float alpha;
uniform float md;

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

uniform int displayMode;

// Solves Ax = b for MSM
vec3 MSMDecomposition(vec4 A, vec3 b)
{
	float X = A.x; float Y = A.y; float Z = A.z; float W = A.w;
	float d = b.y; float d2 = b.z;
	
	// Variable soup, from lower triangular matrix L in Cholesky decomposition
	float L11 = sqrt(Y - X*X);
	float L21 = (Z - Y*X)/L11;
	float L22 = sqrt(W - (Y*Y + L21*L21));

	float c2 = ((d2 - Y - (L21)*((d - X)/L11))) / (L22 * L22);

	float c1 = (((d - X)/L11) - (L21*c2))/L11;

	float c0 = 1.0 - X*c1 - Y*c2;
	return vec3(c0, c1, c2);
}

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
	vec3 L = normalize(lightPosition - fragmentPosition.xyz);
	vec3 V = normalize(viewPosition - fragmentPosition.xyz);
	vec3 H = normalize(L+V);

	// Calculate clamped dot products
	float NL = max(dot(N, L), 0.0);
	float NV = max(dot(N, V), 0.0);
	float HN = max(dot(H, N), 0.0);

	vec3 Ka = vec3(0.2);
	vec3 Kd = diffuseColor;
	vec3 Ks = specularColor;

	// Calculate ambient specular and diffuse colors
	//vec3 ambient = Ka*Kd;
	//vec3 diffuse = Kd*NL;
	//vec3 specular = Ks*pow(HN, shininess);

	// Result for phong lighting
	//vec3 result = diffuse + ambient + specular;

	// Calculate position in light space
	vec4 shadowCoord = shadowMatrix * fragmentPosition;

	// Put into NDC
	shadowCoord /= shadowCoord.w;

	// Get texture coordinate 0 .. 1, apply bias
	vec2 uv = shadowCoord.xy*vec2(0.5) + vec2(0.5);

	// Use texture coordinate to get shadow depth
	float shadowDepth = texture(shadowMap, uv).r;
	float pointDepth = shadowCoord.z;

	float shadow = 0.0;
	float epsilon = 0.00005;
	if(shadowDepth < (pointDepth - epsilon) && (shadowCoord.w > 0) )
	{
		shadow = 1.0;
	}

	float zf = pointDepth;
	vec4 b = texture(shadowMap, uv);
	vec4 bP = (1.0 - alpha)*b + alpha*vec4(md, md, md, md);
	vec3 Z = vec3(1.0, zf, zf*zf);

	vec3 c = MSMDecomposition(bP, Z);

	float c1 = c.x;
	float c2 = c.y;
	float c3 = c.z;

	float dis = c2*c2-4.0*c3*c1;
	if(dis < 0.0)
	{
		dis = 0.0;
	}

	float z2 = (-c2 - sqrt(dis))/(2.0*c3);
	float z3 = (-c2 + sqrt(dis))/(2.0*c3);

	if(z2 > z3)
	{
		float t = z3;
		z3 = z2;
		z2 = t;
	}

	float G = 0.0;
	//zf = 0.0f;

	if(zf <= z2)
	{
		G = 0.0;
	}
	else if(zf <= z3)
	{
		G = (zf*z3 - bP.x*(zf+z3)+bP.y)/((z3-z2)*(zf-z2));
	}
	else
	{
		G = 1.0 - ((z2*z3 - bP.x*(z2+z3)+bP.y)/((zf-z2)*(zf-z3)));
	}

	//G = shadow;

	//result = (1.0 - G)*result;

	// diffuse = Kd / PI;
	//vec3 result = diffuse + (DistributionTerm(shininess, H, HN)*GeometryTerm(L, V, H)*FresnelTerm(Ks, L, H))/4.0;
	//result = (1.0 - G)*result;

	vec3 diffuse = (Kd / PI) * Irradiance(N);
	vec3 specular = SpecularIBL(N, V, Ks, shininess);
	vec3 BRDF = diffuse + specular;
	vec3 result = BRDF;

	// Convert back to sRGB color space
	vec3 eC = exposure*result;
	result = eC / (eC + vec3(1.0));
	result = pow(result, vec3(contrast/2.2));

	// Apply shadow
	result = (1.0 - G)*result;

	color = vec4(result, 1.0);

	if(displayMode == 1)
	{
		color = vec4(fragmentPosition.xyz, 1.0);
	}
	else if(displayMode == 2)
	{
		color = vec4(normalVector, 1.0);
	}
	else if(displayMode == 3)
	{
		color = vec4(diffuseColor, 1.0);
	}
	else if(displayMode == 4)
	{
		color = vec4(specularColor, 1.0);
	}
}