#include "stdafx.h"
#include "PointLight.h"

bool CPointLight::Begin() {
	m_lightid = light_id::LIGHT_POINT;
//	m_objectID = object_id::OBJECT_POINT_LIGHT;

	m_xmmtxScale = XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(m_PointData.fRange, m_PointData.fRange, m_PointData.fRange, 1.0f)));
	m_fRangeRcp = 1 / m_PointData.fRange;

	return CLight::Begin();
}
bool CPointLight::End() {

	return CLight::End();
}

//instance buffer controll base
void CPointLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {

	//형변환
	POINT_LIGHT_DS_CB *pDS_InstanceData = (POINT_LIGHT_DS_CB*)ppMappedResources[0];
	POINT_LIGHT_PS_CB *pPS_InstanceData = (POINT_LIGHT_PS_CB*)ppMappedResources[1];
	
	//포인트 라이트 mtx 제작 이건 DS에서만 사용 한다!!!!! PS에 set하는 것이 아님
	//!!!
	//XMMATRIX xmmtxScale = XMMatrixScaling(m_fRange, m_fRange, m_fRange);
	
	XMMATRIX xmmtxWorld = XMMatrixTranslation(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMMATRIX xmmtxView = pCamera->GetViewMtx();
	XMMATRIX xmmtxProjection = pCamera->GetProjectionMtx();

	XMMATRIX xmmtxPoint = m_xmmtxScale * xmmtxWorld * xmmtxView * xmmtxProjection;
	//포인트 라이트 mtx 제작

	//포인트 mtx 설정!
	pDS_InstanceData[nInstance].m_PointLightMtx = XMMatrixTranspose(xmmtxPoint);

	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, GetPosition());
	//위치, 범위 설정!
	pPS_InstanceData[nInstance].m_PointLightPos_Range.x = xmf3Pos.x;
	pPS_InstanceData[nInstance].m_PointLightPos_Range.y = xmf3Pos.y;
	pPS_InstanceData[nInstance].m_PointLightPos_Range.z = xmf3Pos.z;
	pPS_InstanceData[nInstance].m_PointLightPos_Range.w = m_fRangeRcp;
	//color_Intensity 설정!
	pPS_InstanceData[nInstance].m_PointLightColor_Intensity.x = m_PointData.xmf3Color.x;
	pPS_InstanceData[nInstance].m_PointLightColor_Intensity.y = m_PointData.xmf3Color.y;
	pPS_InstanceData[nInstance].m_PointLightColor_Intensity.z = m_PointData.xmf3Color.z;

//	pPS_InstanceData[nInstance].m_PointLightColor_Intensity.w = m_PointData.fIntensity;
}

void CPointLight::SetLength(float len){
	m_PointData.fRange = len;
	m_xmmtxScale = XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(m_PointData.fRange, m_PointData.fRange, m_PointData.fRange, 1.0f)));
	m_fRangeRcp = 1 / m_PointData.fRange;
}

void CPointLight::SetRange(float outer, float inner){
	m_PointData.fRange = MAX(outer, inner);
	m_xmmtxScale = XMMatrixScalingFromVector(XMVECTOR(XMVectorSet(m_PointData.fRange, m_PointData.fRange, m_PointData.fRange, 1.0f)));
	m_fRangeRcp = 1 / m_PointData.fRange;
}

void CPointLight::SetColor(float r, float g, float b){
	m_PointData.xmf3Color = XMFLOAT3(r, g, b);
}

XMFLOAT3 CPointLight::GetColor()
{
	return XMFLOAT3();
}

CPointLight* CPointLight::CreatePointLight(float fRange, XMFLOAT3 xmf3Color){
	CPointLight* pLight = new CPointLight;
	POINT_LIGHT data;
	data.fRange = fRange;
	data.xmf3Color = xmf3Color;
	pLight->SetPointLightData(data);
	pLight->Begin();
	return pLight;
}

void TW_CALL SetPointLightRange(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CPointLight* plight = reinterpret_cast<CPointLight*>(clientData);
	*static_cast<float *>(value) = plight->GetRange();
}

void TW_CALL GetPointLightRange(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CPointLight* plight = reinterpret_cast<CPointLight*>(clientData);
	float* data = (float*)value;
	plight->SetRange(*data);
}

void CPointLight::PickingProc(){
	CGameObject::PickingProc();

	//color
	TWBARMGR->AddColorBar3F("PickingBar", "Light", "Color", &m_PointData.xmf3Color);
	//range
	TWBARMGR->AddMinMaxBarCB("PickingBar", "Light", "Range", GetPointLightRange, SetPointLightRange, this,
		1.0f, 1000.f, 0.1f);
}

void CPointLight::SaveInfo(){
	CGameObject::SaveInfo();

	EXPORTER->WriteFloat(m_PointData.fRange);
	EXPORTER->WriteEnter();

	EXPORTER->WriteFloat(m_PointData.xmf3Color.x); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(m_PointData.xmf3Color.y); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(m_PointData.xmf3Color.z); EXPORTER->WriteSpace();
	EXPORTER->WriteEnter();
}

void CPointLight::LoadInfo()
{
}



CPointLight::CPointLight() : CLight("pointlight") {
	
}
CPointLight::~CPointLight() {

}