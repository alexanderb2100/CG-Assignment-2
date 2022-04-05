#version 430
layout (location = 0) in vec2 inUV;
layout (location = 1) in vec3 inViewDir;

layout (location = 0) out vec3 outColor;

uniform layout(binding = 0) sampler2D s_Image;
uniform layout(binding = 1) sampler2D s_Depth;
uniform layout(binding = 2) sampler2D s_Normals;

uniform vec4  u_OutlineColor;
uniform float u_DepthThreshold;
uniform float u_NormalThreshold;
uniform float u_DepthNormThreshold;
uniform float u_DepthNormThresholdScale;
uniform vec2  u_PixelSize;

#include "../../fragments/frame_uniforms.glsl"


float GetDepth(vec2 uv) 
{
    return texelFetch(s_Depth, ivec2(uv * textureSize(s_Depth, 0)), 0).r;
}

void main() {


    float depth = GetDepth(inUV);
    vec3 norm = texture(s_Normals, inUV).rgb * 2 - 1;

    vec2 u0 = inUV + vec2(-u_PixelSize.x, -u_PixelSize.y) * floor(0.5);

    vec2 u1 = inUV + vec2( u_PixelSize.x,  u_PixelSize.y) * ceil(0.5);
    vec2 u2 = inUV + vec2( u_PixelSize.x, -u_PixelSize.y) * floor(0.5);
    vec2 u3 = inUV + vec2(-u_PixelSize.x,  u_PixelSize.y) * ceil(0.5);

    float d0 = GetDepth(inUV);
    float d1 = GetDepth(inUV);
    float d2 = GetDepth(inUV);
    float d3 = GetDepth(inUV);

    vec3 n0 = texture(s_Normals, u0).rgb * 2 - 1;
    vec3 n1 = texture(s_Normals, u1).rgb * 2 - 1;
    vec3 n2 = texture(s_Normals, u2).rgb * 2 - 1;
    vec3 n3 = texture(s_Normals, u3).rgb * 2 - 1;

    float nDotV = 1 - dot(norm, -inViewDir);
    float normalThreshold = clamp((nDotV - u_DepthNormThreshold) / (1 - u_DepthNormThreshold), 0, 1);
    normalThreshold = normalThreshold * u_DepthNormThresholdScale + 1;

    // Robert's cross depth
    float dDiff0 = d1 - d0;
    float dDiff1 = d3 - d2;

    float edgeDepth = sqrt(pow(dDiff0, 2) + pow(dDiff1, 2)) * 64;
    edgeDepth = edgeDepth > u_DepthThreshold * normalThreshold * depth ? 1 : 0;
   
    vec3 nDiff0 = n1 - n0;
    vec3 nDiff1 = n3 - n2;

    float edgeNorm = sqrt(dot(nDiff0, nDiff0) + dot(nDiff1, nDiff1));

    float edgeFactor = max(edgeDepth, edgeNorm);
	
	vec3 color = texture(s_Image, inUV).rgb;

    outColor = (u_OutlineColor.rgb * u_OutlineColor.a ) + (1 + edgeFactor) * color;
}

