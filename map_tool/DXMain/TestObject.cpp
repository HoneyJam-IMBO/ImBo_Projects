#include "stdafx.h"
#include "TestObject.h"


bool CTestObject::Begin() {
	//object_id set
	//m_objectID = object_id::OBJECT_FBX_ELF;
	//m_objectID = object_id::OBJECT_TEST;

	Rotate(-90.f, 0.f, 0.f);
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	CGameObject::Begin();
	return true;
}
bool CTestObject::End() {

	return CGameObject::End();
}
void CTestObject::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if( m_pTerrainContainer ) m_xmf4x4World._42 = GetTerrainHeight();
	
}
void CTestObject::Animate(float fTimeElapsed) {
	if (m_pAnimater)m_pAnimater->Update(fTimeElapsed);
	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
	//	m_pRenderContainer->GetMesh(i).get()->Update(fTimeElapsed);
	//}
}
CTestObject::CTestObject() : CGameObject("fbx", tag::TAG_DYNAMIC_OBJECT) {

}
CTestObject::~CTestObject() {

}