#include "stdafx.h"
#include "Space.h"
#include "SpaceContainer.h"
#include "RenderContainerSeller.h"



void CSpace::Begin(CSpaceContainer * pSpaceContainer, UINT size, int lv, XMVECTOR pos) {
	//object_id set 
	//	m_objectID = object_id::OBJECT_SPACE;
	CGameObject::Begin();
	//size�� �˾ƾ� ������ �𸣴ϱ� �ϴ� ����
	m_size = size;
	m_pSpaceContainer = pSpaceContainer;

	for (int i = 0; i < TAG_END; ++i)
	{
		CAtlArray<CGameObject*>* pObjectArray = new CAtlArray<CGameObject*>;
		m_mlpObject[tag(i)] = pObjectArray;
	}
	//m_mlpObject

	
	//�ڽ��� ��ġ�� ������ 
	//������ �߰��� �ƴ϶� ���� ó��/ ��ǥ�� ���� ��ǥ
	SetPosition(pos);

	//���� ���� --
	lv -= 1;

	//0���� ������ return ������ �ܰ��� ��
	/*
	ex) ó�� lootSpace�� Begin�� lv�� 1�� �־��ٸ�
	���ͼ� -1 �ϰ���? �׷� 0 �̾� 0���� ���� �ʾ�
	�׷� �ڽ��� 4�� ���鲨��. 1�� 1�ܰ�
	0�� �������� ����.
	*/
	if (lv < 0) {
		//�ڽ��� ������ ���� -> leaf space��
		//leaf space�� �ڽ��� index�� �˰� �־�� ��
		//�ڽ��� index�� ��� �� ����
		m_index = m_pSpaceContainer->SearchSpace(GetPosition());
		//space container�� �ڽ��� ���
		m_pSpaceContainer->AddSpace(m_index, this);

		float fx = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());
		float fy = static_cast<float>(m_pSpaceContainer->GetSpaceSize());
		float fz = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());

		BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));

		//SetRenderContainer(pSeller);//�׸��� �׸� ���� ������ RenderContainer set
		return;
	}

	//�ڽ� ���� �Ҵ�
	m_ppChildSpace = new CSpace*[4];
	//aabb
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));


	int k = 0;
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 2; ++i) {
			UINT child_size = (size / 2);
			//x���� ����, ���� z����.
			/* �ڽ� ������ ����
			2 3
			0 1
			*/
			XMVECTOR xmvOffset = XMVectorSet(static_cast<float>(i*child_size), 0.f, static_cast<float>(j*child_size), 0.f);
			m_ppChildSpace[k] = new CSpace();
			m_ppChildSpace[k++]->Begin(pSpaceContainer, child_size, lv, GetPosition() + xmvOffset);
		}
	}

}

bool CSpace::End() {

	POSITION ContainerPos = m_mlpObject.GetStartPosition();
	CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
	//CAtlArray<CGameObject*>::CPair*	pInPair = NULL;
	while (ContainerPos != NULL)
	{
		pOutPair = m_mlpObject.GetNext(ContainerPos);
		size_t lpSize = pOutPair->m_value->GetCount();
		for (size_t i = 0; i < lpSize; ++i)
		{
			(*pOutPair->m_value)[i]->End();
			delete (*pOutPair->m_value)[i];
		}
	}

	for (int i = 0; i < TAG_END; ++i)
	{
		delete m_mlpObject[tag(i)];
	}
	m_mlpObject.RemoveAll();
	//for (auto mlp : m_mlpObject) {
	//	for (auto pObject : mlp.second) {
	//		pObject->End();
	//		delete pObject;
	//	}
	//	mlp.second.clear();
	//}
	//m_mlpObject.clear();

	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->End();
			m_ppChildSpace[i] = nullptr;
		}
	}
	m_ppChildSpace = nullptr;




	delete this;

	return true;
}

void CSpace::Animate(float fTimeElapsed) {
	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->Animate(fTimeElapsed);
		}
	}
	else {//leaf space�� animate
		int nObject = 0;

		CAtlArray<CGameObject*>* lpDynObj;
		m_mlpObject.Lookup(tag::TAG_DYNAMIC_OBJECT, lpDynObj);
		size_t iDynObjSize = lpDynObj->GetCount();
		for (size_t i = 0; i < iDynObjSize; ++i)
		{
			(*lpDynObj)[i]->Animate(fTimeElapsed);
			int current_index = m_pSpaceContainer->SearchSpace((*lpDynObj)[i]->GetPosition());
			if ((*lpDynObj)[i]->GetSpaceIndex() != current_index)//���� ���� index�� ���� index�� �ٸ��ٸ�
			{
				m_pSpaceContainer->AddBlockObjectList((*lpDynObj)[i]);//block Object list�� ���
				lpDynObj->RemoveAt(i);
				//m_mlpObject[tag::TAG_DYNAMIC_OBJECT].erase(iter++);
			}
			/*else
				++i;*/
			nObject++;
		}

		//list<CGameObject*>::iterator iter = m_mlpObject[tag::TAG_DYNAMIC_OBJECT].begin();
		//list<CGameObject*>::iterator iter_end = m_mlpObject[tag::TAG_DYNAMIC_OBJECT].end();
		//for (; iter != iter_end; )
		//{
		//	(*iter)->Animate(fTimeElapsed);
		//	int current_index = m_pSpaceContainer->SearchSpace((*iter)->GetPosition());
		//	if ((*iter)->GetSpaceIndex() != current_index)//���� ���� index�� ���� index�� �ٸ��ٸ�
		//	{
		//		m_pSpaceContainer->AddBlockObjectList((*iter));//block Object list�� ���
		//		m_mlpObject[tag::TAG_DYNAMIC_OBJECT].erase(iter++);
		//	}
		//	else
		//		++iter;
		//	nObject++;
		//}

		if (INPUTMGR->GetDebugMode())
			DEBUGER->AddText(20.0f, 800.0f, static_cast<float>(m_index * 15.f), YT_Color(255, 255, 255), L"space %d object_num : %d", m_index, nObject);
		m_bRender = false;
	}
}

void CSpace::OptimizePrepare(CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair* pairGObj, UINT renderFlag, shared_ptr<CCamera> pCamera)
{
	size_t lpSize = pairGObj->m_value->GetCount();
	for (size_t i = 0; i < lpSize; ++i)
	{
		if ((*pairGObj->m_value)[i]->IsVisible(pCamera))
		{
			(*pairGObj->m_value)[i]->RegistToContainer();
		}
	}
}
void CSpace::PrepareRender(shared_ptr<CCamera> pCamera, UINT renderFlag) {

	if (IsVisible(pCamera))
	{											//���⿡	 space �������� �ø�
		if (nullptr == m_ppChildSpace) {		//�� �ڽ��� ������ ���� leaf node
			SetbRender(true);					//���� �׸��� space��.

			if (INPUTMGR->GetDebugMode())
				this->RegistToDebuger();

			POSITION ContainerPos = m_mlpObject.GetStartPosition();
			CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
			while (ContainerPos != NULL)
			{
				pOutPair = m_mlpObject.GetNext(ContainerPos);
				if(pOutPair->m_key == TAG_TERRAIN && (renderFlag & RTAG_TERRAIN))
					OptimizePrepare(pOutPair, renderFlag, pCamera);
				else if (pOutPair->m_key == TAG_STATIC_OBJECT && (renderFlag & RTAG_STATIC_OBJECT))
					OptimizePrepare(pOutPair, renderFlag, pCamera);
				else if (pOutPair->m_key == TAG_DYNAMIC_OBJECT && (renderFlag & RTAG_DYNAMIC_OBJECT))
					OptimizePrepare(pOutPair, renderFlag, pCamera);
				else if (pOutPair->m_key == TAG_LIGHT && (renderFlag & RTAG_LIGHT))
					OptimizePrepare(pOutPair, renderFlag, pCamera);
				else if (pOutPair->m_key == TAG_REFLECTION && (renderFlag & RTAG_REFLECTION))
					OptimizePrepare(pOutPair, renderFlag, pCamera);
			}

			//for (auto mlp : m_mlpObject) {		//��� ��ü�� ���ؼ�
			//	for (auto pObject : mlp.second) {

			//		//	pObject->RegistToContainer();
			//		if (renderFlag & RTAG_TERRAIN) {
			//			if (pObject->GetTag() == TAG_TERRAIN)
			//			{
			//				if (pObject->IsVisible(pCamera))
			//				{
			//					pObject->RegistToContainer();
			//				}
			//			}
			//		}
			//		if (renderFlag & RTAG_STATIC_OBJECT) {
			//			if (pObject->GetTag() == TAG_STATIC_OBJECT)
			//			{
			//				if (pObject->IsVisible(pCamera))
			//				{
			//					pObject->RegistToContainer();
			//				}
			//			}
			//		}
			//		if (renderFlag & RTAG_DYNAMIC_OBJECT) {
			//			if (pObject->GetTag() == TAG_DYNAMIC_OBJECT)
			//			{
			//				if (pObject->IsVisible(pCamera))
			//				{
			//					pObject->RegistToContainer();
			//				}
			//			}
			//		}
			//		if (renderFlag & RTAG_LIGHT) {
			//			if (pObject->GetTag() == TAG_LIGHT)
			//			{
			//				if (pObject->IsVisible(pCamera))
			//				{
			//					pObject->RegistToContainer();
			//				}
			//			}
			//		}
			//	}

			//}//end for
		}//end if
		else {//leaf�� �ƴ϶��
			for (int i = 0; i < 4; ++i) {
				m_ppChildSpace[i]->PrepareRender(pCamera, renderFlag);//�� �ڽĵ� PrePareRender
			}
		}//end else
	}
	//ī�޶� ������ ������ 
	return;
}



void CSpace::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		//��ü�� �ڽ� ��ȣ ���
		ppObjects[i]->SetSpaceIndex(m_index);

		//bool bHaveList = m_mlpObject.Lookup(ppObjects[i]->GetTag());
		//if (false == bHaveList)
		//{
		//	CAtlArray<CGameObject*> aryTemp;
		//	aryTemp.Add(ppObjects[i]);
		//	m_mlpObject.SetAt(ppObjects[i]->GetTag(), &aryTemp);
		//	//m_mapObjlist.insert(map<const TCHAR*, list<CGameObject*>>::value_type(pObjectTag, NewObjectList));
		//	//m_mlpObject[ppObjects[i]->GetTag()]->SetCount(1);
		//}
		//else
			m_mlpObject[ppObjects[i]->GetTag()]->Add(ppObjects[i]);
		/*CAtlArray<CGameObject*>* lpDynObj;
		bool bHaveList = m_mlpObject.Lookup(ppObjects[i]->GetTag(), lpDynObj);
		if (true == bHaveList)
		{
			lpDynObj->Add(ppObjects[i]);
		}
		else
		{
			CAtlArray<CGameObject*> lpCreateDynObj;
			lpCreateDynObj.Add(ppObjects[i]);
			m_mlpObject[ppObjects[i]->GetTag()] = lpCreateDynObj;
		}
*/
		//m_mlpObject.Lookup(ppObjects[i]->GetTag(), lpDynObj);
		//m_mlpObject[ppObjects[i]->GetTag()].emplace_back(ppObjects[i]);
	}
}

void CSpace::AddObject(CGameObject* pObject) {
	if (!pObject) return;
	//��ü�� �ڽ� ��ȣ ���
	pObject->SetSpaceIndex(m_index);


	//bool bHaveList = m_mlpObject.Lookup(pObject->GetTag());
	//if (false == bHaveList)
	//{
	//	CAtlArray<CGameObject*> aryTemp;
	//	aryTemp.Add(pObject);
	//	m_mlpObject.SetAt(pObject->GetTag(), &aryTemp);
	//	//m_mapObjlist.insert(map<const TCHAR*, list<CGameObject*>>::value_type(pObjectTag, NewObjectList));
	//	//m_mlpObject[ppObjects[i]->GetTag()]->SetCount(1);
	//}
	//else
		m_mlpObject[pObject->GetTag()]->Add(pObject);
	//bool bHaveList = m_mlpObject.Lookup(pObject->GetTag());
	//if (false == bHaveList)
	//{
	//	m_mlpObject[pObject->GetTag()]->SetCount(1);
	//}
	//m_mlpObject[pObject->GetTag()]->Add(pObject);
	//m_mlpObject[pObject->GetTag()].emplace_back(pObject);
}
void CSpace::RemoveObject(CGameObject* pObject) {
	if (!pObject) return;
	//if (0 == m_lpObjects.size()) return;


	size_t iVecSize = m_mlpObject[pObject->GetTag()]->GetCount();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		if ((*m_mlpObject[pObject->GetTag()])[i] == pObject)
		{
			m_mlpObject[pObject->GetTag()]->RemoveAt(i);
			return;
		}
	}

	/*m_mlpObject[pObject->GetTag()].remove_if([&pObject](CGameObject* pO) {
		return pObject == pO;
	});*/

}

void CSpace::RemoveObject(string name) {
	POSITION ContainerPos = m_mlpObject.GetStartPosition();
	CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
	while (ContainerPos != NULL)
	{
		pOutPair = m_mlpObject.GetNext(ContainerPos);
		size_t iVecSize = pOutPair->m_value->GetCount();
		for (size_t i = 0; i < iVecSize; ++i)
		{
			if ((*pOutPair->m_value)[i]->GetName() == name)
			{
				m_mlpObject[(*pOutPair->m_value)[i]->GetTag()]->RemoveAt(i);
			}
		}
	}
	/*for (auto data : m_mlpObject) {
		for (auto pObject : data.second) {
			if (pObject->GetName() == name) {
				m_mlpObject[pObject->GetTag()].remove_if([&pObject](CGameObject* pO) {
					return pObject == pO;
				});
			}
		}
	}*/
}

CGameObject * CSpace::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance) {
	float fHitDistance = FLT_MAX;
	distance = fHitDistance;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pObj = nullptr;
	//�ڽ��� ��� ��ü�� ���ؼ� �˻�

	POSITION ContainerPos = m_mlpObject.GetStartPosition();
	CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
	while (ContainerPos != NULL)
	{
		pOutPair = m_mlpObject.GetNext(ContainerPos);
		size_t iVecSize = pOutPair->m_value->GetCount();
		for (size_t i = 0; i < iVecSize; ++i)
		{
			if ((*pOutPair->m_value)[i]->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {
				if (fNearHitDistance > fHitDistance) {
					distance = fHitDistance;//�� �����
					pObj = (*pOutPair->m_value)[i];
				}
			}
		}
	}
	//for (auto Objects : m_mlpObject) {
	//	for (auto pObject : Objects.second) {
	//		if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray�� �浹�ߴٸ�
	//			if (fNearHitDistance > fHitDistance) {//������ ���� ����� �༮�� ��
	//				distance = fHitDistance;//�� ������ ���� ����� ��ü ����
	//				pObj = pObject;
	//			}
	//		}
	//	}
	//}
	return pObj;//�ش� ��ü return
}



CSpace::CSpace() : CGameObject("space", tag::TAG_SPACE) {

}

CSpace::~CSpace()
{
}
