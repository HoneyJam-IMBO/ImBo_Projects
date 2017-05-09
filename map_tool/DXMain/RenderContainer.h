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
	vector<shared_ptr<CMesh>>& GetvMesh() { return m_vpMesh; }
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
	void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.clear(); }//�׸� �� �׸��� clear
	list<CGameObject*>& GetObjectList() { return m_lpObjects; }

	void ClearTextures() { m_vpTexture.clear(); }
	//--------------------------container---------------------------------
protected:
	//--------------------------���� ���------------------------------
	//�����̳��� ��ü���� �������� ���� ���̴�. �׷��Ƿ� ����Ʈ�� �����ϴ� ���� �´�
	list<CGameObject*> m_lpObjects;

	vector<shared_ptr<CMesh>> m_vpMesh;
	shared_ptr<CRenderShader> m_pShader;
	vector<shared_ptr<CTexture>> m_vpTexture;
	//�ֹ߼� texture �ѹ� �����ϸ� �������. 
	vector<shared_ptr<CTexture>> m_vpVolatileTexture;
	vector<shared_ptr<CBuffer>> m_vpBuffer;
	//�ֹ߼� buf �ѹ� �����ϸ� �������. 
	vector<shared_ptr<CBuffer>> m_vpVolatileBuffer;
	shared_ptr<CAnimater> m_pAnimater{ nullptr };//animater

	int m_nBuffer{ 0 };
	vector<shared_ptr<CMaterial>> m_vpMaterial;
	int m_nMaterial{ 0 };
	//--------------------------���� ���------------------------------

	void** m_ppBufferData{ nullptr };
public:
	CRenderContainer();
	virtual ~CRenderContainer();

};

typedef map<string, CRenderContainer*> mapRenderContainer;
typedef map<tag, mapRenderContainer> mapTagRenderContainer;
typedef pair<string, CRenderContainer*> pairRenderContainer;
typedef pair<tag, pairRenderContainer> pairTagRenderContainer;


/*
�����̳� - AnimateContainer, RenderContainer�� �и�
{
���� ���� - 
	1. AnimateContainer�� ��� object Animate
	2. �׸� �׸� object RenderContainer�� AddObject
	3. RenderContainer���� ī�޶� �������� �ø� �� �ν��Ͻ����� ����
���⿡ �������� ����?
CScene�� Space�� �����Ѵ�.
Spcae�� Container�� �����ϴµ� AnimateContainer��.
Animate�� �� �߿��Ѱ� ������ ��� ��� �ٷ� �ٸ� ������ �̽��ϴ� ���� �ƴ϶�
���� �����Ҵٰ� �־� ��� �Ѵ�. pExcludeContainer
���⼭ ExcluderContainer�� ��ü���� ���� ������ �� �� ������

----------------------------------------------------
AnimateContainer/ object_id{enum} �ʿ伺 ���

enum���� 0-nObject ��ü �������� enum�ؼ� �̰� objectid ������ ������ �־
������ �з��� �� �� �ִٸ� AnimateContainer�� object�� �߻�ȭ�� �� ���ִٴ� �����Ͽ�
��ü���� ��� �ǰ� �ϳ��� �������� �ϳ����� ������ �ȴ�.
----------------------------------------------------

----------------------------------------------------
RenderContainer/ object���� �ʿ��ϴ�
RenderContainer�� instancing�ؾ� �ϴϱ� ���� �� ��ü���� �����ؾ� �Ѵ�.
���� RenderContainer�� CScene�� �����ϰ�
Scene�� RenderContainer�� map���� �����Ѵ�. map<object_id, RenderContainer> 

AnimateContainer���� enum�̷� �з��� ������ �ؼ� RenderContainer���� �Ѱ��ָ�
�װ� �޾Ƽ� ó���ϵ��� �Ѵ�.

RenderContainer�� CScene�� �����ϴ°�
}
*/