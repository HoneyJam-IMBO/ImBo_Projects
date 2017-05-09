
//#include "Directional_AmbientLight.hlsli"
#include "UnpackGBuffer.hlsli"
#include "CalcDirectionalLight.hlsli"
#include "CalcDirectionalShadow.hlsli"

Texture2D<float> AOTex : register(t4);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};


float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	SURFACE_DATA gbd = UnpackGBuffer_Tex(input.uv);

if (gbd.depth > 0.99999) {
	//return float4(CalcAmbient(float3(0, 1, 0), gbd.Color), 1.0);
	return float4(gbd.Color, 1.f);
}

//�����͸� ���� ����ü�� ��ȯ
Material mat;
mat.normal = gbd.Normal.xyz;
mat.diffuseColor.xyz = gbd.Color;
mat.diffuseColor.w = 1.0f;//���� ������

mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
mat.specIntensity = gbd.SpecInt;

//���� ��ġ ����
float2 cpPos = input.uv * float2(2.0, -2.0) - float2(1.0, -1.0);
float3 positionW = CalcWorldPos(cpPos, gbd.LinearDepth);
float ao = AOTex.Sample(gssCLAMP_LINEAR, input.uv);

//�����Ʈ �� �𷺼ų� ����Ʈ ���� ���
float4 finalColor;
finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz) * ao;
finalColor.xyz += CalcDirectional(positionW, mat);
//finalColor.xyz = mat.normal;

float3 dir = float3(0, 0, 0) - DirToLight;
float shadowAtt = ShaderPCF(positionW, dot(mat.normal, dir));
finalColor.rgb *= shadowAtt;
finalColor.a = 1.0f;
return finalColor;


//float depth = gbd.LinearDepth;
//float depth = ShadowMap.Sample(gssWRAP_LINEAR, input.uv); //shadowAtt;
//float depth = ConvertDepthToLinear(shadowAtt) * 0.0001;
//return float4(depth, depth, depth, 1.f);
//float depth = gbd.depth;
//float3 cameraPos = ClacCameraPos(cpPos, depth);
//float3 worldPos = CalcWorldPos(cpPos, depth);
//float linearDepth = gbd.LinearDepth;
//return float4(linearDepth, linearDepth, linearDepth, 1.f);
//return float4(cpPos, gbd.depth, 1.f);
//return float4(cameraPos, 1.f);
//return float4(positionW, 1.f);
//return finalColor;
}