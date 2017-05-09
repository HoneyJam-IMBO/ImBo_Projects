#pragma once

#include "Scene.h"
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"
#include "Timer.h"

#define TITLE_MAX_LENGTH 64
//-------------------------deferred lighting------------------
#define RENDER_TARGET_NUMBER 3
//-------------------------deferred lighting------------------


class CDirectXFramework {

public:
	//���� ��ü �� ������ �ʱ�ȭ.
	void Begin(HINSTANCE hInstance, HWND hWnd);
	void End();

	//-------------------------------begin----------------------------
	using FLOAT4 = float[4];
	//-------------------------------begin----------------------------
	
public:
	//�׸� �׸���
	void Render();

	//��ü Animate, input����
	void Update(float fTimeElapsed);

	//�帧
	void FrameAdvance();

	//--------------------------------------input------------------------------------
	void ProcessInput(float fTimeElapsed);
	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	//--------------------------------------input------------------------------------

	//--------------------------------------scene-----------------------------------------
	void ChangeScene(CScene* pScene);
	void PopScene();
	void PushScene(CScene* pScene);
	CScene* GetCurScene() { return m_stackScene.top(); }
	//--------------------------------------scene-----------------------------------------

	shared_ptr<CCamera> GetCamera() { return m_pCamera; }
	
private:
	//str 
	_TCHAR									m_pszBuffer[50];
	//------------------------------------Scene-----------------------------------
	std::stack<CScene*>	m_stackScene;
	//scene ť
	int m_nScene{ 0 };
	//scene�� ����
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//------------------------------------Scene-----------------------------------

public :	
	CDirectXFramework() = default;
	~CDirectXFramework();
};
