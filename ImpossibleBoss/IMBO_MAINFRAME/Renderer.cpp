
#include "stdafx.h"
#include "Renderer.h"

bool CRenderer::Begin() {
	m_pCSGlobalBuffer = RESOURCEMGR->CreateConstantBuffer("CSGlobalBuffer", 1, sizeof(stCSGlobalBufferData), 13, BIND_CS);

	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = stencilMarkOp;
	descDepth.BackFace = stencilMarkOp;
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDepthStencilState);
	//if (false == CreateSwapChain()) return false;

	//layer
	m_pObjectRenderer = new CObjectRenderer();
	m_pObjectRenderer->Begin();
	m_pAORenderer = new CAORenderer();
	m_pAORenderer->Begin();

	m_pLightRenderer = new CLightRenderer();
	m_pLightRenderer->Begin();

	//post processing
	m_pBloomDownScale = new CBloomDownScale();	m_pBloomDownScale->Begin();
	m_pBloom = new CBloom();					m_pBloom->Begin();
	m_p16to1Blur = new CBlur();					m_p16to1Blur->Begin();
	m_p4to1Blur = new CBlur();					m_p4to1Blur->Begin();
	m_pPostProcessingFinalPass = new CPostProcessingFinalPass();	m_pPostProcessingFinalPass->Begin();
	m_pSSLR = new CSSLR();						m_pSSLR->Begin();
	m_pRefrectionRenderer = new CSSRF();		m_pRefrectionRenderer->Begin();
	//layer

	//shadow
	m_pShadow = new CShadow();
	m_pShadow->Begin();

	//UIRenderer 
	m_pUIRederer = new CUIRenderer();
	m_pUIRederer->Initialize();
	m_pWaterRenderer = new CWaterRenderer();
	m_pWaterRenderer->Begin();
	
	// render target�� depth-stencil buffer ����/ deferred texture ����
	if (!CreateRenderTargetView()) {
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("RenderTarget�̳� Depth-Stencil ���� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return false;
	}
	return true;
}

bool CRenderer::End() {
	m_pCSGlobalBuffer = nullptr;

	//if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();
	
	ReleaseForwardRenderTargets();

	//layer
	Safe_EndDelete(m_pObjectRenderer);
	Safe_EndDelete(m_pAORenderer);
	Safe_EndDelete(m_pLightRenderer);
	Safe_EndDelete(m_pBloomDownScale);
	Safe_EndDelete(m_pBloom);
	Safe_EndDelete(m_p16to1Blur);
	Safe_EndDelete(m_p4to1Blur);
	Safe_EndDelete(m_pPostProcessingFinalPass);
	Safe_EndDelete(m_pSSLR);
	Safe_EndDelete(m_pShadow);
	Safe_EndDelete(m_pWaterRenderer);
	Safe_EndDelete(m_pRefrectionRenderer);
	
	//Safe_Delete(m_pUIRederer);

	if (m_pUIRederer)
		delete m_pUIRederer;

	return true;
}

void CRenderer::PreRender()
{
	//CLEAR
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();

	//CLEAR
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3dRenderTargetView) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	

	SetMainRenderTargetView();
	if (nullptr != m_pUIRederer){
		m_pUIRederer->RenderUI();
	}

	HRESULT hr = GLOBALVALUEMGR->GetSwapChain()->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET){
		DEBUGER->DebugMessageBox("explosion", "d");	return;
	}
}


void CRenderer::Render(shared_ptr<CCamera> pCamera) {
	pCamera->SetShaderState();

	//CLEAR
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("SHADOW")->SetShaderState();
	//////

	//shadow render
	m_pShadow->RenderShadowMap(pCamera);
	
	//clear buff
	//CLEAR
	UPDATER->GetSpaceContainer()->PrepareRender(pCamera);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();
	
	DEBUGER->start_Timemeasurement();
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	//SetMainRenderTargetView();
	SetForwardRenderTargets();//gbuff�� �� rtv/ dsv set
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);
	
	//object
	pCamera->SetShaderState();
	m_pObjectRenderer->Excute(pCamera);
	
	//m_pWaterRenderer->RenderWater(pCamera, m_pd3dsrvDepthStencil);
	
	if (INPUTMGR->GetDebugMode()) { DEBUGER->DebugRender(pCamera); }
	DEBUGER->end_Timemeasurement(L"object_render");
	
	//SSAO
	DEBUGER->start_Timemeasurement();
	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);
	size_t iVecSize = m_vObjectLayerResultTexture.GetCount();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		//for (auto texture : m_vObjectLayerResultTexture) {
		//texture->SetShaderState();
		m_vObjectLayerResultTexture[i]->SetShaderState();
	}
	ID3D11ShaderResourceView* pAmbientOcclution = m_pAORenderer->Excute(pCamera, m_p4to1Blur);
	
	DEBUGER->end_Timemeasurement(L"ssao");
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pAmbientOcclution);
	
	DEBUGER->AddTexture(XMFLOAT2(500, 500), XMFLOAT2(600, 600), pAmbientOcclution);
	
	//LIGHT RENDER
	DEBUGER->start_Timemeasurement();
	//SetMainRenderTargetView();
	m_pLightRenderer->Excute(pCamera, m_pShadow);
	//for (auto texture : m_vObjectLayerResultTexture) {
	for (size_t i = 0; i < iVecSize; ++i)
	{
		//texture->CleanShaderState();
		m_vObjectLayerResultTexture[i]->CleanShaderState();
	}
	DEBUGER->end_Timemeasurement(L"light_render");
	

	//SSLR
	DEBUGER->start_Timemeasurement();
	m_pSSLR->Excute(pCamera, m_pd3drtvLight, pAmbientOcclution);
	DEBUGER->end_Timemeasurement(L"sslr");
	
	 //POST PROCESSING
	DEBUGER->start_Timemeasurement();
	SetMainRenderTargetView();
	m_vLightLayerResultTexture[0]->SetShaderState();
	PostProcessing(pCamera);
	
	size_t iLightVecSize = m_vLightLayerResultTexture.GetCount();
	for (size_t i = 0; i < iLightVecSize; ++i)
	{
	//for (auto texture : m_vLightLayerResultTexture) {
	//	texture->CleanShaderState();
		m_vLightLayerResultTexture[i]->CleanShaderState();
	}
	DEBUGER->end_Timemeasurement(L"postprocessing");
	pCamera->SetShaderState();
	m_pObjectRenderer->RenderSkyBox();
	
	//water
	DEBUGER->start_Timemeasurement();
	m_pRefrectionRenderer->Excute(pCamera, m_pd3dRenderTargetView, m_pd3ddsvReadOnlyDepthStencil, m_pd3dsrvLight, m_pd3dsrvDepthStencil, m_pd3dsrvNormal);
	DEBUGER->end_Timemeasurement(L"ssrf");


	if (nullptr != m_pUIRederer) {
		m_pUIRederer->RenderUI();
	}
	
	//DEBUG
	if (INPUTMGR->GetDebugMode()) {
		DEBUGER->AddDepthTexture(XMFLOAT2(500, 0), XMFLOAT2(750, 150), m_pd3dsrvDepthStencil);
		//	DEBUGER->AddTexture(XMFLOAT2(100, 0), XMFLOAT2(350, 250), pRFL);
		DEBUGER->AddTexture(XMFLOAT2(100, 0), XMFLOAT2(400, 300), m_vLightLayerResultTexture[0]->GetShaderResourceView());
		//m_vLightLayerResultTexture
	
		//�̰� �� ���⼭ �������.
		DEBUGER->RenderTexture();
		DEBUGER->RenderText();
	}
	else {
		DEBUGER->ClearDebuger();
	}
	//DEBUGER->ClearDebuger();


	//PRESENT
	DEBUGER->start_Timemeasurement();
	HRESULT hr = GLOBALVALUEMGR->GetSwapChain()->Present(0, 0);
	DEBUGER->end_Timemeasurement(L"present");
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		DEBUGER->DebugMessageBox("explosion", "d");
		return;
	}
}
void CRenderer::Update(float fTimeElapsed) {
	m_pBloomDownScale->SetAdaptation(fTimeElapsed);
}
void CRenderer::PostProcessing(shared_ptr<CCamera> pCamera) {
	m_pBloomDownScale->Excute();
	ID3D11ShaderResourceView* pBloomImage = m_pBloom->Excute(pCamera);
	pBloomImage = m_p16to1Blur->Excute(pBloomImage);

	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &pBloomImage);

	//rtv�� Ǯ��ũ�� ��ο� 
	m_pPostProcessingFinalPass->Excute(pCamera);

	//������ �ϱ����� ����avgLum�� ���� ����� avgLum�� ��ȯ
	m_pBloomDownScale->SwapAdaptationBuff();
}
void CRenderer::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView) {
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CRenderer::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	if (m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs, m_pd3ddsvDepthStencil);
}
void CRenderer::SetMainRenderTargetView() {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3ddsvReadOnlyDepthStencil);
}
void CRenderer::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil) {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(nRenderTarget, pd3dRTVs, pd3ddsvDepthStencil);
}

bool CRenderer::CreateRenderTargetView() {

	HRESULT hResult = S_OK;
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };
	if (FAILED(hResult = GLOBALVALUEMGR->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	//m_pd3dRenderTargetView->


	if (pd3dBackBuffer) pd3dBackBuffer->Release();
	{
		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels = 1;
		//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//----------------------------------------Resource Desc-----------------------------------------//
		//----------------------------------------TextUre Desc-----------------------------------------//
		D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
		::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTexture2DDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
		d3dTexture2DDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
		d3dTexture2DDesc.MipLevels = 1;
		d3dTexture2DDesc.ArraySize = 1;
		d3dTexture2DDesc.SampleDesc.Count = 1;
		d3dTexture2DDesc.SampleDesc.Quality = 0;
		d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DDesc.CPUAccessFlags = 0;
		d3dTexture2DDesc.MiscFlags = 0;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		//----------------------------------------TextUre Desc-----------------------------------------//
		//----------------------------------------Render Desc-----------------------------------------//
		D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		ReleaseForwardRenderTargets();

		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDepthStencil, &d3dSRVDesc, &m_pd3dsrvDepthStencil);
		//real depth stencil
		d3dDepthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
		if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvReadOnlyDepthStencil))) return(false);
		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//--------------------------------------Scene3 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene3 RTV Create-----------------------------------------//

		//�ڱ� texture set -> sampler set����
		//---------------------make texture---------------------
		//texture set to light rendercontainer
		
		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvDepthStencil};
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS | BIND_CS };
		shared_ptr<CTexture> pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.Add(pTexture);

		pd3dSRV = { m_pd3dsrvColorSpecInt };
		Slot = { 1 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.Add(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 2 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.Add(pTexture);
		
		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 3 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.Add(pTexture);
		//---------------------make texture---------------------


		//light texture����
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(m_pd3dsrvLight, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.Add(pTexture);
		//light texture����
	}
	m_pAORenderer->ResizeBuffer();
	m_pRefrectionRenderer->ResizeBuffer();
	m_pBloomDownScale->ResizeBuffer();
	m_pBloom->ResizeBuffer();
	m_p16to1Blur->ResizeBuffer(4);
	m_p4to1Blur->ResizeBuffer(2);
	m_pSSLR->ResizeBuffer();
	m_pShadow->ResizeBuffer();
	//m_pWaterRenderer->ResizeBuffer();

	return true;
}

void CRenderer::ReleaseForwardRenderTargets() {
	//texture end
	m_vObjectLayerResultTexture.RemoveAll();
	m_vLightLayerResultTexture.RemoveAll();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dtxtDepthStencil) m_pd3dtxtDepthStencil->Release();
	m_pd3dtxtDepthStencil = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3dsrvDepthStencil) m_pd3dsrvDepthStencil->Release();
	m_pd3dsrvDepthStencil = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3ddsvDepthStencil) m_pd3ddsvDepthStencil->Release();
	m_pd3ddsvDepthStencil = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;


}

bool CRenderer::ResizeBuffer() {
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();

	if (FAILED(GLOBALVALUEMGR->GetSwapChain()->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		MessageBox(nullptr, TEXT("�ٺ�"), TEXT("�ٺ�2"), MB_OK);

	//resize rtv size, deferred texture size
	CreateRenderTargetView();

	stCSGlobalBufferData* pData = (stCSGlobalBufferData*) m_pCSGlobalBuffer->Map();
	pData->nWidth = GLOBALVALUEMGR->GetrcClient().right;
	pData->nHeight= GLOBALVALUEMGR->GetrcClient().bottom;
	pData->nHalfWidth = pData->nWidth / 2;
	pData->nHalfHeight = pData->nHeight / 2;
	pData->nQuadWidth = pData->nWidth / 4;
	pData->nQuadHeight = pData->nHeight / 4;
	m_pCSGlobalBuffer->Unmap();
	m_pCSGlobalBuffer->SetShaderState();

	return true;
}

void CRenderer::LoadEffectInfo(wstring wsOutputPath, wstring wsSceneName){
	//back buffer color
	m_fBackBufferClearColor[0] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[1] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[2] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[3] = IMPORTER->ReadFloat();

	//ssao
	float fSSAORadius = IMPORTER->ReadFloat();
	float fSSAOOffsetRadius = IMPORTER->ReadFloat();
	bool bSSAOOnOff = false;
	m_pAORenderer->SetSSAOValues(bSSAOOnOff, fSSAOOffsetRadius, fSSAORadius);

	//bloom
	float fBLOOMThreshold = IMPORTER->ReadFloat();
	m_pBloomDownScale->SetBloomThreshold(fBLOOMThreshold);
	float fBLOOMMiddleGrey = IMPORTER->ReadFloat();
	float fBLOOMWhite = IMPORTER->ReadFloat();
	float fBLOOMScale = IMPORTER->ReadFloat();
	m_pPostProcessingFinalPass->SetBloomFinalPassValues(fBLOOMMiddleGrey, fBLOOMWhite, fBLOOMScale);
	//sslr
	bool bSSLROnOff = IMPORTER->ReadBool();
	float fSSLRMaxSunDist = IMPORTER->ReadFloat();
	float fSSLRInitDecay = IMPORTER->ReadFloat();
	float fSSLRDistDecay = IMPORTER->ReadFloat();
	float fSSLRMaxDeltaLen = IMPORTER->ReadFloat();
	m_pSSLR->SetSSLRValues(bSSLROnOff, fSSLRMaxSunDist, fSSLRInitDecay, fSSLRDistDecay, fSSLRMaxDeltaLen);
	
	//ssrf
	float fMinDepthBias = IMPORTER->ReadFloat();
	float fMaxDepthBias = IMPORTER->ReadFloat();;
	float fEdgeDistThreshold = IMPORTER->ReadFloat();;
	float fReflectionScale = IMPORTER->ReadFloat();;
	float fViewAngleThreshold = IMPORTER->ReadFloat();;
	float fPixelScale = IMPORTER->ReadFloat();;
	float fNumStepScale = IMPORTER->ReadFloat();;

	m_pRefrectionRenderer->SetSSRFValues(fMinDepthBias, fMaxDepthBias, fEdgeDistThreshold, fReflectionScale, fViewAngleThreshold, fPixelScale, fNumStepScale);
	m_pShadow->LoadShadow(wsOutputPath, wsSceneName);
}

CRenderer::CRenderer() :CSingleTonBase<CRenderer>("rendereringleton") {

}

CRenderer::~CRenderer() {

}
