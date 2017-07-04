#pragma once

#include "Scene.h"

//object
#include "FreeCamera.h"

class CSCOriTown;
class CSCOriTown :public CScene{

public:
	CSCOriTown(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCOriTown();
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);
	CGameObject* PickObjectPointedByCursor(int xClient, int yClient);
	
	//network
	void NetworkProc();
	VOID PROC_PT_FTOWN_READY_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	//�ֱ��� ��ġ ����ȭ
	VOID PROC_PT_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE *Packet, DWORD dwPacketLength);

	CGameObject* GetPlayer() { return m_ppPawn ? m_ppPawn[NETWORKMGR->GetSLOT_ID()] : nullptr; }
private:
	void ReadMapData();

	void RoadSkillObjects();

	void CreateUI();

	bool m_bGameStart{ false };
private:
	//CGameObject* m_pObject{ nullptr };
	CGameObject** m_ppPawn{ nullptr };
	//CPlayer* m_pPlayer{ nullptr };
};