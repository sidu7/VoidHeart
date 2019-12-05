#version 330

uniform sampler2D finalImage;

uniform vec2 screenSize;

uniform int fxaaON;
uniform float fxaaSpan;

out vec4 color;

void main( void ) {
	vec2 texCoord = gl_FragCoord.xy / screenSize;
	
	if(fxaaON == 1) 
	{
        float FXAA_SPAN_MAX = fxaaSpan;
        float FXAA_REDUCE_MUL = 1.0/8.0;
        float FXAA_REDUCE_MIN = 1.0/128.0;

		// sample centre and pixels in the diagonal directions
        vec3 rgbNW=texture2D(finalImage,texCoord+(vec2(-1.0,-1.0)/screenSize)).xyz;
        vec3 rgbNE=texture2D(finalImage,texCoord+(vec2(1.0,-1.0)/screenSize)).xyz;
        vec3 rgbSW=texture2D(finalImage,texCoord+(vec2(-1.0,1.0)/screenSize)).xyz;
        vec3 rgbSE=texture2D(finalImage,texCoord+(vec2(1.0,1.0)/screenSize)).xyz;
        vec3 rgbM=texture2D(finalImage,texCoord).xyz;
        
		// grayscale constant is used to calculate a scalar luminance for shader calculations
		// (could have only used R & G channels but this is more accurate)
        vec3 luma=vec3(0.299, 0.587, 0.114);
        float lumaNW = dot(rgbNW, luma);
        float lumaNE = dot(rgbNE, luma);
        float lumaSW = dot(rgbSW, luma);
        float lumaSE = dot(rgbSE, luma);
        float lumaM  = dot(rgbM,  luma);
        
		// calculate min and max to obtain the range of luma
        float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
        float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
        
		// calculate direction perpendicular to luma gradient for sampling
        vec2 dir;
        dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
        dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	
		// calculate sampling step based on the brightness of the sampled points
		// (More bright points means smaller step size)
        float dirReduce = max(
                (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
                FXAA_REDUCE_MIN);
         
		// factor to normalize the sampling direction and include the brightness factor 
        float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
        
		// calculate final sampling direction by clamping between max span, including the
		// brightness factor and scaling to uv
        dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
                  max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                  dir * rcpDirMin)) / screenSize;
                
		// take the closer samples
        vec3 rgbA = 0.5 * (
                texture2D(finalImage, texCoord.xy + dir * (1.0/3.0 - 0.5)).xyz +
                texture2D(finalImage, texCoord.xy + dir * (2.0/3.0 - 0.5)).xyz);
		// take the farther samples
        vec3 rgbB = rgbA * 0.5 + 0.25 * (
                texture2D(finalImage, texCoord.xy + dir * (0.0/3.0 - 0.5)).xyz +
                texture2D(finalImage, texCoord.xy + dir * (3.0/3.0 - 0.5)).xyz);

		// calculate luma of farther sample for comparison
        float lumaB = dot(rgbB, luma);
		
		// if farther sample is out of the contrast range then use the inner sample
        if((lumaB < lumaMin) || (lumaB > lumaMax)){
                color.xyz=rgbA;
        }else{
                color.xyz=rgbB;
        }

			color.w =1.0;
	}

}