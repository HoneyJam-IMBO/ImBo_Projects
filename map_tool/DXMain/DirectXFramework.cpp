#include "stdafx.h"
#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{
	//singleton Init 
	//global value
	GLOBALVALUEMGR->Begin(hInstance, hWnd);
	TWBARMGR->Begin(" GLOBAL help='test ui death' ");
	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	DEBUGER->Begin();
	INPUTMGR->Begin();
	RENDERER->Begin();
	RENDERER->SetFramework(this);
	UPDATER->Begin();
	//singleton Init

	_tcscpy_s(m_pszBuffer, _T("DXMAIN ("));

	//device ���� ���� �ؾ���

	//camera
	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------ī�޶� ����--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		60.0f * XM_PI / 180.0f
		// ��Ⱦ��
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
		// �ּ� �Ÿ�
		, 10.0f
		// �ִ� �Ÿ�
		, 3000.0f);

	//��ġ ����

	//viewprojection��� ����
	XMVECTOR eye = { 0.0f, 200.f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 128.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS�� ����Ʈ ����
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	//------------------------------------------ī�޶� ����--------------------------------------


	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//int barSize[2] = { 224, 320 };
	//TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

}

void CDirectXFramework::End() {
	//camera end
	if (m_pCamera) {
		m_pCamera->End();
	}
	m_pCamera = nullptr;

	//scene end
	for (int i = 0; i < m_nScene; ++i) {
		m_stackScene.top()->End();
		delete m_stackScene.top();
		m_stackScene.pop();
	}


	//singleton End
	RESOURCEMGR->End();
	RCSELLER->End();
	DEBUGER->End();
	INPUTMGR->End();
	UPDATER->End();
	//singleton End

	//	//ui
	TWBARMGR->End();
}

void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	//Update2D();

	Render();
	//Render2D();

	TIMEMGR->GetFrameRate(m_pszBuffer + 8, 37);
	::SetWindowText(GLOBALVALUEMGR->GethWnd(), m_pszBuffer);
}

		
void CDirectXFramework::Render() {
	//scene
	//set
	//-----------------------------ī�޶� ���� set------------------------------------
	m_pCamera->SetShaderState();
	//-----------------------------ī�޶� ���� set------------------------------------

	RENDERER->Render(m_pCamera);
}

void CDirectXFramework::Update(float fTimeElapsed) {
	DEBUGER->start_Timemeasurement();

	RENDERER->Update(fTimeElapsed);

	//postprocessinglayer ������ set
	//m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	UPDATER->PreUpdate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	UPDATER->Update(fTimeElapsed);
	UPDATER->PhisicsUpdate(fTimeElapsed);


	INPUTMGR->SetWheel(WHEEL_NON);

	if (INPUTMGR->KeyDown(VK_0)) {
		TWBARMGR->DeleteBar("TweakBarTest2");
	}

	DEBUGER->end_Timemeasurement(L"update");
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------

}
void CDirectXFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_stackScene.top()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
		if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_FREECAMERA)
			INPUTMGR->SetbCapture(true);

		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseLeft(true);
		break;

	case WM_RBUTTONDOWN:
		if (GLOBALVALUEMGR->GetToolMode() == TOOL_MODE_FREECAMERA)
			INPUTMGR->SetbCapture(true);

		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseRight(true);
		break;

	case WM_LBUTTONUP:
	{
		//static bool showCusor = true;
		//showCusor = showCusor ? false : true;
		//ShowCursor(showCusor);
		INPUTMGR->SetMouseLeft(false);
		INPUTMGR->SetbCapture(false);
		break;
	}

	case WM_RBUTTONUP:
		INPUTMGR->SetMouseRight(false);
		INPUTMGR->SetbCapture(false);
		break;

	case WM_MOUSEMOVE:
		INPUTMGR->SetMousePoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		((short)HIWORD(wParam) < WHEEL_NON) ?
			INPUTMGR->SetWheel(WHEEL_DOWN) :
			INPUTMGR->SetWheel(WHEEL_UP);
		break;
		//for drag&drop
	case WM_DROPFILES:
		INPUTMGR->ProcDropFile(wParam);

		break;
	default:
		break;
	}
}


void CDirectXFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	m_stackScene.top()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
	}
}

LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//	//ui
	if (TWBARMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam)) return 0;

	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE: {
		GLOBALVALUEMGR->SetrcClient(LOWORD(lParam), HIWORD(lParam));
		GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(0, NULL, NULL);

		RENDERER->ResizeBuffer();

		m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_DROPFILES:
		INPUTMGR->ProcDropFile(wParam);

		break;
	}
	return(0);
}

//--------------------------------------scene-----------------------------------------
void CDirectXFramework::ChangeScene(CScene* pScene) {
	//���� �ƿ� �ٲ� ������ ���� pop�ϰ� 
	if (m_nScene != 0) {
		//������ Scene�� �����ϸ�

		PopScene();
		//������ scene�� ������

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
	else {
		//�ƴϸ� ó�� �ִ� Scene�̸� �׳�
		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene�� ������

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top�� ������ begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//���ο� scene�� ���� ����
	++m_nScene;
}
//--------------------------------------scene-----------------------------------------

//--------------------------------------deferred rendering func-----------------------------

CDirectXFramework::~CDirectXFramework() {

}
