#pragma once
#include "Object.h"
#include "Space.h"
#include "Camera.h"
#include "DirectionalLight.h"
//#include "SceneMain.h"
//#include "RenderContainerSeller.h"

class CSpaceContainer : public CObject {
public:
	void Begin();
	bool End();

	//space controller
	void Animate(float fTimeElapsed);
	void PrepareRenderOneSpace(shared_ptr<CCamera> pCamera, UINT renderFlag = RTAG_TERRAIN | RTAG_STATIC_OBJECT | RTAG_DYNAMIC_OBJECT | RTAG_LIGHT, int render_space = -1);
	void PrepareRender(shared_ptr<CCamera> pCamera, UINT renderFlag = RTAG_TERRAIN | RTAG_STATIC_OBJECT | RTAG_DYNAMIC_OBJECT | RTAG_LIGHT | RTAG_REFLECTION);
	//animate�ϴٰ� �ش� ������ ��� ��ü �ӽ� �����
	void AddBlockObjectList(CGameObject* pObject);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	void RemoveObject(string name);
	void ClearBlockObjectList();
	int SearchSpace(XMVECTOR xmvPos);
	void AddSpace(UINT index, CSpace* pSpace);
	void ChangeSpaceData();
	//��� lay �浹 �˻�� ��ü �� ���� ���� ����� ��ü
	CGameObject* PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse);
	//space controller

	static CSpaceContainer* CreateSpaceContainer(int size, int lv);

	void ClearAllObjects();
	void SaveObjectInfos();
	void LoadObjectInfos();
private:
	//space���� �����Ѵ�.
	CSpace** m_ppSpace{ nullptr };
	//���� Ʈ���� ��Ʈ��忡 �ش��ϴ� startSpace ����
	CSpace* m_pStartSpace{ nullptr };
	//block object list
	list<CGameObject*> m_lpBlockObject;

	//��ü ���� ũ��
	float m_size{ 0 };
	//������ ���� 
	float m_level{ 0 };
	//�� ���̵忡 �ִ� ������ ����
	int m_oneSideSpaceNum{ 0 };
	//���� �ϳ��� ũ��
	int m_oneSpaceSize{ 0 };
	//������ ����
	int m_nSpace{ 0 };

public:
	CSpaceContainer();
	~CSpaceContainer();

	//set get
	CSpace** GetAllSpace() { return m_ppSpace; }
	list<CGameObject*>& GetBlockObjectList() { return m_lpBlockObject; }
	CSpace* GetStartSpace() { return m_pStartSpace; }
	UINT GetSpaceNum() { return m_nSpace; }
	UINT GetSize() { return m_size; }
	UINT GetLevel() { return m_level; }
	UINT GetOneSideSpaceNum() { return m_oneSideSpaceNum; }
	UINT GetOneSpaceSize() { return m_oneSpaceSize; }

	int GetSpaceSize() { return m_size; }
	int GetSpaceLevel() { return m_level; }
	void SetSpaceSize(int size) { m_size = size; }
	void SetSpaceLevel(int lv) { m_level = lv; }
	//set get
};