#include "stdafx.h"
#include "SceneMain.h"

//��ü ȸ�� ���� ����
float gfXDegree{ 1 };
float gfYDegree{ 1 };
float gfZDegree{ 1 };
//zxc
void TW_CALL BaseTextureSelectCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
	//	string s = pLFS->Filename;
	//	string path{ "../inputdata/" };

	wstring ws = { L"" };
	ws.assign(pLFS->Filename.cbegin(), pLFS->Filename.cend());
	UPDATER->GetTerrainContainer()->SetBaseTexture(ws);
	const char* barName{ "LoadFile" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}

void TW_CALL LoadFileCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
//	string s = pLFS->Filename;
//	string path{ "../inputdata/" };

	pLFS->m_pScene->CreateControllObject(pLFS->Filename);
	const char* barName{ "LoadFile" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}
void TW_CALL LoadFileButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->CreateLoadFileUI();
}
void TW_CALL AddInfoCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
//	string s = pLFS->Filename;
//	string path{ "../inputdata/" };

	pLFS->m_pScene->AddFBXAnimationInfo(pLFS->Filename);
	
	const char* barName{ "AddInfo" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}
void TW_CALL AddInfoButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->CreateAddInfoUI();
}
void TW_CALL ClearAllButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->ClearAllFBXObject();
}
void TW_CALL WriteNowButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	EXPORTER->Begin(L"../outputdata/text.txt");
	EXPORTER->ExportFbxObject(pScene->GetFBXObject());
	EXPORTER->End();
}
//==============================================================================

void TW_CALL TMTerrainButtonCallback(void * clientData) {
	GLOBALVALUEMGR->SetToolMode(TOOL_MODE_TERRAIN);
}
void TW_CALL TMSplattingButtonCallback(void * clientData) {
	GLOBALVALUEMGR->SetToolMode(TOOL_MODE_SPLATTING);
}
void TW_CALL TMObjectPositioningButtonCallback(void * clientData) {
	GLOBALVALUEMGR->SetToolMode(TOOL_MODE_OBJECTPOSITIONING);
	CSceneMain* pData = (CSceneMain*)clientData;
	pData->CreateObjectPositioningUI();
	//object positioning ui����

}
void TW_CALL TMFreeCameraButtonCallback(void * clientData) {
	GLOBALVALUEMGR->SetToolMode(TOOL_MODE_FREECAMERA);
}
void TW_CALL TMCharacterButtonCallback(void * clientData) {
	GLOBALVALUEMGR->SetToolMode(TOOL_MODE_CHARACTER);
}
void TW_CALL MCBaseTextureSelectButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	//ui�� �ϳ� ���� ���� texture�� terrain rendercontainer��
	pData->CreateBaseTextureSelectUI();
}

void TW_CALL OPCreatePositioningObjectButtonCallback(void * clientData) {
	CGameObject* pObject = (CGameObject*)clientData;
	GLOBALVALUEMGR->SetPositioningObject(pObject);
}
//void TW_CALL SetTerrainOnOffCallback(const void * value, void * clientData) {
//	if (nullptr == clientData) return;
//	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
//	bool val = (*static_cast<const bool *>(value));
//	if (val) {
//		pScene->CreateTerrainContainer();
//	}
//	else {
//		pScene->DeleteTerrainContainer();
//	}
//}

void TW_CALL GetTerrainOnOffCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	*static_cast<bool *>(value) = (nullptr != UPDATER->GetTerrainContainer());
}
//void TW_CALL SetSkyBoxOnOffCallback(const void * value, void * clientData) {
//	if (nullptr == clientData) return;
//	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
//	bool val = (*static_cast<const bool *>(value));
//	if (val) {
//		pScene->CreateSkyBoxContainer();
//	}
//	else {
//		pScene->DeleteSkyBoxContainer();
//	}
//}

void TW_CALL GetSkyBoxOnOffCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	*static_cast<bool *>(value) = (nullptr != UPDATER->GetSkyBoxContainer());
}


void TW_CALL MCTerrainHeightScaleGetCallback(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CSceneMain* pData = reinterpret_cast<CSceneMain*>(clientData);
	if(UPDATER->GetTerrainContainer())
		*static_cast<float *>(value) = UPDATER->GetTerrainContainer()->GetHeightScale();
}
void TW_CALL MCTerrainHeightScaleSetCallback(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CSceneMain* pData = reinterpret_cast<CSceneMain*>(clientData);
	if (UPDATER->GetTerrainContainer())
		UPDATER->GetTerrainContainer()->SetHeightScale(*static_cast<const float *>(value));
}
void TW_CALL MCCreateSplattingButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	if (UPDATER->GetTerrainContainer()) {
		UPDATER->GetTerrainContainer()->CreateSplattingInfo();
	}
}
void TW_CALL SMSetButtonCallback(void * clientData) {
	CSceneMain*  pData = (CSceneMain*)clientData;
	if (UPDATER->GetTerrainContainer())
		UPDATER->GetTerrainContainer()->SetStempMode(STEMP_MODE_SET);
}
void TW_CALL SMIndeButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	if (UPDATER->GetTerrainContainer())
		UPDATER->GetTerrainContainer()->SetStempMode(STEMP_MODE_INDE);
}
void TW_CALL SMBrushButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	if (UPDATER->GetTerrainContainer())
		UPDATER->GetTerrainContainer()->SetStempMode(STEMP_MODE_BRUSH);
}
void TW_CALL FCSaveButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	wcout << L"if you want save input the path, sceneName : " << endl;
	wcout << L"SceneName : ";
	char c[128];
	cin >> c;
	string sSceneName{ c };
	pData->SaveScene(sSceneName);
}
void TW_CALL FCOverWriteButtonCallback(void* clientData) {
	if (GLOBALVALUEMGR->GetLoad()) {
		CSceneMain* pData = (CSceneMain*)clientData;
		pData->SaveScene(pData->GetSceneName());
	}
}
void TW_CALL FCSetMeshRootButtonCallback(void* clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	wcout << L"input the path : " << endl;
	wcout << L"Mesh Root Path: ";
	WCHAR wc[128];
	wcin >> wc;

	pData->SetMeshRoot(wc);
}

void TW_CALL FCLoadButtonCallback(void * clientData) {
	CSceneMain* pData = (CSceneMain*)clientData;
	pData->CreateSceneListUI();
}
void TW_CALL LoadSceneButtonCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
	
	//pLFS->m_pScene->End();
	pLFS->m_pScene->LoadScene(pLFS->Filename);

	const char* barName{ "LoadScene" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}

void CSceneMain::CreatePositioningObject() {
	CPointLight *pPositioningPointLight = CPointLight::CreatePointLight(100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5));
	m_vpObjectList.push_back(pPositioningPointLight);

	CCapsuleLight* pPositioningCapsuleLight = CCapsuleLight::CreateCapsuleLight(100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5), 50.f);
	m_vpObjectList.push_back(pPositioningCapsuleLight);

	CSpotLight* pPositioningSpotLight = CSpotLight::CreateSpotLight(500.f, XMFLOAT3(0.1f, 0.1f, 0.1f), 30.f, 15.f);
	m_vpObjectList.push_back(pPositioningSpotLight);

	CGameObject* pWater = new CGameObject("water", tag::TAG_REFLECTION);
	pWater->Begin();
	m_vpObjectList.push_back(pWater);


	HPEN p;
}
void CSceneMain::CreatePositioningStempObject(){
	for (auto vStempMesh : RESOURCEMGR->GetAllStempMesh()) {
		string name = vStempMesh.second[0]->GetName();
		tag t = vStempMesh.second[0]->GetTag();
		bool bAnimation = true;// vStempMesh.second[0]->GetbAnimation();
							   //animation ������ ������ setting������ ��ü�̴�.
		CGameObject* pObject = new CGameObject(name, t);
		pObject->Begin();
		m_vpObjectList.push_back(pObject);
	}
}
void CSceneMain::SetMeshRoot(wstring wsMeshRoot){
	CScene::LoadResource(wsMeshRoot);

	CreatePositioningStempObject();
	CreateObjectPositioningUI();
}
void CSceneMain::SaveScene(string sceneName){
	
	wstring wsSceneName;
	wsSceneName.assign(sceneName.begin(), sceneName.end());
	WCHAR wc[256];
	wsprintf(wc, L"../../Assets/SceneResource/%s/", wsSceneName.c_str());
	wstring wsOutputPath{(WCHAR*)wc};
	
	//cin>>scene name;
	//cin>>path
	//scene name
	//wstring wsSceneName = L"test";


	//EXPORTER->Begin(L"../outputdata/scene.txt");
	wstring wsFileName = wsSceneName + L".scn";
	EXPORTER->Begin(wsOutputPath + wsFileName);

	//output path
	EXPORTER->WriteWstring(wsOutputPath);
	EXPORTER->WriteEnter();
	//scene name
	EXPORTER->WriteWstring(wsSceneName);
	EXPORTER->WriteEnter();

	////resource root path
	//EXPORTER->WriteWstring(m_wsMeshRoot);
	//EXPORTER->WriteEnter();

	//space info
	UPDATER->SaveSpaceInfo();
	UPDATER->SaveTerrainInfo(wsOutputPath, wsSceneName);
	UPDATER->SaveObjectsInfo();


	//effect info
	RENDERER->SaveEffectInfo(wsOutputPath, wsSceneName);
	

	EXPORTER->End();
	/*
		3. height map image / /[scene name] + height map
		4. normal map image / /[scene name] + normal map
		5. splatting blending images [scene name] + [splatting blending info] + [index]
	*/
	//EXPORTER->MakeBitmap24();
}
void CSceneMain::LoadScene(string path){
	CScene::LoadScene(path);
}
void CSceneMain::CreateSceneListUI(){
	m_LoadFileStruct.clear();

	const char* barName{ "LoadScene" };
	TWBARMGR->AddBar(barName);

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".scn");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".SCN");
	
	//const char* groupName = "File";
	char menuName[256];
	int cnt{ 0 };
	m_LoadFileStruct.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_LoadFileStruct[cnt] = LoadFileStruct{ this, filsDirectory };

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());
		sprintf(menuName, "%s", menuNameString.c_str());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);

		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str(), menuName, LoadSceneButtonCallback, &m_LoadFileStruct[cnt]);
		cnt++;
	}
}

void CSceneMain::LoadResource(wstring wsMeshRoot){
	CScene::LoadResource(wsMeshRoot);

	CreatePositioningStempObject();
	//CreateObjectPositioningUI();
}

bool CSceneMain::Begin() {
	//��� positioning�� ������ ��ü�� �̸� ������ �д�.
	CreatePositioningObject();

	const char* barName{ "TOOL_MODE" };
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 300);
	TWBARMGR->SetBarPosition(barName, 0, 0);
	TWBARMGR->SetBarColor(barName, 255, 0, 255);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	
	TWBARMGR->AddButtonCB(barName, "TOOL_MODE", "Terrain", TMTerrainButtonCallback, nullptr);
	TWBARMGR->AddButtonCB(barName, "TOOL_MODE", "Splatting", TMSplattingButtonCallback, nullptr);
	TWBARMGR->AddButtonCB(barName, "TOOL_MODE", "ObjectPositioning", TMObjectPositioningButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "TOOL_MODE", "FreeCamera", TMFreeCameraButtonCallback, nullptr);
	TWBARMGR->AddButtonCB(barName, "TOOL_MODE", "Character", TMCharacterButtonCallback, nullptr);

	TWBARMGR->AddButtonCB(barName, "StempControll", "SetStemp", SMSetButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "StempControll", "IndeStemp", SMIndeButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "StempControll", "BrushStemp", SMBrushButtonCallback, this);

	TWBARMGR->AddMinMaxBarRW(barName, "ObjectControll", "XDegree", &gfXDegree,
		0.0f, 360.f, 1.f);
	TWBARMGR->AddMinMaxBarRW(barName, "ObjectControll", "YDegree", &gfYDegree,
		0.0f, 360.f, 1.f);
	TWBARMGR->AddMinMaxBarRW(barName, "ObjectControll", "ZDegree", &gfZDegree,
		0.0f, 360.f, 1.f);

	TWBARMGR->AddMinMaxBarCB(barName, "TerrainControll", "TerrainHeightScale", MCTerrainHeightScaleSetCallback, MCTerrainHeightScaleGetCallback,
		this, 0.1f, 1000.f, 1.f);
	TWBARMGR->AddButtonCB(barName, "TerrainControll", "CreateSplatting", MCCreateSplattingButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "TerrainControll", "BaseTextureSelect", MCBaseTextureSelectButtonCallback, this);
	
	TWBARMGR->AddButtonCB(barName, "FileControll", "Set_Mesh_Root", FCSetMeshRootButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "FileControll", "Save", FCSaveButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "FileControll", "OverWrite", FCOverWriteButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "FileControll", "Load", FCLoadButtonCallback, this);


	//test
	//TWBARMGR->AddBoolBarCB(barName, "Test", "TerrainOnOff", SetTerrainOnOffCallback, GetTerrainOnOffCallback, this);
	//test

	//m_pPlayer = new CPlayer;
	//m_pPlayer->Begin();
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	UPDATER->SetCamera(m_pCamera);
	//----------------------------------camera-------------------------------------

	//CreateSceneContainers();
	

	//--------------------------------��ü ����------------------------
	int nMaxObjects = 1;
	float space_size = static_cast<float>(UPDATER->GetSpaceContainer()->GetSpaceSize());

	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		CTestCube* pCube = new CTestCube();
	//		pCube->Begin();
	//		pCube->SetTerrainContainer(m_pTerrainContainer);
	//		pCube->SetPosition(XMVectorSet(i * 100, 100, j * 100, 0.f));
	//		m_pSpaceContainer->AddObject(pCube);
	//	}
	//}

	//m_pRotationTestObject = new CTestCube();
	//m_pRotationTestObject->Begin();
	//m_pRotationTestObject->SetTerrainContainer(m_pTerrainContainer);
	//m_pRotationTestObject->SetPosition(XMLoadFloat3(&XMFLOAT3(0, 0, 0)));
	//m_pSpaceContainer->AddObject(m_pRotationTestObject);

	//fbx mesh
	//CTestObject* pObject = nullptr;
	
	//CBunny* pBunny = new CBunny();
	//pBunny->Begin();
	////pBunny->SetTerrainContainer(m_pTerrainContainer);
	//pBunny->SetPosition(XMLoadFloat3(&XMFLOAT3(SPACE_SIZE / 2.f, 0, SPACE_SIZE / 2.f)));
	//m_pSpaceContainer->AddObject(pBunny);
	//fbx mesh

	//CPointLight* pPointLight = nullptr;
	//int nPointLight = 10;
	////int nPointLight = 0;
	//float fStartX = 0;
	//float fStartZ = 0;
	//pPointLight = new CPointLight;
	//pPointLight->Begin(POINT_LIGHT{ 100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5) });
	//pPointLight->SetTerrainContainer(m_pTerrainContainer);
	//pPointLight->SetPosition(XMVectorSet(0, 100, 0, 0.f));
	//m_pSpaceContainer->AddObject(pPointLight);
	//
	//for (int i = 0; i < nPointLight; ++i) {
	//		pPointLight = new CPointLight;
	//		pPointLight->Begin(POINT_LIGHT{ 100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5) });
	//		pPointLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//		pPointLight->SetPosition(XMVectorSet(rand() % space_size, 100, rand() % space_size, 0.f));
	//		m_pSpaceContainer->AddObject(pPointLight);
	//}
	//
	//fStartX += 500;
	//fStartZ += 500;
	//CCapsuleLight* pCapsuleLight = nullptr;
	//int nCapsuleLight = 10;
	//
	//for (int i = 0; i < nCapsuleLight; ++i) {
	//	pCapsuleLight = new CCapsuleLight;
	//	pCapsuleLight->Begin(CAPSULE_LIGHT{ 100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5), 50.f});
	//	pCapsuleLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//	pCapsuleLight->SetPosition(XMVectorSet(rand() % space_size, rand() % 50 + 100, rand() % space_size, 0.f));
	//	m_pSpaceContainer->AddObject(pCapsuleLight);
	//}
	//
	//fStartX += 500;
	//fStartZ += 500;
	//
	//int nSpotLight = 10;
	//CSpotLight* pSpotLight = nullptr;
	//for (int i = 0; i < nSpotLight; ++i) {
	//		pSpotLight = new CSpotLight;
	//		pSpotLight->Begin(SPOT_LIGHT{ 100.f, XMFLOAT3(rand() % 100, rand() % 100, rand() % 100), 50.f, 50.f });
	//		pSpotLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//		pSpotLight->SetPosition(XMVectorSet(rand() % space_size, rand()%50+100, rand() % space_size, 0.f));
	//		m_pSpaceContainer->AddObject(pSpotLight);
	//}

	//--------------------------���� ��ü ����-------------------------

	/*
	XMFLOAT4 m_DirToLight;
	XMFLOAT4 m_DirLightColor;
	XMFLOAT4 m_DirLightPower;

	XMFLOAT4 m_AmbientDown;
	XMFLOAT4 m_AmbientRange;
	XMFLOAT4 m_AmbientUp;
	XMFLOAT4 m_AmbientColor;
	*/
	//m_pDirectionalLight->SetRenderContainer(m_RenderContainerSeller);
	//directional light
	//--------------------------���� ��ü ����-------------------------
	//--------------------------------��ü ����-------------------------
	//m_space_lv = 3;
	//
	//ChangeSceneContainers();
	return CScene::Begin();
}

bool CSceneMain::End() {
	for (auto pObject : m_vpObjectList) {
		delete pObject;
	}
	m_vpObjectList.clear();

	for (auto pObject : m_vpStempObjectList) {
		delete pObject;
	}
	m_vpStempObjectList.clear();
	//m_pPlayer->End();
	//seller
	//m_RenderContainerSeller->End();
	
	

	//ī�޶�� Framework�� �����ϴ� ���̱⿡ End()�۾��� �������� �ʴ´�.
	return CScene::End();
}

void CSceneMain::Animate(float fTimeElapsed) {

	DEBUGER->AddText(10, 350, 100, YT_Color(255, 255, 0, 255), L"mode : t - 1, s - 2, o - 3, f - 4");

	CScene::Animate(fTimeElapsed);

	//--------------------------���� ��ü animate / regist-------------------------
	UPDATER->SetCamera(m_pCamera);
	//UPDATER->GetSpaceContainer()->SetSpaceSize(m_space_size);
	//UPDATER->GetSpaceContainer()->SetSpaceLevel(m_space_lv);

	if (INPUTMGR->KeyDown(VK_1)) {
		GLOBALVALUEMGR->SetToolMode(TOOL_MODE_TERRAIN);
	}
	else if (INPUTMGR->KeyDown(VK_2)) {
		GLOBALVALUEMGR->SetToolMode(TOOL_MODE_SPLATTING);
	}
	else if (INPUTMGR->KeyDown(VK_3)) {
		GLOBALVALUEMGR->SetToolMode(TOOL_MODE_OBJECTPOSITIONING);
	}
	else if (INPUTMGR->KeyDown(VK_4)) {
		GLOBALVALUEMGR->SetToolMode(TOOL_MODE_FREECAMERA);
	}
	else if (INPUTMGR->KeyDown(VK_5)) {
		GLOBALVALUEMGR->SetToolMode(TOOL_MODE_CHARACTER);
	}
	else if (INPUTMGR->KeyDown(VK_6)) {
		UPDATER->GetDirectionalLight()->PickingProc();
	}
	if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_TERRAIN || GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_SPLATTING) {
		if (INPUTMGR->OnlyKeyDown(VK_F5_)) {
			float x = (UPDATER->GetTerrainContainer()->GetHeightScale() + 1.f);
			UPDATER->GetTerrainContainer()->SetHeightScale(x);
			if (x < 1000)
				UPDATER->GetTerrainContainer()->SetHeightScale(x);
		}
		else if (INPUTMGR->OnlyKeyDown(VK_F6_)) {
			float x = (UPDATER->GetTerrainContainer()->GetHeightScale() - 1.f);
			if (x > 0)
				UPDATER->GetTerrainContainer()->SetHeightScale(x);
		}
	}
	//object positioning ��ü ����
	if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_OBJECTPOSITIONING) {
		
		if (GLOBALVALUEMGR->GetPositioningObject()) {
			if (UPDATER->GetTerrainContainer()) {
				XMFLOAT2 xmf2CurPicPos = UPDATER->GetTerrainContainer()->GetCurPickPos();
				float fHeight = UPDATER->GetTerrainContainer()->GetHeight(xmf2CurPicPos);

				GLOBALVALUEMGR->GetPositioningObject()->SetPosition(XMVectorSet(xmf2CurPicPos.x, fHeight, xmf2CurPicPos.y, 1.0));
				GLOBALVALUEMGR->GetPositioningObject()->RegistToContainer();
			}
			else {
				GLOBALVALUEMGR->GetPositioningObject()->SetPosition(XMVectorSet(UPDATER->GetSpaceContainer()->GetSpaceSize()/2.f, 0, UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f, 1.0));
				GLOBALVALUEMGR->GetPositioningObject()->RegistToContainer();
			}
		}
	}
	//--------------------------���� ��ü animate / regist-------------------------
}


void CSceneMain::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:

		POINT p = INPUTMGR->GetMousePoint();
		m_pPickingObject = PickObjectPointedByCursor(p.x, p.y);
		if (m_pPickingObject) {
			m_pPickingObject->PickingProc();
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		

	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
void CSceneMain::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	
	//RCSELLER->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_KEYUP:
		
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		case VK_P:

		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CSceneMain::ProcessInput(float fTimeElapsed) {

	if (GLOBALVALUEMGR->GetPositioningObject()){
		if (INPUTMGR->OnlyKeyDown(VK_Z)) {
			float radian = gfXDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetRight(), radian));
		}else if (INPUTMGR->OnlyKeyDown(VK_X)) {
			float radian = gfYDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetUp(), radian));
		}else if (INPUTMGR->OnlyKeyDown(VK_C)) {
			float radian = gfZDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetLook(), radian));
		}if (INPUTMGR->OnlyKeyDown(VK_A)) {
			float radian = gfXDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetRight(), -radian));
		}
		else if (INPUTMGR->OnlyKeyDown(VK_S)) {
			float radian = gfYDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetUp(), -radian));
		}
		else if (INPUTMGR->OnlyKeyDown(VK_D)) {
			float radian = gfZDegree * (1.f / 180.f) * XM_PI;
			GLOBALVALUEMGR->GetPositioningObject()->Rotate(XMMatrixRotationAxis(GLOBALVALUEMGR->GetPositioningObject()->GetLook(), -radian));
		}
		else if (INPUTMGR->KeyDown(VK_SPACE_)) {
			GLOBALVALUEMGR->GetPositioningObject()->SetWorldMtx(XMMatrixIdentity());
		}
	}
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
	if(GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_FREECAMERA)
		m_pCamera->ProcessInput(fTimeElapsed);

	if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_OBJECTPOSITIONING) {
		if (INPUTMGR->MouseLeftOnlyDown()) {
			if (GLOBALVALUEMGR->GetPositioningObject()) {
				ObjectPositioning();
			}
		}
		if (INPUTMGR->OnlyKeyDown(VK_F3_)) {
			if (GLOBALVALUEMGR->GetPositioningObject()) {
				XMVECTOR xmvScale = GLOBALVALUEMGR->GetPositioningObject()->GetScale();
				xmvScale += XMVectorSet(0.1, 0.1, 0.1, 0);
				GLOBALVALUEMGR->GetPositioningObject()->SetScale(xmvScale);
			}
		}
		else if (INPUTMGR->OnlyKeyDown(VK_F4_)) {
			if (GLOBALVALUEMGR->GetPositioningObject()) {
				XMVECTOR xmvScale = GLOBALVALUEMGR->GetPositioningObject()->GetScale();
				xmvScale -= XMVectorSet(0.1, 0.1, 0.1, 0);
				GLOBALVALUEMGR->GetPositioningObject()->SetScale(xmvScale);
			}
		}
	}
	if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_FREECAMERA) {
		if (INPUTMGR->OnlyKeyDown(VK_F3_)) {
			if (m_pPickingObject) {
				XMVECTOR xmvScale = m_pPickingObject->GetScale();
				xmvScale += XMVectorSet(0.1, 0.1, 0.1, 0);
				m_pPickingObject->SetScale(xmvScale);
			}
		}
		else if (INPUTMGR->OnlyKeyDown(VK_F4_)) {
			if (m_pPickingObject) {
				XMVECTOR xmvScale = m_pPickingObject->GetScale();
				xmvScale -= XMVectorSet(0.1, 0.1, 0.1, 0);
				m_pPickingObject->SetScale(xmvScale);
			}
		}
	}
}


CGameObject* CSceneMain::PickObjectPointedByCursor(int xClient, int yClient){

	if (!m_pCamera) return(NULL);
	if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_FREECAMERA) {
		//Get screen pos -> Camera pos
		XMFLOAT4X4 xmf4x4Projection;
		XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
		D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

		//�� �̰� ȭ���� ���� ���� ī�޶� ��ǥ���� �༮�̰�
		XMFLOAT3 xmf3PickPosition;
		xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
		xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
		xmf3PickPosition.z = 1.0f;

		XMVECTOR xmvPickPosition;
		xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
		XMMATRIX xmMtxViewInverse;
		xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
		//picking pos�� camera inverse�� �������� �̰� picking pos�� world pos!
		xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
		XMVECTOR xmvRayDir = xmvPickPosition - m_pCamera->GetPosition();

		CGameObject* pNearestObject = NULL;
		float fHitDistance = FLT_MAX;
		float fNearDistance = FLT_MAX;
		pNearestObject = UPDATER->PickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance);

		fNearDistance = fHitDistance;

		return(pNearestObject);
	}
	return nullptr;
}

void CSceneMain::CreateControllObject(string path){
//resource ����	
	m_MeshCnt = RESOURCEMGR->CreateMultiMesh(path, "Test");
	//m_MeshCnt = RESOURCEMGR->CreateMultiMesh("../outputata/text.txt", "Test");
	RCSELLER->GetRenderContainer("fbx")->ClearMesh();
	char pName[20];
	for (int i = 0; i < m_MeshCnt; ++i) {
		sprintf(pName, "%s%d", "Test", i);
		RCSELLER->GetRenderContainer("fbx")->AddMesh(RESOURCEMGR->GetMesh(pName, i));
	}
	RCSELLER->GetRenderContainer("fbx")->SetAnimater(RESOURCEMGR->GetAnimater("Test"));
//resource ����	


//��ü ����
	m_pFBXObject = new CTestObject();
	m_pFBXObject->Begin();
	//pObject->SetTerrainContainer(m_pTerrainContainer);
	m_pFBXObject->SetPosition(XMLoadFloat3(&XMFLOAT3(UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f, 0, UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f)));
	UPDATER->GetSpaceContainer()->AddObject(m_pFBXObject);
//��ü ����
	//ui pop up!
	m_pFBXObject->PickingProc();
}

void CSceneMain::CreateLoadFileUI(){

	const char* barName{ "LoadFile" };
	TWBARMGR->AddBar(barName);

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".fbx");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".FBX");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".GJM");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, true, L".GJM");
	

	//const char* groupName = "File";
	char menuName[256];
	int cnt{ 0 };
	m_LoadFileStruct.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_LoadFileStruct[cnt] = LoadFileStruct{ this, filsDirectory };

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());
		sprintf(menuName, "%s", menuNameString.c_str());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);
	
		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str() , menuName, LoadFileCallback, &m_LoadFileStruct[cnt]);
		cnt++;
	}
	//make texture/ set texture
	//pObj->GetRenderContainer()->GetMesh(pObj->GetSelectMeshIndex());
}

void CSceneMain::AddFBXAnimationInfo(string path){
	if (nullptr == m_pFBXObject) {
		//wstring ws{ L"" };
		//ws.assign(path.cbegin(), path.cend());
		CreateControllObject(path);
		return;
	}

	//fbx animation info �߰�!
	string name{ "Test" };
	FBXIMPORTER->Begin(path);
	if (FBXIMPORTER->GetHasAnimation()) {
		CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromFBXFile(RESOURCEMGR->GetAnimater(name));
	}
	FBXIMPORTER->End();
	//fbx animation info �߰�!

	m_pFBXObject->PickingProc();
}

void CSceneMain::CreateAddInfoUI(){
	const char* barName{ "AddInfo" };
	TWBARMGR->AddBar(barName);

	if (m_LoadFileStruct.empty()) {
		vector<wstring> vFile;
		DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, false, L".fbx");
		DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/SceneResource", true, false, L".FBX");

		const char* groupName = "File";
		char menuName[256];
		int cnt{ 0 };
		m_LoadFileStruct.resize(vFile.size());
		for (auto data : vFile) {
			string s{ "" };
			s.assign(data.cbegin(), data.cend());
			m_LoadFileStruct[cnt] = LoadFileStruct{ this, s };
			sprintf(menuName, "%s", s.c_str());
			TWBARMGR->AddButtonCB(barName, groupName, menuName, AddInfoCallback, &m_LoadFileStruct[cnt]);
			cnt++;
		}
	}
	else {
		const char* groupName = "File";
		char menuName[256];
		int cnt{ 0 };
		for (auto data : m_LoadFileStruct) {
			sprintf(menuName, "%s", m_LoadFileStruct[cnt].Filename.c_str());
			TWBARMGR->AddButtonCB(barName, groupName, menuName, AddInfoCallback, &m_LoadFileStruct[cnt]);
			cnt++;
		}
	}
}

void CSceneMain::ClearAllFBXObject(){
	if (m_pFBXObject) {
		UPDATER->GetSpaceContainer()->RemoveObject(m_pFBXObject);
		m_pFBXObject->GetRenderContainer()->ClearMesh();
		m_pFBXObject->GetRenderContainer()->ClearAnimater();
		m_pFBXObject->End();
		delete m_pFBXObject;
		m_pFBXObject = nullptr;

		//pObject->SetTerrainContainer(m_pTerrainContainer);
		char MeshName[64];
		const char* name = "Test";
		for (int i = 0; i < m_MeshCnt; ++i) {
			sprintf(MeshName, "%s%d", name, i);
			RESOURCEMGR->ReleaseStempMesh(MeshName);
		}
		RESOURCEMGR->ReleaseAnimater(name);
		m_MeshCnt = 0;

	}
	ClearAllFBXUI();
}

void CSceneMain::ClearAllFBXUI(){
	TWBARMGR->DeleteBar("AnimaterInfo");
	TWBARMGR->DeleteBar("AddInfo");
	TWBARMGR->DeleteBar("LoadFile");

	TWBARMGR->DeleteBar("AnimationInfo");
	TWBARMGR->DeleteBar("ActiveJoint");

	TWBARMGR->DeleteBar("MeshInfo");
	TWBARMGR->DeleteBar("PickingBar");
	TWBARMGR->DeleteBar("LoadTextureFile");
}

void CSceneMain::CreateBaseTextureSelectUI(){
	const char* barName{ "LoadFile" };
	TWBARMGR->AddBar(barName);

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".jpg");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".JPG");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".png");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets", true, true, L".PNG");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/Detail_Texture/Tile", true, true, L".tga");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/Detail_Texture/Tile", true, true, L".TGA");


	//const char* groupName = "File";
	char menuName[256];
	int cnt{ 0 };
	m_LoadFileStruct.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_LoadFileStruct[cnt] = LoadFileStruct{ this, filsDirectory };

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());
		sprintf(menuName, "%s", menuNameString.c_str());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);

		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str(), menuName, BaseTextureSelectCallback, &m_LoadFileStruct[cnt]);
		cnt++;
	}
}

void CSceneMain::CreateObjectPositioningUI(){
	const char* barName{ "ObjectList" };
	TWBARMGR->DeleteBar(barName);
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 250, 300);
	TWBARMGR->SetBarPosition(barName, 0, 0);
	TWBARMGR->SetBarColor(barName, 255, 0, 255);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	for (auto pObject : m_vpObjectList) {
		TWBARMGR->AddButtonCB(barName, "Objects", pObject->GetName().c_str(), OPCreatePositioningObjectButtonCallback, pObject);
	}
	for (auto pObject : m_vpStempObjectList) {
		TWBARMGR->AddButtonCB(barName, "StempObjects", pObject->GetName().c_str(), OPCreatePositioningObjectButtonCallback, pObject);
	}
	
}

void CSceneMain::ObjectPositioning(){
	
	if (GLOBALVALUEMGR->GetPositioningObject()->GetName() == "pointlight") {
		CPointLight *pPositioningPointLight = CPointLight::CreatePointLight(100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5));
		pPositioningPointLight->SetWorldMtx(GLOBALVALUEMGR->GetPositioningObject()->GetWorldMtx());
		UPDATER->GetSpaceContainer()->AddObject(pPositioningPointLight);
	}
	else if (GLOBALVALUEMGR->GetPositioningObject()->GetName() == "spotlight") {
		CSpotLight* pPositioningSpotLight = CSpotLight::CreateSpotLight(500.f, XMFLOAT3(0.1f, 0.1f, 0.1f), 30.f, 15.f);
		//CSpotLight* pPositioningSpotLight = new CSpotLight;
		//pPositioningSpotLight->Begin(SPOT_LIGHT{ 500.f, XMFLOAT3(rand() % 100, rand() % 100, rand() % 100), 30.f, 15.f });
		pPositioningSpotLight->SetWorldMtx(GLOBALVALUEMGR->GetPositioningObject()->GetWorldMtx());
		UPDATER->GetSpaceContainer()->AddObject(pPositioningSpotLight);
	}
	else if (GLOBALVALUEMGR->GetPositioningObject()->GetName() == "capsulelight") {
		CCapsuleLight* pPositioningCapsuleLight = CCapsuleLight::CreateCapsuleLight(100.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5), 50.f);
		pPositioningCapsuleLight->SetWorldMtx(GLOBALVALUEMGR->GetPositioningObject()->GetWorldMtx());
		UPDATER->GetSpaceContainer()->AddObject(pPositioningCapsuleLight);
	}
	else {
		CGameObject* pObject = nullptr;
		//if(GLOBALVALUEMGR->GetPositioningObject()->GetAnimater())
		pObject = new CGameObject(GLOBALVALUEMGR->GetPositioningObject()->GetName(), GLOBALVALUEMGR->GetPositioningObject()->GetTag());
		pObject->Begin();
		pObject->SetWorldMtx(GLOBALVALUEMGR->GetPositioningObject()->GetWorldMtx());
		UPDATER->GetSpaceContainer()->AddObject(pObject);
	}
}

void CSceneMain::CreateSceneContainers(){
	
}

void CSceneMain::CreateTerrainContainer(){
	//terrain
	UPDATER->SetTerrainContainer(CTerrainContainer::CreateTerrainContainer(L"Temp", 256, 256, 0.5, UPDATER->GetSpaceContainer(), true));
}

//void CSceneMain::DeleteTerrainContainer(){
//	m_pTerrainContainer->End();
//	delete m_pTerrainContainer;
//	m_pTerrainContainer = nullptr;
//}

void CSceneMain::CreateSkyBoxContainer(){
	//skybox
	UPDATER->SetSkyBoxContainer(CSkyBoxContainer::CreateSkyBoxContainer(L"Temp", 0, UPDATER->GetSpaceContainer()));
}

//void CSceneMain::DeleteSkyBoxContainer(){
//	m_pSkyBoxContainer->End();
//	delete m_pSkyBoxContainer;
//	m_pSkyBoxContainer = nullptr;
//}

CSceneMain::CSceneMain(CDirectXFramework* pFrameWork) : CScene("Main") {
	m_pFrameWork = pFrameWork;
}
CSceneMain::~CSceneMain() {

}

