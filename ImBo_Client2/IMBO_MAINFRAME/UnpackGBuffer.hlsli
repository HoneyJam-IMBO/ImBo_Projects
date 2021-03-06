//specpowr 정규화 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };

cbuffer cbPsCameraStatic : register(b11) {
	float4x4 CameraProjectionMtx : packoffset(c0);
	float4 PerspectiveValues : packoffset(c4);
}
cbuffer cbPsCameraDynamic : register(b12) {
	float4x4 ViewInv : packoffset(c0);
	float4 EyePosition : packoffset(c4);
}

SamplerState        gssSamplerState		: register(s0);

Texture2D           gtxtDepthTexture	: register(t0);
Texture2D           gtxtColor 			: register(t1);
Texture2D           gtxtNormal_Depth	: register(t2);
Texture2D           gtxtSpecInt_Pow	 	: register(t3);

struct SURFACE_DATA {
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float3 SpecInt;
	float SpecPow;
	float depth;
};

//------------------hellper func---------------------

float ConvertDepthToLinear(float depth){
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
	return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth){
	float4 position;

	position.xy = csPos.xy * PerspectiveValues.xy * depth;
	position.z = depth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}
//------------------hellper func---------------------

//------------------unpack gbuffer-------------------
SURFACE_DATA UnpackGBuffer(float2 location) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	float3 location3 = float3(location, 0);

	//베이스 색상과 스펙큘러 세기 값 추출
	//float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	float4 baseColorSpecInt = gtxtColor.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	//Out.SpecInt = baseColorSpecInt.w;

	//노멀 샘플링 후 전체 범위 변환 및 정규화
	float4 normal_depth = gtxtNormal_Depth.Load(location3);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//깊이 값 추출 및 선형 깊이 값으로 변환
	Out.depth = gtxtDepthTexture.Load(location3).x;
	Out.LinearDepth = ConvertDepthToLinear(Out.depth);
	//Out.LinearDepth = depth;

	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
	float4 SpecPowerNorm = gtxtSpecInt_Pow.Load(location3);
	//Out.PositionW = PositionW_SpecPowerNorm.xyz;
	Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.w * g_SpecPowerRange.y;
	Out.SpecInt = SpecPowerNorm.xyz;
	return Out;
}
SURFACE_DATA UnpackGBuffer_Tex(float2 texCoord) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	//베이스 색상과 스펙큘러 세기 값 추출
	float4 baseColorSpecInt = gtxtColor.Sample(gssSamplerState, texCoord);
	//float4 baseColorSpecInt = gtxtColorSpecInt.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	//Out.SpecInt = baseColorSpecInt.w;

	//노멀 샘플링 후 전체 범위 변환 및 정규화
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
	//float4 normal_depth = gtxtNormal_Depth.Load(location3);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//깊이 값 추출 및 선형 깊이 값으로 변환
	Out.depth = gtxtDepthTexture.Sample(gssSamplerState, texCoord);
	//float depth = gtxtDepthTexture.Load(location3).x;
	Out.LinearDepth = ConvertDepthToLinear(Out.depth);
	//Out.LinearDepth = depth;

	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
	float4 SpecPowerNorm = gtxtSpecInt_Pow.Sample(gssSamplerState, texCoord);
	//float4 PositionW_SpecPowerNorm = gtxtSpecPow_Int.Load(location3);
	Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.w * g_SpecPowerRange.y;
	Out.SpecInt = SpecPowerNorm.xyz;

	return Out;
}
/*SURFACE_DATA UnpackGBuffer(float2 texCoord) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	//베이스 색상과 스펙큘러 세기 값 추출
	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecInt = baseColorSpecInt.w;

	//노멀 샘플링 후 전체 범위 변환 및 정규화
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//깊이 값 추출 및 선형 깊이 값으로 변환
	float depth = normal_depth.w;
	Out.LinearDepth = ConvertDepthToLinear(depth);

	//원래 범위 값에 대해 스펙큘러 파워 스케일 조정
	float SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;

	return Out;
}*/
//------------------unpack gbuffer-------------------
