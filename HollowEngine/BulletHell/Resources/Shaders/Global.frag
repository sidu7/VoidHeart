#version 430

#define PI 3.14159265

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 bloom;

in vec2 texCoords;

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;

// Shadow maps
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
uniform int shadowMode;
uniform float alpha;
uniform float md;

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

vec3 Cholesky(float m11, float m12, float m13, float m22, float m23, float m33, float z1, float z2, float z3)
{
    float a = sqrt(m11);
    float b = m12 / a;
    float c = m13 / a;
    float d = sqrt(m22 - (b*b));
    float e = (m23 - b*c) / d;
    float f = sqrt(m33 - c*c - e*e);

    float c1p = z1 / a;
    float c2p = (z2 - b*c1p) / d;
    float c3p = (z3 - c*c1p - e*c2p) / f;

    float c3 = c3p / f;
    float c2 = (c2p - e*c3) / d;
    float c1 = (c1p - b*c2 - c*c3) / a;

    return vec3(c1,c2,c3);
}

// Distribution term for BRDF
float DistributionTerm(float a, float HN)
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
	float HN = max(dot(H, N), 0.0);

	vec3 Kd = diffuseColor;
	vec3 Ks = specularColor;

	// Calculate position in light space
	vec4 shadowCoord = shadowMatrix * fragmentPosition;

	// Put into NDC
	float G = 0.0;
	if(shadowCoord.w > 0)
	{
		vec2 uv = shadowCoord.xy/shadowCoord.w;
		if(uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
		{
			// Use texture coordinate to get shadow depth
			float shadowDepth = texture(shadowMap, uv).r;
			float pointDepth = shadowCoord.w;

			float shadow = 0.0;
			float epsilon = 0.0005;
			if(shadowDepth < (pointDepth - epsilon))
			{
				shadow = 1.0;
			}
	

			float zf = pointDepth;
			vec4 b = texture(shadowMap, uv);
			vec4 bP = (1.0 - alpha)*b + alpha*vec4(md, md, md, md);
			vec3 Z = vec3(1.0, zf, zf*zf);

			//vec3 c = MSMDecomposition(bP, Z);
			//1.0,bprime.x, bprime.y, bprime.y,prime.z,bprime.w,1.0,
			vec3 c = Cholesky(1.0, bP.x, bP.y, bP.y, bP.z, bP.w, 1.0, zf, zf*zf);

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

			//float G = 0.0;

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

			if(shadowMode == 1)
			{
				G = shadow;
			}
		}
	}

	// Calculate BRDF lighting
	vec3 diffuse = Kd / PI;
	vec3 specular = (DistributionTerm(shininess, HN)*GeometryTerm(L, V, H)*FresnelTerm(Ks, L, H))/4.0;
	vec3 BRDF = lightColor * NL * (diffuse + specular);
	vec3 result = BRDF;

	// Apply shadow
	result = (1.0 - G)*result;

	color = vec4(result, 1.0f);
	bloom = vec4(0.0, 0.0, 0.0, 0.0);

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