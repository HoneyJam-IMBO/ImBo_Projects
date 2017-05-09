#pragma once

#include "DXObject.h"
#include "GameObject.h"

class CDirectXFramework;

class CScene{

public:
	//begin end
	virtual bool Begin() { return true; };
	virtual bool Begin(string path);
	virtual bool End();
	//begin end
	//animate
	virtual void Animate(float fTimeElapsed);
	//animate
	//input
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void ProcessInput(float fTimeElapsed) {};
	//input
	//pick
	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient) { return nullptr; };
	//pick
	//create scene
	static CScene* CreateScene(string name, CDirectXFramework* pFramework);
	//create scene

	void LoadScene(string path);
	virtual void LoadResource(wstring wsMeshRoot);//�߰�

	string GetSceneName() { return m_sName; }
	wstring GetMeshRoot() { return m_wsMeshRoot; }
protected:
	string m_sName;
	//resource�� root dir�� assets�̴�. �߰�
	wstring m_wsMeshRoot{ L"../../Assets" };

	//framework
	CDirectXFramework* m_pFrameWork{ nullptr };
	//��¥ ī�޶�� framework�� �ִ�.
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//picking
	CGameObject* m_pPickingObject{ nullptr };
private:

public:
	CScene::CScene(string name) { m_sName = name; }
	~CScene() {}

	//set get
	void SetFrameWork(CDirectXFramework* pFramework) { m_pFrameWork = pFramework; }
	CDirectXFramework* GetFrameWork() { return m_pFrameWork; }
	//set get
};
