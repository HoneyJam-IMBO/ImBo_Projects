#pragma once
#include "DXObject.h"
#include "GameObject.h"

//dxobject
#include "Mesh.h"
#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "Material.h"
#include "Animater.h"//animater!!
//dxobject

#include "Camera.h"

class CRenderContainer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	void ClearVolatileResources();
private:
	virtual void RenderExcute();
	void RenderExcuteWithOutObject();
public:
	void Render(shared_ptr<CCamera> pCamera);
	void RenderWithOutObject(shared_ptr<CCamera> pCamera);

	shared_ptr<CMesh> GetMesh(UINT index = 0) { return m_vpMesh[index]; }
	CAtlArray<shared_ptr<CMesh>>& GetvMesh() { return m_vpMesh; }
	//set�� �ƴ϶����� add��.
	void ClearMesh();
	void ClearBuffer();
	void ClearAnimater();
	void AddMesh(shared_ptr<CMesh> pMesh);
	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CRenderShader> pShader);
	
	//void AddMesh(shared_ptr<CMesh> pMesh);
	//void AddShader(shared_ptr<CRenderShader> pShader);
	void AddTexture(shared_ptr<CTexture> pTexture);
	//�ֹ߼� texture�� add
	void AddVolatileTexture(shared_ptr<CTexture> pTexture);
	void AddBuffer(shared_ptr<CBuffer> pBuffers);
	//�ֹ߼� buf�� �߰���
	void AddVolatileBuffer(shared_ptr<CBuffer> pBuffers);
	void AddInstanceBuffer(shared_ptr<CBuffer> pBuffers);
	void AddMaterial(shared_ptr<CMaterial> pMaterial);
	
	//animater!
	void SetAnimater(shared_ptr<CAnimater> pAnimater) { m_pAnimater = pAnimater; }
	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }
	//animater!

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	//void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.RemoveAll(); }//�׸� �� �׸��� clear
	CAtlArray<CGameObject*>& GetObjectList() { return m_lpObjects; }

	void ClearTextures() { m_vpTexture.RemoveAll(); }
	//--------------------------container---------------------------------
protected:
	//--------------------------���� ���------------------------------
	//�����̳��� ��ü���� �������� ���� ���̴�. �׷��Ƿ� ����Ʈ�� �����ϴ� ���� �´�
//	int m_nInstance{ 0 };
	//vector<CGameObject*> m_lpObjects;
	CAtlArray<CGameObject*>	m_lpObjects;
	

	CAtlArray<shared_ptr<CMesh>>	m_vpMesh;		//vector<shared_ptr<CMesh>>		m_vpMesh;
	shared_ptr<CRenderShader>		m_pShader;
	CAtlArray<shared_ptr<CTexture>>	m_vpTexture;	//vector<shared_ptr<CTexture>>	m_vpTexture;
	//�ֹ߼� texture �ѹ� �����ϸ� �������. 
	CAtlArray<shared_ptr<CTexture>>	m_vpVolatileTexture;	//vector<shared_ptr<CTexture>>	m_vpVolatileTexture;
	CAtlArray<shared_ptr<CBuffer>>	m_vpVolatileBuffer;		//vector<shared_ptr<CBuffer>>	m_vpVolatileBuffer;

	CAtlArray<shared_ptr<CBuffer>>	m_vpBuffer;			//vector<shared_ptr<CBuffer>>		m_vpBuffer;
	CAtlArray<shared_ptr<CMaterial>>	m_vpMaterial;	//vector<shared_ptr<CMaterial>>	m_vpMaterial;
	shared_ptr<CAnimater>			m_pAnimater{ nullptr };//animater

	int m_nBuffer{ 0 };
	int m_nMaterial{ 0 };
	//--------------------------���� ���------------------------------

	void** m_ppBufferData{ nullptr };
public:
	CRenderContainer();
	virtual ~CRenderContainer();
	//CAtlString a;
	//CString
};


typedef CAtlMap<tag, CAtlMap<CString, CRenderContainer*>> mapTagRenderContainer;
typedef CAtlMap<CString, CRenderContainer*> mapRC;
//typedef map<string, CRenderContainer*> mapRenderContainer;
//typedef map<tag, mapRenderContainer> mapTagRenderContainer;
//typedef pair<string, CRenderContainer*> pairRenderContainer;
//typedef pair<tag, pairRenderContainer> pairTagRenderContainer;
