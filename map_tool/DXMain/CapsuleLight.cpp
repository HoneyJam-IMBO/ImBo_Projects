#include "stdafx.h"
#include "CapsuleLight.h"

bool CCapsuleLight::Begin() {
	m_lightid = light_id::LIGHT_CAPSULE;
//	m_objectID = object_id::OBJECT_CAPSULE_LIGHT;

	m_fHalfSegmentLen = m_CapsuleData.CapsuleLightLen*0.5f;
	m_fCapsuleLightRangeRcp = 1 / m_CapsuleData.CapsuleLightRange;

	////���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//SetComponent(pComponent);


	return CLight::Begin();
}
bool CCapsuleLight::End() {

	return CLight::End();
}

//instance buffer controll base
void CCapsuleLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {

	//����ȯ
	CAPSULE_LIGHT_DS_CB  *pDS_InstanceData = (CAPSULE_LIGHT_DS_CB*)ppMappedResources[0];
	CAPSULE_LIGHT_PS_CB  *pPS_InstanceData = (CAPSULE_LIGHT_PS_CB*)ppMappedResources[1];


	//light projection
	XMMATRIX xmmtxWorld = GetWorldMtx();
	XMMATRIX xmmtxView = pCamera->GetViewMtx();
	XMMATRIX xmmtxProjection = pCamera->GetProjectionMtx();

	XMMATRIX xmmtxLight = xmmtxWorld * xmmtxView * xmmtxProjection;

	pDS_InstanceData[nInstance].LightProjection = XMMatrixTranspose(xmmtxLight);
	pDS_InstanceData[nInstance].HalfSegmentLen = m_fHalfSegmentLen;
	pDS_InstanceData[nInstance].CapsuleLightRange = m_CapsuleData.CapsuleLightRange;
	//light projection

	//lightpos = capsule start point = pos - vDir * fLen*0.5
	XMVECTOR vDir = GetLook();
	XMVECTOR vPos = GetPosition();
	vPos = vPos - vDir*m_fHalfSegmentLen;
	XMStoreFloat3(&pPS_InstanceData[nInstance].CapsuleLightPos, vPos);
	pPS_InstanceData[nInstance].CapsuleLightLen = m_CapsuleData.CapsuleLightLen;
	pPS_InstanceData[nInstance].CapsuleLightColor = m_CapsuleData.CapsuleLightColor;
	pPS_InstanceData[nInstance].CapsuleLightRangeRcp = m_fCapsuleLightRangeRcp;
//	pPS_InstanceData[nInstance].CapsuleLightIntensity = m_CapsuleData.CapsuleLightIntensity;
	//Dir = Look
	XMStoreFloat3(&pPS_InstanceData[nInstance].CapsuleLightDir, vDir);

}


void CCapsuleLight::SetLength(float len){
	m_CapsuleData.CapsuleLightLen = len;
	m_fHalfSegmentLen = m_CapsuleData.CapsuleLightLen*0.5f;
}

void CCapsuleLight::SetRange(float outer, float inner){
	m_CapsuleData.CapsuleLightRange = MAX(outer, inner);
	m_fCapsuleLightRangeRcp = 1 / m_CapsuleData.CapsuleLightRange;
}

void CCapsuleLight::SetColor(float r, float g, float b){
	m_CapsuleData.CapsuleLightColor = XMFLOAT3(r, g, b);
}

XMFLOAT3 CCapsuleLight::GetColor(){

	return XMFLOAT3();
}

CCapsuleLight * CCapsuleLight::CreateCapsuleLight(float CapsuleLightLen, XMFLOAT3 CapsuleLightColor, float CapsuleLightRange){
	CCapsuleLight* pLight = new CCapsuleLight;
	CAPSULE_LIGHT data;
	data.CapsuleLightColor = CapsuleLightColor;
	data.CapsuleLightLen = CapsuleLightLen;
	data.CapsuleLightRange = CapsuleLightRange;
	pLight->SetCapsuleLightData(data);
	pLight->Begin();
	return pLight;
}
void TW_CALL SetCapsuleLightRange(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CCapsuleLight* plight = reinterpret_cast<CCapsuleLight*>(clientData);
	*static_cast<float *>(value) = plight->GetRange();
}

void TW_CALL GetCapsuleLightRange(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CCapsuleLight* plight = reinterpret_cast<CCapsuleLight*>(clientData);
	float* data = (float*)value;
	plight->SetRange(*data);
}
void TW_CALL SetCapsuleLightLength(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CCapsuleLight* plight = reinterpret_cast<CCapsuleLight*>(clientData);
	*static_cast<float *>(value) = plight->GetLength();
}

void TW_CALL GetCapsuleLightLength(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CCapsuleLight* plight = reinterpret_cast<CCapsuleLight*>(clientData);
	float* data = (float*)value;
	plight->SetLength(*data);
}
void CCapsuleLight::PickingProc(){
	CGameObject::PickingProc();

	//color
	TWBARMGR->AddColorBar3F("PickingBar", "Light", "Color", &m_CapsuleData.CapsuleLightColor);
	
	//range
	TWBARMGR->AddMinMaxBarCB("PickingBar", "Light", "Range", GetCapsuleLightRange, SetCapsuleLightRange, this,
		1.0f, 1000.f, 0.1f);
	TWBARMGR->AddMinMaxBarCB("PickingBar", "Light", "Length", GetCapsuleLightLength, SetCapsuleLightLength, this,
		1.0f, 1000.f, 0.1f);

}

void CCapsuleLight::SaveInfo(){
	CGameObject::SaveInfo();
	EXPORTER->WriteFloat(m_CapsuleData.CapsuleLightColor.x); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(m_CapsuleData.CapsuleLightColor.x); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(m_CapsuleData.CapsuleLightColor.x); EXPORTER->WriteSpace();
	EXPORTER->WriteEnter();

	EXPORTER->WriteFloat(m_CapsuleData.CapsuleLightLen); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(m_CapsuleData.CapsuleLightRange); EXPORTER->WriteSpace();
	EXPORTER->WriteEnter();
}

void CCapsuleLight::LoadInfo(){

}


CCapsuleLight::CCapsuleLight() : CLight("capsulelight") {
	
}
CCapsuleLight ::~CCapsuleLight() {

}