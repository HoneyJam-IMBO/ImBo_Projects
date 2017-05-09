#include "stdafx.h"
#include "SCHeroSel.h"
#include "ImageUI.h"
#include "ClickButton.h"

#include "LoadingBack.h"



CSCHeroSel::CSCHeroSel(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSCHeroSel::~CSCHeroSel()
{
}

bool CSCHeroSel::Begin()
{
	//NETWORKMGR->Connect("192.168.10.101");
	//NETWORKMGR->SendPacket();

	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	string strName = "Back";
	CUIObject* pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), StringToTCHAR(strName), 0.f);
	m_vecUI.push_back(pUI);

	for (int i = 0; i < 6; ++i)
	{
		strName = "CButton_" + to_string(i);
		pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 400.f + i * 160.f, WINSIZEY * 0.75f)),
												XMLoadFloat2(&XMFLOAT2(70.f, 120.f)), StringToTCHAR(strName));
		((CClickButton*)pUI)->SetID(0);
		m_vecButtonUI.push_back(pUI);

		strName = "Char_Thumb_" + to_string(i);
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.4f)),
							XMLoadFloat2(&XMFLOAT2(200.f, 280.f)), StringToTCHAR(strName), 1.f);
		m_vecCharUI.push_back(pUI);
	}
	strName = "Button_Ready";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.92f)),
		XMLoadFloat2(&XMFLOAT2(105.f, 32.f)), StringToTCHAR(strName));
	((CClickButton*)pUI)->SetID(1);
	m_vecButtonUI.push_back(pUI);

	for (int i = 0; i < 4; ++i)
	{
		strName = "Ready_0";
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 150.f + i * 100.f, WINSIZEY * 0.55f)),
			XMLoadFloat2(&XMFLOAT2(20.f, 20.f)), StringToTCHAR(strName), 2.f);
		m_vecReadyUI.push_back(pUI);
	}

	return CScene::Begin();
}
bool CSCHeroSel::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecUI[i];
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecButtonUI[i];
	}
	iVecSize = m_vecReadyUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecReadyUI[i];
	}
	return true;
}

void CSCHeroSel::Animate(float fTimeElapsed)
{
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i){
		m_vecUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i){
		m_vecButtonUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecReadyUI.size();
	for (size_t i = 0; i < iVecSize; ++i){
		m_vecReadyUI[i]->Update(fTimeElapsed);
	}
	KeyInput();

	//m_vecReadyUI[0]->SetImageName()


	if (-1 != m_iHeroSelNum)
	{
		m_vecCharUI[m_iHeroSelNum]->Update(fTimeElapsed);
	}

	if (INPUTMGR->KeyDown(VK_1))
	{
		SCENEMGR->ChangeScene(SC_ORITOWN);
	}
	if (INPUTMGR->KeyDown(VK_P))
	{
		//cs_packet_create_room* pPacket = new cs_packet_create_room;
		//pPacket->Size = sizeof(cs_packet_create_room);
		//pPacket->Type = CS_CREATE_ROOM;
		//NETWORKMGR->SendPacket(pPacket, pPacket->Size);
	}
	if (INPUTMGR->KeyDown(VK_L))
	{
		//�̰� Room Join
		//cs_packet_join_room* pPacket = new cs_packet_join_room;
		//pPacket->Size = sizeof(cs_packet_join_room);
		//pPacket->Type = CS_JOIN_ROOM;
		//pPacket->RoomNumber = 0;   // 0�� �ӽ�
		//NETWORKMGR->SendPacket(pPacket, pPacket->Size);
	}

}

void CSCHeroSel::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::ProcessInput(float fTimeElapsed)
{
}

void CSCHeroSel::SetSelSceneInfo(int slot_id, int character, bool is_ready){
	if (is_ready) {
		m_vecReadyUI[slot_id]->SetImageName(L"Ready_1");
	}
	else {
		m_vecReadyUI[slot_id]->SetImageName(L"Ready_1");
	}
}

void CSCHeroSel::KeyInput()
{
	if (true == INPUTMGR->MouseLeftOnlyDown())
	{
		CheckCollisionButton();
	}
}

void CSCHeroSel::CheckCollisionButton()
{
	POINT ptPos = INPUTMGR->GetMousePoint();
	size_t iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		XMFLOAT2 xmButtonPos, xmButtonSize;
		XMStoreFloat2(&xmButtonPos, m_vecButtonUI[i]->GetUIPos());
		XMStoreFloat2(&xmButtonSize, m_vecButtonUI[i]->GetUISize());
		if ((ptPos.x > xmButtonPos.x - xmButtonSize.x&& ptPos.x < xmButtonPos.x + xmButtonSize.x )
			&& (ptPos.y > xmButtonPos.y - xmButtonSize.y&& ptPos.y < xmButtonPos.y + xmButtonSize.y))
		{
			if (((CClickButton*)m_vecButtonUI[i])->GetID() == 0)
			{
				m_iHeroSelNum = i;
				cs_packet_client_info_in_room* pPacket = new cs_packet_client_info_in_room;
				pPacket->Character = i;
				pPacket->isReady = false;
				pPacket->Size = sizeof(cs_packet_client_info_in_room);
				pPacket->Type = CS_CHARACTER_READY_CHANGE;
				NETWORKMGR->SendPacket(pPacket, pPacket->Size);
				return;
			}
			else if (((CClickButton*)m_vecButtonUI[i])->GetID() == 1)
			{
				if (m_iHeroSelNum == -1)
					return;

				m_bReady =( m_bReady + 1 )%2;
				cs_packet_client_info_in_room* pPacket = new cs_packet_client_info_in_room;
				pPacket->Character = m_iHeroSelNum;
				pPacket->isReady = m_bReady;
				pPacket->Size = sizeof(cs_packet_client_info_in_room);
				pPacket->Type = CS_CHARACTER_READY_CHANGE;
				NETWORKMGR->SendPacket(pPacket, pPacket->Size);
				return;
			}			
		}
	}
}
