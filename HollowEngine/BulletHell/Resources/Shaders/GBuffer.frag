#version 430

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gDiffuse;
layout (location = 3) out vec4 gSpecular;

in vec4 fragmentPosition;
in vec3 normalVector;
in vec2 texCoords;
in mat3 TBN;
in float cameraDepth;

uniform vec3 diffuseColor;
uniform sampler2D diffuseTexture;

uniform vec3 specularColor;
uniform sampler2D specularTexture;
uniform float shininess;

uniform int hasDiffuseTexture;
uniform int hasSpecularTexture;
uniform int hasNormalMap;
uniform int hasHeightMap;
uniform float heightScale;
in vec3 viewPosition;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;
uniform float alphaValue;

vec2 ParallaxMapping(vec2 uv, vec3 V)
{
	float height = texture(heightTexture, uv).r;
	return uv - V.xy * (height * heightScale);
}

vec2 ParallaxOcclusionMapping(vec2 uv, vec3 V)
{
	const float minLayers = 0;
	const float maxLayers = 32;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), V)));

	float layerDepth = 1.0 / numLayers;

	float currentLayerDepth = 0.0;

	vec2 P = V.xy / V.z * heightScale;
	vec2 deltaUV = P / numLayers;

	vec2 currentUV = uv;
	float currentDMV = texture(heightTexture, currentUV).r;

	while(currentLayerDepth < currentDMV)
	{
		currentUV -= deltaUV;
		currentDMV = texture(heightTexture, currentUV).r;
		currentLayerDepth += layerDepth;
	}

	vec2 prevUV = currentUV + deltaUV;

	float afterDepth = currentDMV - currentLayerDepth;
	float beforeDepth = texture(heightTexture, prevUV).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalUV = prevUV * weight + currentUV * (1.0 - weight);

	return finalUV;
}

void main()
{
	// Store the position in the G-Buffer
	gPosition = fragmentPosition;

	// Store the normal in the G-Buffer
	gNormal = vec4(normalize(normalVector), gl_FragDepth);

	// Assuming if a height map is present, a normal map is also present
	if(hasHeightMap == 1)
	{
		vec2 uv = texCoords;
		vec3 V = viewPosition;
		uv = ParallaxMapping(uv, V);
		//uv = ParallaxOcclusionMapping(uv, V);
		if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0)
			discard;

		vec3 normal = normalize(texture(normalTexture, uv).rgb * 2.0 - vec3(1.0));
		gNormal = vec4(normal, gl_FragDepth);
	}

	else if(hasNormalMap == 1)
	{
		vec3 normal = texture(normalTexture, texCoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(TBN * normal);
		gNormal = vec4(normal, gl_FragDepth);
	}
	
	// Store diffuse color in the G-Buffer
	gDiffuse = (diffuseColor + hasDiffuseTexture*texture(diffuseTexture, texCoords).rgb) * alphaValue;

	// Store specular color in the G-Buffer
	gSpecular = vec4(specularColor, shininess) + vec4(hasSpecularTexture*texture(specularTexture, texCoords).rgb, 0.0);
}