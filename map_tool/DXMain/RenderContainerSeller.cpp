#include "stdafx.h"
#include "RenderContainerSeller.h"


bool CRenderContainerSeller::Begin(){
	string object_name;
	//test
	tag t = tag::TAG_DYNAMIC_OBJECT;
	object_name = "test";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Rect5", 0));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	//test

	object_name = "fbx";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("Rect1", 0));
#ifdef USE_ANIM
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
#else
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
#endif
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("FBX"));
	m_mTagRenderContainer[t][object_name]->Begin();


	object_name = "plane";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Plane", 0));//mesh set�� ���ÿ� instancing buffer ���� �� set
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Plane"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	t = tag::TAG_LIGHT;
	object_name = "directionallight";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set�� ���ÿ� instancing buffer ���� �� se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DirectionalLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("DirectionalLightCB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "pointlight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("PointLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	object_name = "spotlight";
	//spot light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("SpotLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "capsulelight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("CapsuleLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_POSTPROCESSING;
	object_name = "postprocessing";
	//�ڱ� shader set
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("PostProcessing"));
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_SPACE;
	object_name = "skybox";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SkyBox", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("SkyBox"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("SkyBox"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "space";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Space", 0));//mesh set�� ���ÿ� instancing buffer ���� �� set
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("SpaceIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	t = tag::TAG_TERRAIN;
	object_name = "terrain";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_REFLECTION;
	object_name = "water";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("Water", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Water"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	t = tag::TAG_SSRF;
	object_name = "SSRF";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set�� ���ÿ� instancing buffer ���� �� se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("SSRF"));
	m_mTagRenderContainer[t][object_name]->Begin();

	t = tag::TAG_SSLR;
	object_name = "laytrace";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set�� ���ÿ� instancing buffer ���� �� se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("RayTrace"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "combine";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set�� ���ÿ� instancing buffer ���� �� se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Combine"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_DEBUG;
	object_name = "aabb";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("BoundingBox", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("BoundingBox"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("BoundingBox"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("BoundingBoxIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "coordinatesys";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CoordinateSys", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("CoordinateSys"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("AABB"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("CoordinateSysIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugpointlight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugPointLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugcapsulelight";
	//capsule light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugCapsuleLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugspotlight";
	//spot light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugSpotLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugtexture";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DebugTexture", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugTexture"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DebugTextureIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugdepthtexture";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DebugTexture", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugDepthTexture"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DebugTextureIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	//�ڵ� rendercontainer����!
	CreateStempRenderContainer();

	return true;
}

bool CRenderContainerSeller::End(){
	//render container delete
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mTagRenderContainer.clear();

	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mStempRenderContainer.clear();
	//render container delete

	return true;
}

CRenderContainer* CRenderContainerSeller::GetRenderContainer(string name) {
	//������ �ٷ� return
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}
	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}
	return nullptr;
}

CRenderContainer * CRenderContainerSeller::GetRenderContainer(tag t, string name){
	//������ �ٷ� return
	if (m_mTagRenderContainer.end() != m_mTagRenderContainer.find(t)) {
		if (m_mTagRenderContainer[t].end() != m_mTagRenderContainer[t].find(name))
			return m_mTagRenderContainer[t][name];
	}
	if (m_mStempRenderContainer.end() != m_mStempRenderContainer.find(t)) {
		if (m_mStempRenderContainer[t].end() != m_mStempRenderContainer[t].find(name))
			return m_mStempRenderContainer[t][name];
	}
	return nullptr;
}
void CRenderContainerSeller::ClearStempRenderContainer(){
	{
		for (auto pairTagRenderContainer : m_mStempRenderContainer) {
			for (auto pairRenderContainer : pairTagRenderContainer.second) {
				delete pairRenderContainer.second;
			}
			pairTagRenderContainer.second.clear();
		}
		m_mStempRenderContainer.clear();
	}
}
void CRenderContainerSeller::CreateStempRenderContainer(){
	//tag�� mesh���� ���;� �Ѵ�. 
	//animation tool���� ���ο� ������ save����� �Ѵ�.
	
	for (auto vStempMesh : RESOURCEMGR->GetAllStempMesh()) {
		string name = vStempMesh.second[0]->GetName();
		tag t = vStempMesh.second[0]->GetTag();
		m_mStempRenderContainer[t][name] = new CRenderContainer;
		shared_ptr<CAnimater> pAnimater = RESOURCEMGR->GetAnimater(name);
		//animation
		if (pAnimater) {
			m_mStempRenderContainer[t][name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
			m_mStempRenderContainer[t][name]->SetAnimater(RESOURCEMGR->GetAnimater(name));
		}
		else {
			m_mStempRenderContainer[t][name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
		}
		//animation
		//mesh
		for (auto pStempMesh : vStempMesh.second) {
			m_mStempRenderContainer[t][name]->AddMesh(pStempMesh);
		}//mesh�� 1�� �̻��̸� mesh material, texture�� ����Ѵ�.
		if (vStempMesh.second.size() == 1) {
			//mesh�� 1����� 
			//mseh�� resource�� rc�� �ű�� 
			m_mStempRenderContainer[t][name]->AddMaterial(vStempMesh.second[0]->GetMeshMaterial());
			for (auto pTexture : vStempMesh.second[0]->GetvMeshTexture()) {
				m_mStempRenderContainer[t][name]->AddTexture(pTexture);
			}
			//�ڽ��� ���� resource�� �����ش�.
			vStempMesh.second[0]->ClearMeshResources();
		}
		//mesh
		m_mStempRenderContainer[t][name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	}
}
//
//void CRenderContainerSeller::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
//	static UINT currMesh = 0;
//	static char name[20];
//	switch (nMessageID)
//	{
//	case WM_KEYDOWN:
//
//		switch (wParam)
//		{
//		case VK_F5:
//			sprintf(name, "FBX_%d", ++currMesh);
//			m_mRenderContainer[OBJECT_FBX_ELF]->SetMesh(RESOURCEMGR->GetMesh(name));
//			break;
//		case VK_F6:
//			sprintf(name, "FBX_%d", --currMesh);
//			m_mRenderContainer[OBJECT_FBX_ELF]->SetMesh(RESOURCEMGR->GetMesh(name));
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//}
