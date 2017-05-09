#include "stdafx.h"
#include "Loading.h"
#include "SceneMgr.h"

#include "SCHeroSel.h"
#include "SCOriTown.h"

#include "LoadingBack.h"

CLoading::CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork)
	:CScene(eID)
{
	m_pFrameWork = pFrameWork;
}
CLoading::~CLoading()
{
	End();
}

bool CLoading::Begin()
{
	m_bIsLoading = true;
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	RESOURCEMGR->ReleaseAllResource();
	RCSELLER->ClearStempRenderContainer();

	string strName = "Logo";
	RESOURCEMGR->CreateTexture("Logo", _T("../../Assets/Loading/Logo.png"), PS_TEXTURE, BIND_PS);
	CUIObject* pUI = CLoadingBack::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEX * 0.5f)), XMLoadFloat2(&XMFLOAT2(250.f, 250.f)), StringToTCHAR(strName));
	m_vecUI.push_back(pUI);

	m_hLoadThread = CreateThread(NULL, 0, FuncLoadResourceThread, this, 0, NULL);

	return true;
}
bool CLoading::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecUI[i];
		m_vecUI[i] = nullptr;
	}
	m_vecUI.clear();

	return true;
}

void CLoading::Animate(float fTimeElapsed)
{
	Sleep(10);
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}

	if (true == m_bComplete)
	{
		CScene*	pScene = nullptr;
		switch (m_eSceneID) {
		case SC_HEROSEL:		pScene = new CSCHeroSel(m_eSceneID, m_pFrameWork);			break;
		case SC_ORITOWN:		pScene = new CSCOriTown(m_eSceneID, m_pFrameWork);			break;
		}

		m_bIsLoading = false;
		UPDATER->GetSkyBoxContainer()->SetActive(true);
		UPDATER->GetTerrainContainer()->SetActive(true);
		CSceneMgr::GetInstance()->SetPresentScene(m_eSceneID, pScene);

		End();
		delete this;
	}
}

void CLoading::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::ProcessInput(float fTimeElapsed)
{

}

DWORD WINAPI CLoading::FuncLoadResourceThread(LPVOID arg)
{
	// ��� �߰��ϴ¾ֵ� �Լ��� ����
	RESOURCEMGR->CreateTexture("DEFAULT", _T("../../Assets/default.jpg"), PS_TEXTURE, BIND_PS);

	CLoading*	pMyThread = (CLoading*)arg;
	switch (pMyThread->GetSceneID())
	{
	case SC_LOBBY:
		pMyThread->LoadScene_LOBBY();
		break;
	case SC_HEROSEL:
		pMyThread->LoadScene_HEROSEL();
		break;
	case SC_ORITOWN:
		pMyThread->LoadScene_ORITOWN();
		break;
	case SC_REPAIRTOWN:
		pMyThread->LoadScene_REPAIRTOWN();
		break;
	case SC_ALDENAD:
		pMyThread->LoadScene_ALDENAD();
		break;
	case SC_BOSS:
		pMyThread->LoadScene_BOSS();
		break;
	}
	pMyThread->SetLoadComplete(true);

	return 0;
}

void CLoading::LoadScene_LOBBY()
{
}
void CLoading::LoadScene_HEROSEL()
{
	RESOURCEMGR->CreateTexture("Back", _T("../../Assets/Scene_HeroSel/background.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_0", _T("../../Assets/Scene_HeroSel/Char_Button_1.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_1", _T("../../Assets/Scene_HeroSel/Char_Button_2.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_2", _T("../../Assets/Scene_HeroSel/Char_Button_3.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_3", _T("../../Assets/Scene_HeroSel/Char_Button_4.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_4", _T("../../Assets/Scene_HeroSel/Char_Button_5.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_5", _T("../../Assets/Scene_HeroSel/Char_Button_6.jpg"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Char_Thumb_0", _T("../../Assets/Scene_HeroSel/Char_Thumb_1.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Thumb_1", _T("../../Assets/Scene_HeroSel/Char_Thumb_2.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Thumb_2", _T("../../Assets/Scene_HeroSel/Char_Thumb_3.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Thumb_3", _T("../../Assets/Scene_HeroSel/Char_Thumb_4.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Thumb_4", _T("../../Assets/Scene_HeroSel/Char_Thumb_1.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Thumb_5", _T("../../Assets/Scene_HeroSel/Char_Thumb_2.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Button_Ready", _T("../../Assets/Scene_HeroSel/Button_Ready.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Ready_0", _T("../../Assets/Scene_HeroSel/Ready_0.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Ready_1", _T("../../Assets/Scene_HeroSel/Ready_1.png"), PS_TEXTURE, BIND_PS);

}
void CLoading::LoadScene_ORITOWN()
{
	LoadScene("../../Assets/SceneResource/test/test.scn");
}

void CLoading::LoadScene_REPAIRTOWN()
{
}

void CLoading::LoadScene_ALDENAD()
{
}

void CLoading::LoadScene_BOSS()
{
}
