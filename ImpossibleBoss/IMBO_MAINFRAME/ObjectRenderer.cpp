#include "stdafx.h"
#include "ObjectRenderer.h"

bool CObjectRenderer::Begin() {
	//for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
	//	m_mObjectRenderContainer[RenderContainer.first] = RenderContainer.second;
	//}
	//
	//for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_STATIC_OBJECT]) {
	//	m_mObjectRenderContainer[RenderContainer.first] = RenderContainer.second;
	//}
	CString csName = CA2CT("terrain");
	m_pTerrainRenderContainer = RCSELLER->GetTagRenderContainer()[tag::TAG_TERRAIN][CA2CT("terrain")];
	m_pSkyBoxRenderContainer = RCSELLER->GetTagRenderContainer()[tag::TAG_SPACE][CA2CT("skybox")];

	//skybox depth stencil
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC noSkyStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_EQUAL };
	descDepth.FrontFace = noSkyStencilOp;
	descDepth.BackFace = noSkyStencilOp;
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDepthStencilState);

	return true;
}

bool CObjectRenderer::End() {
//	m_mObjectRenderContainer.clear();
	m_mObjectRenderContainer.RemoveAll();
	m_pTerrainRenderContainer = nullptr;
	m_pSkyBoxRenderContainer = nullptr;
	if (m_pd3dDepthStencilState)m_pd3dDepthStencilState->Release();
	if (m_pd3dTempDepthStencilState)m_pd3dTempDepthStencilState->Release();

	return true;
}

void CObjectRenderer::SetShaderState() {

}

void CObjectRenderer::CleanShaderState() {

	
	CAtlMap<tag, mapRC>::CPair* pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->ClearObjectList();
		}
	}




	/*for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
		pRenderContainer.second->ClearObjectList();
	}
	for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_STATIC_OBJECT]) {
		pRenderContainer.second->ClearObjectList();
	}
	for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
		pRenderContainer.second->ClearObjectList();
	}
	for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_STATIC_OBJECT]) {
		pRenderContainer.second->ClearObjectList();
	}
*/
	m_pTerrainRenderContainer->ClearObjectList();
}

void CObjectRenderer::UpdateShaderState() {

}

void CObjectRenderer::Excute(shared_ptr<CCamera> pCamera) {
	//scene�� ��� Part�� rendercontainer�ȿ� part list Render!

	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pd3dTempDepthStencilState, &m_TempStencil);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	//m_pSkyBoxRenderContainer->Render(pCamera);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dTempDepthStencilState, m_TempStencil);

	m_pTerrainRenderContainer->Render(pCamera);


	CAtlMap<tag, mapRC>::CPair* pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
		}
	}

	pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
		}
	}


	//for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_STATIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_STATIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}

	CleanShaderState();
}
void CObjectRenderer::ExcuteShadowRender(shared_ptr<CCamera> pCamera)
{
	m_pTerrainRenderContainer->Render(pCamera);
	m_pTerrainRenderContainer->ClearObjectList();

	CAtlMap<tag, mapRC>::CPair* pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_STATIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
			pInPair->m_value->ClearObjectList();
		}
	}

	pOutPair = RCSELLER->GetStempRenderContainer().Lookup(tag::TAG_DYNAMIC_OBJECT);
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			pInPair->m_value->Render(pCamera);
			pInPair->m_value->ClearObjectList();
		}
	}

	//for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_STATIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_DYNAMIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
	//for (auto pRenderContainer : RCSELLER->GetStempRenderContainer()[tag::TAG_STATIC_OBJECT]) {
	//	pRenderContainer.second->Render(pCamera);
	//}
}

void CObjectRenderer::RenderSkyBox() {
	m_pSkyBoxRenderContainer->Render(nullptr);
	m_pSkyBoxRenderContainer->ClearObjectList();
}

CObjectRenderer::CObjectRenderer() : DXObject("objectrenderer") {

}

CObjectRenderer::~CObjectRenderer() {

}
