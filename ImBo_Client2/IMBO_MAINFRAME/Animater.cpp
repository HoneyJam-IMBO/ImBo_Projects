#include "stdafx.h""
#include "Animater.h"

bool CAnimater::Begin() {
	m_vpAnimationInfos.clear();
//	m_pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 0, BIND_VS, 0);

	return true;
}

bool CAnimater::End() {

	size_t iVecSize = m_vpAnimationInfos.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vpAnimationInfos[i]->End();
		delete m_vpAnimationInfos[i];
	}
	/*for (auto data : m_vpAnimationInfos) {
		data->End();
		delete data;
	}*/
	m_vpAnimationInfos.clear();

	if (m_pMainBoundingBox) {
		m_pMainBoundingBox->End();
		delete m_pMainBoundingBox;
	}
	m_pMainBoundingBox = nullptr;

	if(m_pSkeletonData)m_pSkeletonData->End();
	delete m_pSkeletonData;
	m_pSkeletonData = nullptr;

	if (m_pAnimBuffer) {
		m_pAnimBuffer->End();
		delete m_pAnimBuffer;
	}
	m_pAnimBuffer = nullptr;

	return true;
}

void CAnimater::SetShaderState() {
	if (m_vpAnimationInfos.empty()) {
		m_pAnimBuffer->SetShaderState();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->SetShaderState();
}

void CAnimater::CleanShaderState() {
	if (m_vpAnimationInfos.empty()) {

		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->CleanShaderState();
}

void CAnimater::Update(float fTimeElapsed) {
	if (m_vpAnimationInfos.empty()) {
		void* pData = m_pAnimBuffer->Map();
		XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

		for (int j = 0; j < 256; ++j) {
			pAnimationData[j] = XMMatrixIdentity();
		}
		m_pAnimBuffer->Unmap();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->Update(fTimeElapsed);
}

void CAnimater::AddAnimationInfo(CAnimationInfo * pAnimationInfo) {
	//1. 넣고
	m_vpAnimationInfos.push_back(pAnimationInfo);

	//	//2. 모든 anim info를 가지고. 전체적인 Joint tree를 제작한다.
	//	CreateJointTree();
	//
	//	//3. 모든 anim info에게 새로운 joint tree를 가지고 자신의 joint를 재정렬 할 것을 요청
	//	ChangeAllAnimationInfoJointData();
}

//void CAnimater::CreateJointTree() {
//	//모든? x 처음과 끝
//	m_vJointName.clear();
//	int Lastindex = m_vpAnimationInfos.size() - 1;
//	vector<CFbxJointData>::iterator iter;
//	//기준은 항상 처음 메쉬
//	for (auto FJoint : m_vpAnimationInfos[0]->GetJoints()) {
//		m_vJointName.push_back(FJoint.GetJointName());
//	}
//	for (auto LJoint : m_vpAnimationInfos[Lastindex]->GetJoints()) {
//		iter = find_if(m_vpAnimationInfos[0]->GetJoints().begin(), m_vpAnimationInfos[0]->GetJoints().end(), [&LJoint](CFbxJointData& my) {
//			return (my.GetJointName() == LJoint.GetJointName());
//		});
//		if (iter == m_vpAnimationInfos[0]->GetJoints().end()) {
//			//없으면
//			m_vJointName.push_back(LJoint.GetJointName());
//		}
//	}
//}
//
//void CAnimater::ChangeAllAnimationInfoJointData(){
//	for (auto pAnimationInfo : m_vpAnimationInfos) {
//		pAnimationInfo->ChangeJointData(m_vJointName);
//	}
//}

//void CAnimater::DeleteAnimationInfo(UINT AnimationIndex) {
//	vector<CAnimationInfo*>::iterator iter;
//	for (iter = m_vpAnimationInfos.begin(); iter != m_vpAnimationInfos.end(); ++iter) {
//		if ((*iter)->GetAnimationIndex() == AnimationIndex) break;
//	}
//	if (iter != m_vpAnimationInfos.end()) {
//		delete (*iter);
//		m_vpAnimationInfos.erase(iter);
//		m_CurAnimationIndex = 0;
//		UINT index{ 0 };
//		//animation info 재정렬
//		for (auto pAnimationInfo : m_vpAnimationInfos) {
//			pAnimationInfo->SetAnimationIndex(index++);
//		}
//	}
//}

void CAnimater::SetCurAnimationIndex(UINT AnimationIndex) {
	if (m_CurAnimationIndex == AnimationIndex) return;
	m_CurAnimationIndex = AnimationIndex;
	ResetAnimationInfos();
}

void CAnimater::ResetAnimationInfos() {
	size_t iVecSize = m_vpAnimationInfos.size();
	for(size_t i = 0; i < iVecSize; ++i)
	{
	//for (auto pAnimationInfo : m_vpAnimationInfos) {
		m_vpAnimationInfos[i]->Reset();
		//pAnimationInfo->Reset();
	}
}

UINT CAnimater::GetAnimaterJointCnt() {
	UINT JointCnt{ 0 };

	if (false == m_vpAnimationInfos.empty()) {
		JointCnt = m_pSkeletonData->GetJointCnt();
	}

	return JointCnt;
}

 CAnimater* CAnimater::CreateAnimaterFromFBXFile(bool bHasAnimation) {
	//if (bHasAnimation) {
	//	 CAnimater* pAnimater = new CAnimater>();
	//	pAnimater->SetMeshOffsetMtx(XMMatrixIdentity());

	//	CBoundingBox* pMainBoundingBox = new CBoundingBox();
	//	pMainBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(5.f, 5.f, 5.f, 1.f));
	//	pAnimater->SetpMainAABB(pMainBoundingBox);

	//	 CBuffer* pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 10, BIND_VS);
	//	pAnimater->SetpAnimBuffer(pAnimBuffer);

	//	CSkeletonData* pSkeletonData = new CSkeletonData;
	//	*pSkeletonData = FBXIMPORTER->GetSkeletonData();
	//	pAnimater->SetpSkeletonData(pSkeletonData);

	//	pAnimater->Begin();
	//	return pAnimater;
	//}

	return nullptr;
}

 CAnimater* CAnimater::CreateAnimaterFromGJMFile(bool bHasAnimation) {

	if (bHasAnimation) {
		//animation data가 있으면
		 CAnimater* pAnimater = new CAnimater();
		XMFLOAT4X4 xmf4x4MeshOffsetMtx = IMPORTER->ReadFloat4x4();
		pAnimater->SetMeshOffsetMtx(xmf4x4MeshOffsetMtx);
		
		pAnimater->SetHead(IMPORTER->ReadInt());
		pAnimater->SetRHand(IMPORTER->ReadInt());
		pAnimater->SetLHand(IMPORTER->ReadInt());

		XMFLOAT3 xmf3Position;
		xmf3Position.x = IMPORTER->ReadFloat();
		xmf3Position.y = IMPORTER->ReadFloat();
		xmf3Position.z = IMPORTER->ReadFloat();

		XMFLOAT4 xmf4Scale;
		xmf4Scale.x = IMPORTER->ReadFloat();
		xmf4Scale.y = IMPORTER->ReadFloat();
		xmf4Scale.z = IMPORTER->ReadFloat();
		xmf4Scale.w = 1.f;

		CBoundingBox* pMainBoundingBox = new CBoundingBox();
		pMainBoundingBox->Begin(XMLoadFloat3(&xmf3Position), XMLoadFloat4(&xmf4Scale));
		pAnimater->SetpMainAABB(pMainBoundingBox);

		 CBuffer* pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 10, BIND_VS);
		pAnimater->SetpAnimBuffer(pAnimBuffer);

		CSkeletonData* pSkeletonData = new CSkeletonData;
		//set skeleton data
		UINT nJointCnt = IMPORTER->ReadUINT();
		for (UINT i = 0; i < nJointCnt; ++i) {
			CFbxJointData jointData;
			jointData.SetJointName(IMPORTER->Readstring());
			jointData.SetMyIndex(IMPORTER->ReadUINT());
			jointData.SetParentIndex(IMPORTER->ReadUINT());
			jointData.SetOffsetMtx(IMPORTER->ReadXMMatrix());
			//if (i == 0) continue;
			pSkeletonData->GetJointDatas().push_back(jointData);
		};
		//set skeleton data
		pAnimater->SetpSkeletonData(pSkeletonData);

		pAnimater->Begin();

		//CAnimationInfo* pAnimationInfo;
		//pAnimationInfo = new CAnimationInfo(pd3dDevice, pd3dDeviceContext);
		//pAnimationInfo->Begin(pAnimater);

		return pAnimater;
	}

	return nullptr;
	//return  CAnimater>();
}

CAnimater::CAnimater() : DXObject("animater") {
	XMStoreFloat4x4(&m_xmf4x4MeshOffsetMtx, XMMatrixIdentity());
}

CAnimater::~CAnimater() {

}