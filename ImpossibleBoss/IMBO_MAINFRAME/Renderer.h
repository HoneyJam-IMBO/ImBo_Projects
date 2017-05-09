#pragma once

#include "SingleTon.h"

//
#include "DirectionalLight.h"
#include "TerrainContainer.h"
#include "SkyBoxContainer.h"
#include "ObjectRenderer.h"
#include "AORenderer.h"
#include "LightRenderer.h"
#include "BloomDownScale.h"
#include "Bloom.h"
#include "Blur.h"
#include "PostProcessingFinalPass.h"
#include "SSLR.h"
#include "SSRF.h"

#include "Shadow.h"
#include "UIRenderer.h"
#include "WaterRenderer.h"

class CDirectXFramework;

struct stCSGlobalBufferData {
	UINT nWidth{ 0 };//1
	UINT nHeight{ 0 };
	UINT nHalfWidth{ 0 };//1/2
	UINT nHalfHeight{ 0 };
	UINT nQuadWidth{ 0 };//1/4
	UINT nQuadHeight{ 0 };
	UINT pad[2];
};
class CRenderer : public CSingleTonBase<CRenderer> {

public:
	bool Begin();
	bool End();


	void PreRender();
	void Render(shared_ptr<CCamera> pCamera);
	void PostProcessing(shared_ptr<CCamera> pCamera);
	void Update(float fTimeElapsed);

	void SetForwardRenderTargets();
	void SetMainRenderTargetView();
	void SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil);

	//bool CreateSwapChain();
	bool CreateRenderTargetView();
	void ReleaseForwardRenderTargets();
	void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView);

	bool ResizeBuffer();
	void SetFramework(CDirectXFramework* pFramework) { m_pFramework = pFramework; }

	CRenderContainer*	GetTerrainRenderContainer() { return m_pObjectRenderer->GetTerrainRenderContainer(); }
	CRenderContainer*	GetSkyBoxRenderContainer() { return m_pObjectRenderer->GetSkyBoxRenderContainer(); }
	CObjectRenderer*	GetObjectRenderer() { return m_pObjectRenderer; }
	CShadow*			GetShadow() { return m_pShadow; }
	CUIRenderer*		GetUIRenderer() { return m_pUIRederer; }

	void LoadEffectInfo(wstring wsOutputPath, wstring wsSceneName);
private:
	shared_ptr<CBuffer> m_pCSGlobalBuffer{ nullptr };
	CDirectXFramework* m_pFramework{ nullptr };
	CRenderContainer* m_pTerrainRenderContainer{ nullptr };
	CRenderContainer* m_pSkyBoxRenderContainer{ nullptr };

	//IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };
	ID3D11RenderTargetView	*	m_pd3dRenderTargetView{ nullptr };
	
	ID3D11DepthStencilState* m_pd3dDepthStencilState{ nullptr };
	ID3D11DepthStencilView		*m_pd3ddsvReadOnlyDepthStencil{ nullptr };

	//---------------------------deferred rendering----------------------
	ID3D11Texture2D			 *m_pd3dtxtDepthStencil{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };//0
	ID3D11Texture2D			 *m_pd3dtxtNormal{ nullptr };//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow{ nullptr };//2


	ID3D11ShaderResourceView *m_pd3dsrvDepthStencil{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal{ nullptr };//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow{ nullptr };//2

	ID3D11DepthStencilView	 *m_pd3ddsvDepthStencil{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt{ nullptr };//0
	ID3D11RenderTargetView   *m_pd3drtvNormal{ nullptr };//1
	ID3D11RenderTargetView   *m_pd3drtvSpecPow{ nullptr };//2

	//vector<shared_ptr<CTexture>> m_vObjectLayerResultTexture;
	//vector<shared_ptr<CTexture>> m_vLightLayerResultTexture;
	CAtlArray<shared_ptr<CTexture>> m_vObjectLayerResultTexture;
	CAtlArray<shared_ptr<CTexture>> m_vLightLayerResultTexture;

	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvLight{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvLight{ nullptr };
	//--------------------------light render target----------------
	//-------------------------layer-------------------------
	CObjectRenderer*	m_pObjectRenderer{ nullptr };
	CAORenderer*		m_pAORenderer{ nullptr };
	CLightRenderer*		m_pLightRenderer{ nullptr };
	CSSRF*				m_pRefrectionRenderer{ nullptr };
	CBloomDownScale*	m_pBloomDownScale{ nullptr };
	CBloom*				m_pBloom{ nullptr };
	CBlur*				m_p16to1Blur{ nullptr };
	CBlur*				m_p4to1Blur{ nullptr };
	CPostProcessingFinalPass* m_pPostProcessingFinalPass{ nullptr };
	CSSLR*				m_pSSLR{ nullptr };
	CShadow*			m_pShadow{ nullptr };

	CUIRenderer*		m_pUIRederer{ nullptr };
	CWaterRenderer*		m_pWaterRenderer{ nullptr };
	//-------------------------layer-------------------------

	//effects
	//back  buffer color
	float m_fBackBufferClearColor[4];
	
public:
	CRenderer();
	virtual ~CRenderer();
};
