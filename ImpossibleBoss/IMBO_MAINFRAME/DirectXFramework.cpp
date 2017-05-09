#include "stdafx.h"

#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{
	srand((unsigned)time(NULL));
	InitSingleton(hInstance, hWnd);
	_tcscpy_s(m_pszBuffer, _T("Impossible Boss ("));		//	GameTitle
}

void CDirectXFramework::End() {
	//camera end
	if (m_pCamera) { m_pCamera->End(); }
	m_pCamera = nullptr;

	//singleton End
	RESOURCEMGR->End();
	RCSELLER->End();
	DEBUGER->End();
	INPUTMGR->End();
	UPDATER->End();
	SCENEMGR->End();
	NETWORKMGR->End();
}
void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	Render();

	TIMEMGR->GetFrameRate(m_pszBuffer + 17, 35);
	::SetWindowText(GLOBALVALUEMGR->GethWnd(), m_pszBuffer);
}
void CDirectXFramework::Update(float fTimeElapsed) {
	//postprocessinglayer ������ set
	if (nullptr != SCENEMGR->GetPresentScene())
	{
		if (false == SCENEMGR->GetPresentScene()->GetIsLoading())
		{
			RENDERER->Update(fTimeElapsed);
			ProcessInput(fTimeElapsed);
			UPDATER->PreUpdate(fTimeElapsed);
		}		
	
		SCENEMGR->GetPresentScene()->Animate(fTimeElapsed);
		if (false == SCENEMGR->GetPresentScene()->GetIsLoading())
		{
			UPDATER->Update(fTimeElapsed);
			UPDATER->PhisicsUpdate(fTimeElapsed);
		}		
	}	
}

		
void CDirectXFramework::Render() {
	//-----------------------------ī�޶� ���� set------------------------------------

	if (nullptr != SCENEMGR->GetPresentScene())
	{
		if (true == SCENEMGR->GetPresentScene()->GetIsLoading())
			RENDERER->PreRender();
		else
		{
			//m_pCamera->SetShaderState();
			RENDERER->Render(m_pCamera);
		}			
	}	
}


void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	if (nullptr != SCENEMGR->GetPresentScene())
		SCENEMGR->GetPresentScene()->ProcessInput(fTimeElapsed);
}
LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	INPUTMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	return(0);
}

CDirectXFramework::~CDirectXFramework() {

}

void CDirectXFramework::InitSingleton(HINSTANCE hInstance, HWND hWnd)
{
	//singleton Init 
	GLOBALVALUEMGR->Begin(hInstance, hWnd);

	Make_Camera();

	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	DEBUGER->Begin();
	INPUTMGR->Begin();
	RENDERER->Begin();
	CAMMGR->Begin();
	UPDATER->Begin();
	SCENEMGR->Begin(this);
	NETWORKMGR->Begin();

	SCENEMGR->ChangeScene(SC_ORITOWN);
}

void CDirectXFramework::Make_Camera()
{
	
	//camera default
	m_pCamera = make_shared<CFreeCamera>();
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)// ��Ⱦ��
		, NEAR_PLANE, FAR_PLANE);// �ִ� �Ÿ�

	XMVECTOR eye = { 0.0f, 400.f, -300.0f, 0.0f };
	XMVECTOR at = { 0.0f, 0.0f, 1.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	CAMMGR->SetCamera(CAM_FREE, m_pCamera.get());


	CCamera* pCamera = new CFreeCamera;
	pCamera->Begin();
	pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)// ��Ⱦ��
		, NEAR_PLANE, FAR_PLANE);// �ִ� �Ÿ�
	eye = { 0.0f, 0.f, 0.0f, 0.0f };
	at = { 0.0f, 0.0f, 1.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	pCamera->SetLookAt(eye, at, up);
	pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	CAMMGR->SetCamera(CAM_LIGHT, pCamera);

}
