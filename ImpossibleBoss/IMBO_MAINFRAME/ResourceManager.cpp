#include "stdafx.h"
#include "ResourceManager.h"

bool CResourceManager::Begin() {
	CreateSamplers();
	CreateTextures();
	CreateRenderShaders();
	CreateBuffers();
	CreateGlobalBuffers();
	CreateMaterials();
	CreateMeshs();
	CreateAnimaters();
	CreateGJMResources();
	CreateFBXResources();

	return true;
}

bool CResourceManager::End() {
	ReleaseSamplers();
	ReleaseTextures();
	ReleaseRenderShaders();
	ReleaseBuffers();
	ReleaseGlobalBuffers();
	ReleaseMaterials();
	ReleaseMeshs();
	ReleaseStempMeshs();
	ReleaseAnimaters();

	return true;
}

void CResourceManager::CreateSamplers() {
	//make sampler
	//WRAP
	CreateSampler("WRAP_LINEAR", 0, BIND_DS | BIND_PS, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_COMPARISON_NEVER, 0, 0);
	CreateSampler("WRAP_POINT", 1, BIND_DS | BIND_PS, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_COMPARISON_NEVER, 0, 0);
	//CLAMP
	CreateSampler("CLAMP_LINEAR", 2, BIND_DS | BIND_PS, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_COMPARISON_NEVER, 0, 0);
	CreateSampler("CLAMP_POINT", 3, BIND_DS | BIND_PS, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_COMPARISON_NEVER, 0, 0);
	CreateSampler("SHADOW", 4, BIND_PS, D3D11_TEXTURE_ADDRESS_BORDER, D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_COMPARISON_LESS, 0, 0, 1);


	POSITION Pos = m_mSampler.GetStartPosition();
	CAtlMap<CString, shared_ptr<CSampler>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mSampler.GetNext(Pos);
		pOutPair->m_value->SetShaderState();
	}
	//for (auto pSampler : m_mSampler) {
	//	pSampler.second->SetShaderState();
	//}
}

void CResourceManager::CreateTextures() {
	//texture
}

void CResourceManager::CreateRenderShaders() {
	//default
	CreateRenderShader("DEFAULT", TEXT("Default"),
		IE_POSITION | IE_NORMAL | IE_TEXCOORD | IE_TANGENT | IE_BINORMAL);
	//default

	//animation render shader
	CreateRenderShader("AnimationObject", TEXT("AnimationObject"),
		IE_POSITION | IE_NORMAL | IE_TEXCOORD | IE_TANGENT | IE_BINORMAL | IE_BONEWEIGHT | IE_BONEINDEX);
	//animation render shader

	//post processing shader
	CreateRenderShader("PostProcessing", L"PostProcessing",
		IE_POSITION | IE_TEXCOORD);
	//post processing shader
	//terrain
	CreateRenderShader("Terrain", L"Terrain",
		//IE_POSITION | IE_INSWORLDMTX| IE_TANGENT | IE_BITANGENT,
		IE_POSITION | IE_INSWORLDMTX,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//terrain

	//tessplane
	CreateRenderShader("SSRF", L"SSRF",
		0, BIND_VS | BIND_PS);
	CreateRenderShader("WaterT", L"TessPlane", 
		IE_POSITION | IE_INSWORLDMTX, 
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//tessplane

	//BoundingBox
	CreateRenderShader("BoundingBox", L"BoundingBox",
		IE_INSPOS | IE_INSFLOAT_C_A | IE_INSQUATERNION,
		BIND_VS | BIND_GS | BIND_PS);
	//BoundingBox

	//debug textue
	CreateRenderShader("DebugTexture", TEXT("DebugTexture"),
		IE_INSFLOAT_B_A | IE_INSFLOAT_B_B,
		BIND_VS | BIND_GS | BIND_PS);
	CreateRenderShader("DebugDepthTexture", TEXT("DebugDepthTexture"),
		IE_INSFLOAT_B_A | IE_INSFLOAT_B_B,
		BIND_VS | BIND_GS | BIND_PS);
	//shader
	CreateRenderShader("SkyBox", TEXT("SkyBox"),
		IE_POSITION | IE_TEXCOORD | IE_INSWORLDMTX);

	CreateRenderShader("UI", L"UI",
		IE_POSITION | IE_TEXCOORD);

	CreateRenderShader("Water", L"Water",
		IE_POSITION | IE_NORMAL | IE_TEXCOORD);

	//coordinatesys render shader
	CreateRenderShader("CoordinateSys", L"CoordinateSys",
		IE_POSITION | IE_INSWORLDMTX);
	//coordinatesys render shader

	//light shader
	shared_ptr<CRenderShader> pShader = CRenderShader::CreateRenderShader(L"DirectionalLight");
	//m_mRenderShader.insert(pairShader("DirectionalLight", pShader));
	m_mRenderShader.SetAt(CA2CT("DirectionalLight"), pShader);

	pShader = CRenderShader::CreateRenderShader(L"PointLight", 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("PointLight", pShader));
	m_mRenderShader.SetAt(CA2CT("PointLight"), pShader);

	LPCTSTR pDebugPointLightShaderName[] = { TEXT("VSPointLight.cso") , TEXT("HSPointLight.cso"), TEXT("DSPointLight.cso"), TEXT("PSDebugLight.cso") };
	pShader = CRenderShader::CreateRenderShader(pDebugPointLightShaderName, 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("DebugPointLight", pShader));
	m_mRenderShader.SetAt(CA2CT("DebugPointLight"), pShader);

	pShader = CRenderShader::CreateRenderShader(L"CapsuleLight", 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("CapsuleLight", pShader));
	m_mRenderShader.SetAt(CA2CT("CapsuleLight"), pShader);

	LPCTSTR pDebugCapsuleLightShaderName[] = { TEXT("VSCapsuleLight.cso") , TEXT("HSCapsuleLight.cso"), TEXT("DSCapsuleLight.cso"), TEXT("PSDebugLight.cso") };
	pShader = CRenderShader::CreateRenderShader(pDebugCapsuleLightShaderName, 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("DebugCapsuleLight", pShader));
	m_mRenderShader.SetAt(CA2CT("DebugCapsuleLight"), pShader);

	pShader = CRenderShader::CreateRenderShader(L"SpotLight", 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("SpotLight", pShader));
	m_mRenderShader.SetAt(CA2CT("SpotLight"), pShader);

	LPCTSTR pDebugSpotLightShaderName[] = { TEXT("VSSpotLight.cso") , TEXT("HSSpotLight.cso"), TEXT("DSSpotLight.cso"), TEXT("PSDebugLight.cso") };
	pShader = CRenderShader::CreateRenderShader(pDebugSpotLightShaderName, 0,
		BIND_VS | BIND_HS | BIND_DS | BIND_PS);
	//m_mRenderShader.insert(pairShader("DebugSpotLight", pShader));
	m_mRenderShader.SetAt(CA2CT("DebugSpotLight"), pShader);
	//light shader

	//lay trace shader
	pShader = CRenderShader::CreateRenderShader(L"RayTrace");
	//m_mRenderShader.insert(pairShader("RayTrace", pShader));
	m_mRenderShader.SetAt(CA2CT("RayTrace"), pShader);
	//combine shader
	pShader = CRenderShader::CreateRenderShader(L"Combine");
	//m_mRenderShader.insert(pairShader("Combine", pShader));
	m_mRenderShader.SetAt(CA2CT("Combine"), pShader);
}

void CResourceManager::CreateMeshs() {
	//mesh
	shared_ptr<CMesh> pMesh;
	pMesh = make_shared<CDirectionalLightMesh>();
	pMesh->Begin();
	m_mvMesh["DirectionalLight"].push_back(pMesh);

	pMesh = make_shared<CPointLightMesh>();
	pMesh->Begin();
	m_mvMesh["PointLight"].push_back(pMesh);

	pMesh = make_shared<CSpotLightMesh>();
	pMesh->Begin();
	m_mvMesh["SpotLight"].push_back(pMesh);

	pMesh = make_shared<CCapsuleLightMesh>();
	pMesh->Begin();
	m_mvMesh["CapsuleLight"].push_back(pMesh);

	pMesh = make_shared<CDeferredMesh>();
	pMesh->Begin();
	m_mvMesh["PostProcessing"].push_back(pMesh);

	pMesh = make_shared<CSpaceMesh>();
	pMesh->Begin();
	m_mvMesh["Space"].push_back(pMesh);

	pMesh = make_shared<CBoundingBoxMesh>();
	pMesh->Begin();
	m_mvMesh["BoundingBox"].push_back(pMesh);

	//debug
	pMesh = make_shared<CDebugTextureMesh>();
	pMesh->Begin();
	m_mvMesh["DebugTexture"].push_back(pMesh);

	//UI
	pMesh = make_shared<CDeferredMesh>();
	pMesh->Begin();
	m_mvMesh["UI"].push_back(pMesh);

	pMesh = make_shared<CCoordinateSysMesh>();
	pMesh->Begin();
	m_mvMesh["CoordinateSys"].push_back(pMesh);

	//skybox
	pMesh = make_shared<CSkyBoxMesh>();
	pMesh->Begin();
	m_mvMesh["SkyBox"].push_back(pMesh);

	//Water
	pMesh = make_shared<CWaterMesh>();
	pMesh->Begin();
	m_mvMesh["Water"].push_back(pMesh); 

	pMesh = make_shared<CTessPlaneMesh>();
	pMesh->Begin();
	m_mvMesh["WaterT"].push_back(pMesh);

}
void CResourceManager::CreateStempMeshs() {
	
}
void CResourceManager::CreateBuffers() {
	//instance buffer
	shared_ptr<CBuffer> pConstantBuffer;
	CreateInstancingBuffer("DEFAULTIB", 1000, sizeof(VS_VB_INSTANCE));
	CreateInstancingBuffer("50000IB", 50000, sizeof(VS_VB_INSTANCE));
	CreateInstancingBuffer("ONEIB", 1, sizeof(VS_VB_INSTANCE));


	CreateInstancingBuffer("BoundingBoxIB", BOUNDINGBOX_NUM, sizeof(VS_VB_BOUNDINGBOX_INSTANCE));
	CreateInstancingBuffer("CoordinateSysIB", COORD_NUM, sizeof(VS_VB_INSTANCE));
	//debug texture
	CreateInstancingBuffer("DebugTextureIB", 1, sizeof(VS_VB_DEBUG_TEXTURE_INSTANCE));
	//instance buffer


	//stemp object's instance mtx buffer
	CreateConstantBuffer("DEFAULTCB", 1000, sizeof(VS_VB_INSTANCE), VS_CB_MODEL, BIND_VS);
	//light buffer
	CreateConstantBuffer("DirectionalLightCB", 1, sizeof(DIRECTIONAL_AMBIENT_LIGHT), PS_OBJECT_BUFFER_SLOT, BIND_PS);
	CreateConstantBuffer("PointLightCB1", 1000, sizeof(POINT_LIGHT_DS_CB), DS_OBJECT_BUFFER_SLOT, BIND_DS);
	CreateConstantBuffer("PointLightCB2", 1000, sizeof(POINT_LIGHT_PS_CB), PS_OBJECT_BUFFER_SLOT, BIND_PS);
	CreateConstantBuffer("SpotLightCB1", 1000, sizeof(SPOT_LIGHT_DS_CB), DS_OBJECT_BUFFER_SLOT, BIND_DS);
	CreateConstantBuffer("SpotLightCB2", 1000, sizeof(SPOT_LIGHT_PS_CB), PS_OBJECT_BUFFER_SLOT, BIND_PS);
	CreateConstantBuffer("CapsuleLightCB1", 1000, sizeof(CAPSULE_LIGHT_DS_CB), DS_OBJECT_BUFFER_SLOT, BIND_DS);
	CreateConstantBuffer("CapsuleLightCB2", 1000, sizeof(CAPSULE_LIGHT_PS_CB), PS_OBJECT_BUFFER_SLOT, BIND_PS);

	CreateConstantBuffer("TerrainCB", 1, sizeof(TERRAIN_GLOBAL_VALUE), VS_GLOBAL_BUFFER_SLOT, BIND_VS | BIND_DS);
}

void CResourceManager::CreateGlobalBuffers() {
	
}

void CResourceManager::CreateMaterials() {

	shared_ptr<CMaterial> pMaterial;
	//material
	CreateMaterial("DEFAULT", XMFLOAT4(1.f, 1.f, 1.f, 1.0f), 1.0f, 1.0f);
	CreateMaterial("RED", XMFLOAT4(1.f, 0.f, 0.f, 1.0f), 1.0f, 1.0f);
	CreateMaterial("Core", XMFLOAT4(0.5f, 0.1f, 0.1f, 1.0f), 4.0f, 1.0f);
	CreateMaterial("Boost", XMFLOAT4(0.1f, 0.6f, 0.6f, 1.0f), 4.0f, 1.0f);
	CreateMaterial("Weapon", XMFLOAT4(0.1f, 0.7f, 0.0f, 1.0f), 4.0f, 1.0f);
	CreateMaterial("Adaptor", XMFLOAT4(0.6f, 0.5f, 0.1f, 1.0f), 4.0f, 1.0f);
	CreateMaterial("Plane", XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), 8.0f, 2.0f);
	CreateMaterial("Terrain", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	CreateMaterial("BoundingBox", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8.0f, 2.0f);
	CreateMaterial("FBX", XMFLOAT4(2.0f, 2.0f, 2.0f, 2.0f), 1.f, 1.f);
	CreateMaterial("BUNNY", XMFLOAT4(1.0f, 1.f, 1.f, 1.f), 1.f, 1.f);
	CreateMaterial("SkyBox", XMFLOAT4(5.0f, 5.0f, 5.0f, 5.0f), 1.f, 1.f);

//	RESOURCEMGR->GetMaterial("DEFAULT")->SetShaderState();
}
void CResourceManager::CreateAnimaters() {

}
void CResourceManager::CreateGJMResources() {
	//WCHAR*pwchar;
	//IMPORTER->ReadWCHAR(pwchar, 0);
}
void CResourceManager::CreateFBXResources() {

}

shared_ptr<CTexture> CResourceManager::CreateTexture(string name, UINT nTextures, _TCHAR(*ppstrFilePaths)[128], UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer) {
	shared_ptr<CTexture> pTexture = CTexture::CreateTexture(nTextures, ppstrFilePaths, Slot, BindFlag, pConstantBuffer);
	bool bIsHave = m_mTexture.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mTexture[CA2CT(name.c_str())]->End();
		m_mTexture.RemoveKey(CA2CT(name.c_str()));
	}
	m_mTexture.SetAt(CA2CT(name.c_str()), pTexture);

	return m_mTexture[CA2CT(name.c_str())];
	/*if (m_mTexture.find(name) != m_mTexture.end()) {
	m_mTexture[name]->End();
	m_mTexture.erase(name);
	}
	m_mTexture.insert(pairTexture(name, pTexture))*/;
	//return m_mTexture[name];
}

shared_ptr<CTexture> CResourceManager::CreateTexture(string name, UINT nTextures, ID3D11Texture2D ** ppd3dTextures, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer) {
	shared_ptr<CTexture> pTexture = CTexture::CreateTexture(nTextures, ppd3dTextures, Slot, BindFlag, pConstantBuffer);
	bool bIsHave = m_mTexture.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mTexture[CA2CT(name.c_str())]->End();
		m_mTexture.RemoveKey(CA2CT(name.c_str()));
	}
	m_mTexture.SetAt(CA2CT(name.c_str()), pTexture);

	return m_mTexture[CA2CT(name.c_str())];
	/*if (m_mTexture.find(name) != m_mTexture.end()) {
		m_mTexture[name]->End();
		m_mTexture.erase(name);
	}
	m_mTexture.insert(pairTexture(name, pTexture));
	return m_mTexture[name];*/
}

shared_ptr<CTexture> CResourceManager::CreateTexture(string name, _TCHAR(pstrFilePath)[128], UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer) {
	shared_ptr<CTexture> pTexture = CTexture::CreateTexture(pstrFilePath, Slot, BindFlag, pConstantBuffer);
	bool bIsHave = m_mTexture.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mTexture[CA2CT(name.c_str())]->End();
		m_mTexture.RemoveKey(CA2CT(name.c_str()));
	}
	m_mTexture.SetAt(CA2CT(name.c_str()), pTexture);
	return m_mTexture[CA2CT(name.c_str())];
	/*return m_mTexture[CA2CT(name.c_str())];
	if (m_mTexture.find(name) != m_mTexture.end()) {
		m_mTexture[name]->End();
		m_mTexture.erase(name);
	}
	m_mTexture.insert(pairTexture(name, pTexture));
	return m_mTexture[name];*/
}

shared_ptr<CTexture> CResourceManager::CreateTexture(string name, ID3D11ShaderResourceView * pShaderResourceView, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer) {
	shared_ptr<CTexture> pTexture = CTexture::CreateTexture(pShaderResourceView, Slot, BindFlag, pConstantBuffer);
	bool bIsHave = m_mTexture.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mTexture[CA2CT(name.c_str())]->End();
		m_mTexture.RemoveKey(CA2CT(name.c_str()));
	}
	m_mTexture.SetAt(CA2CT(name.c_str()), pTexture);

	return m_mTexture[CA2CT(name.c_str())];
	/*if (m_mTexture.find(name) != m_mTexture.end()) {
		m_mTexture[name]->End();
		m_mTexture.erase(name);
	}
	m_mTexture.insert(pairTexture(name, pTexture));
	return m_mTexture[name];*/
}

shared_ptr<CSampler> CResourceManager::CreateSampler(string name, UINT Slot, UINT BindFlags, D3D11_TEXTURE_ADDRESS_MODE Mode, D3D11_FILTER Filter, D3D11_COMPARISON_FUNC ComparisionFunc, float MinLOD, float MaxLOD, float BorderColor) {
	shared_ptr<CSampler> pSampler = CSampler::CreateSampler(Slot, BindFlags, Mode, Filter, ComparisionFunc, MinLOD, MaxLOD, BorderColor);
	/*if (m_mSampler.find(name) != m_mSampler.end()) {
		m_mSampler[name]->End();
		m_mSampler.erase(name);
	}
	m_mSampler.insert(pairSampler(name, pSampler));
	return m_mSampler[name];*/
	bool bIsHave = m_mSampler.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mSampler[CA2CT(name.c_str())]->End();
		m_mSampler.RemoveKey(CA2CT(name.c_str()));
	}
	m_mSampler.SetAt(CA2CT(name.c_str()), pSampler);

	return m_mSampler[CA2CT(name.c_str())];
}

shared_ptr<CRenderShader> CResourceManager::CreateRenderShader(string name, LPCTSTR ShaderName, UINT InputElementFlag, UINT BindFlag) {
	shared_ptr<CRenderShader> pShader = CRenderShader::CreateRenderShader(ShaderName, InputElementFlag, BindFlag);

	/*if (m_mRenderShader.find(name) != m_mRenderShader.end()) {
		m_mRenderShader[name]->End();
		m_mRenderShader.erase(name);
	}
	m_mRenderShader.insert(pairShader(name, pShader));
	return m_mRenderShader[name];*/
	bool bIsHave = m_mRenderShader.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mRenderShader[CA2CT(name.c_str())]->End();
		m_mRenderShader.RemoveKey(CA2CT(name.c_str()));
	}
	m_mRenderShader.SetAt(CA2CT(name.c_str()), pShader);

	return m_mRenderShader[CA2CT(name.c_str())];
}

shared_ptr<CBuffer> CResourceManager::CreateConstantBuffer(string name, UINT nObject, UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset) {
	shared_ptr<CBuffer> pBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag, Offset);
	//if (m_mBuffer.find(name) != m_mBuffer.end()) {//������ �����ǰ��� ��ü
	//	m_mBuffer[name]->End();
	//	m_mBuffer.erase(name);
	//}

	//m_mBuffer.insert(pairBuffer(name, pBuffer));

	//return m_mBuffer[name];
	bool bIsHave = m_mBuffer.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mBuffer[CA2CT(name.c_str())]->End();
		m_mBuffer.RemoveKey(CA2CT(name.c_str()));
	}
	m_mBuffer.SetAt(CA2CT(name.c_str()), pBuffer);

	return m_mBuffer[CA2CT(name.c_str())];
}

shared_ptr<CBuffer> CResourceManager::CreateInstancingBuffer(string name, UINT nObject, UINT BufferStride, UINT Offset) {
	shared_ptr<CBuffer> pBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride, Offset);
	//if (m_mBuffer.find(name) != m_mBuffer.end()) {//������ �����ǰ��� ��ü
	//	m_mBuffer[name]->End();
	//	m_mBuffer.erase(name);
	//}
	//m_mBuffer.insert(pairBuffer(name, pBuffer));
	//return m_mBuffer[name];
	bool bIsHave = m_mBuffer.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mBuffer[CA2CT(name.c_str())]->End();
		m_mBuffer.RemoveKey(CA2CT(name.c_str()));
	}
	m_mBuffer.SetAt(CA2CT(name.c_str()), pBuffer);

	return m_mBuffer[CA2CT(name.c_str())];
}

shared_ptr<CBuffer> CResourceManager::CreateGlobalBuffer(string name, UINT nObject, UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset) {
	shared_ptr<CBuffer> pBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag, Offset);
	/*if (m_mGlobalBuffer.find(name) != m_mGlobalBuffer.end()) {
		m_mGlobalBuffer[name]->End();
		m_mGlobalBuffer.erase(name);
	}
	m_mGlobalBuffer.insert(pairBuffer(name, pBuffer));
	return m_mGlobalBuffer[name];*/
	bool bIsHave = m_mGlobalBuffer.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mGlobalBuffer[CA2CT(name.c_str())]->End();
		m_mGlobalBuffer.RemoveKey(CA2CT(name.c_str()));
	}
	m_mGlobalBuffer.SetAt(CA2CT(name.c_str()), pBuffer);

	return m_mGlobalBuffer[CA2CT(name.c_str())];
}

shared_ptr<CMaterial> CResourceManager::CreateMaterial(string name, XMFLOAT4 color, float specExp, float specIntensity) {
	shared_ptr<CMaterial> pMaterial = CMaterial::CreateMaterial(color, specExp, specIntensity);;
	/*if (m_mMaterial.find(name) != m_mMaterial.end()) {
		m_mMaterial[name]->End();
		m_mMaterial.erase(name);
	}
	m_mMaterial.insert(pairMaterial(name, pMaterial));
	return m_mMaterial[name];*/
	bool bIsHave = m_mMaterial.Lookup(CA2CT(name.c_str()));
	if (bIsHave)
	{
		m_mMaterial[CA2CT(name.c_str())]->End();
		m_mMaterial.RemoveKey(CA2CT(name.c_str()));
	}
	m_mMaterial.SetAt(CA2CT(name.c_str()), pMaterial);

	return m_mMaterial[CA2CT(name.c_str())];
}

void CResourceManager::CreateAnimater(string path, string animaterName) {

}

UINT CResourceManager::CreateMultiMesh(string path, string name) {
	if (m_mvStempMesh.find(name) != m_mvStempMesh.end()) {
		for (auto pMesh : m_mvStempMesh[name]) {
			pMesh->End();
		}
		m_mvStempMesh[name].clear();
		m_mvStempMesh.erase(name);
	}

	wstring ws{ L"" };
	ws.assign(path.cbegin(), path.cend());
	wstring extention{ PathFindExtension(ws.c_str()) };

	if (L".FBX" == extention || L".fbx" == extention) {
		return CreateFBXResource(path, name);
	}
	else if (L".GJM" == extention || L".gjm" == extention || L".txt" == extention) {
		return CreateGJMResource(path, name);
	}

	//return -1;
}
UINT CResourceManager::CreateGJMResource(string path, string name) {
	IMPORTER->Begin(path);
	char pName[20];

	bool bHasAnimation = IMPORTER->ReadBool();

	shared_ptr<CMesh> pMesh;
	UINT nMeshCnt = IMPORTER->ReadUINT();
	tag t = (tag)IMPORTER->ReadUINT();
	for (UINT i = 0; i < nMeshCnt; ++i) {
		sprintf(pName, "%s", name.c_str());
		pMesh = CFileBasedMesh::CreateMeshFromGJMFile(pName, i, bHasAnimation);
		pMesh->SetTag(t);
		m_mvStempMesh[name.c_str()].push_back(pMesh);
	}
	if (false == bHasAnimation) {
		IMPORTER->End();
		return nMeshCnt;
	}

	//animater
	sprintf(pName, "%s", name.c_str());
	shared_ptr<CAnimater> pAnimater = CAnimater::CreateAnimaterFromGJMFile();
	//m_mAnimater.insert(pairAnimater(pName, pAnimater));
	m_mAnimater.SetAt(CA2CT(name.c_str()), pAnimater);

	//animation info
	int animationCnt = IMPORTER->ReadInt();
	for (int i = 0; i < animationCnt; ++i) {
		CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromGJMFile(pAnimater);
	}//end animation info for

	IMPORTER->End();
	return nMeshCnt;

}
UINT CResourceManager::CreateFBXResource(string path, string name) {
//	string sPath{ "" };
//	sPath.assign(path.cbegin(), path.cend());
//
//	//multi mesh data load
//	FBXIMPORTER->Begin(sPath);
//	char pName[20];
//
//	shared_ptr<CFileBasedMesh> pFBXMesh;
//	//	int i = FBXIMPORTER->GetMeshCnt();
//	if (FBXIMPORTER->GetHasAnimation()) {
//
//		for (UINT i = 0; i < FBXIMPORTER->GetMeshCnt(); ++i) {
//			sprintf(pName, "%s%d", name.c_str(), i);
//			pFBXMesh = CFileBasedMesh::CreateMeshFromFBXFile(name, i);
//			pFBXMesh->SetTag(TAG_DYNAMIC_OBJECT);
//			m_mvStempMesh[name.c_str()].push_back(pFBXMesh);
//		}
//
//		sprintf(pName, "%s", name.c_str());
//		shared_ptr<CAnimater> pAnimater = CAnimater::CreateAnimaterFromFBXFile(true);
//		CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromFBXFile(pAnimater);
//		m_mAnimater.insert(pairAnimater(pName, pAnimater));
//	}
//	else {
//		for (UINT j = 0; j < FBXIMPORTER->GetMeshCnt(); ++j) {
//			sprintf(pName, "%s%d", name.c_str(), j);
//			pFBXMesh = CFileBasedMesh::CreateMeshFromFBXFile(name, j);
//			pFBXMesh->SetTag(TAG_STATIC_OBJECT);
//			m_mvStempMesh[name.c_str()].push_back(pFBXMesh);
//		}
//	}
//
//	int meshCnt = FBXIMPORTER->GetMeshCnt();
//	FBXIMPORTER->End();
//	return meshCnt;
	return 0;
}

void CResourceManager::CreateTerrainMesh(float fOneSpaceSize, string name) {
	if (m_mvMesh.find(name) != m_mvMesh.end()) {
		for (auto pMesh : m_mvMesh[name]) {
			pMesh->End();
		}
		m_mvMesh[name].clear();
	}
	m_mvMesh[name].push_back(CTerrainMesh::CreateTerrainMesh(fOneSpaceSize));
}

//Release
void CResourceManager::ReleaseSamplers() {
	POSITION Pos = m_mSampler.GetStartPosition();
	CAtlMap<CString, shared_ptr<CSampler>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mSampler.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->CleanShaderState();
			pOutPair->m_value->End();
		}		
	}
	m_mSampler.RemoveAll();
	//for (auto data : m_mSampler) {
	//	if (data.second) {
	//		data.second->CleanShaderState();
	//		data.second->End();
	//	}
	//}
	//m_mSampler.clear();
}

void CResourceManager::ReleaseTextures() {

	POSITION Pos = m_mTexture.GetStartPosition();
	CAtlMap<CString, shared_ptr<CTexture>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mTexture.GetNext(Pos);
		pOutPair->m_value->CleanShaderState();
		pOutPair->m_value->End();
		
	}
	m_mTexture.RemoveAll();
	/*for (auto data : m_mTexture) {
		if (data.second)data.second->End();
	}
	m_mTexture.clear();*/
}

void CResourceManager::ReleaseRenderShaders() {
	POSITION Pos = m_mRenderShader.GetStartPosition();
	CAtlMap<CString, shared_ptr<CRenderShader>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mRenderShader.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->End();
		}
	}
	m_mRenderShader.RemoveAll();
	//for (auto data : m_mRenderShader) {
	//	if (data.second)data.second->End();
	//}
	//m_mRenderShader.clear();
}

void CResourceManager::ReleaseMeshs() {
	for (auto vMesh : m_mvMesh) {
		for (auto pMesh : vMesh.second) {
			if (pMesh) pMesh->End();
		}
		vMesh.second.clear();
	}
	m_mvMesh.clear();
}

void CResourceManager::ReleaseStempMeshs() {
	for (auto vMesh : m_mvStempMesh) {
		for (auto pMesh : vMesh.second) {
			if (pMesh) pMesh->End();
		}
		vMesh.second.clear();
	}
	m_mvStempMesh.clear();
}

void CResourceManager::ReleaseAllResource()
{
	ReleaseTextures();
	ReleaseStempMeshs();		//gjm
	ReleaseAnimaters();
}

void CResourceManager::ReleaseMesh(string name) {
	map<string, vector<shared_ptr<CMesh>>> ::iterator iter = m_mvMesh.find(name);
	if (iter != m_mvMesh.end()) {
		for (auto pMesh : m_mvMesh[name]) {
			pMesh->End();
		}
		m_mvMesh[name].clear();
	}
	//delete iter->second.get();
	m_mvMesh.erase(iter);
}

void CResourceManager::ReleaseStempMesh(string name) {
	map<string, vector<shared_ptr<CMesh>>> ::iterator iter = m_mvStempMesh.find(name);
	if (iter != m_mvStempMesh.end()) {
		for (auto pMesh : m_mvStempMesh[name]) {
			pMesh->End();
		}
		m_mvStempMesh[name].clear();
	}
	//delete iter->second.get();
	m_mvMesh.erase(iter);
}

void CResourceManager::ReleaseAnimater(string name) {

	shared_ptr<CAnimater> isHave;
	bool bFind = m_mAnimater.Lookup(CA2CT(name.c_str()), isHave);
	if (bFind)
	{
		isHave->End();
		m_mAnimater.RemoveKey(CA2CT(name.c_str()));
	}
	/*map<string, shared_ptr<CAnimater>> ::iterator iter = m_mAnimater.find(name);
	(iter->second)->End();
	m_mAnimater.erase(iter);*/
}

void CResourceManager::ReleaseBuffers() {
	POSITION Pos = m_mBuffer.GetStartPosition();
	CAtlMap<CString, shared_ptr<CBuffer>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mBuffer.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->End();
		}
	}
	m_mBuffer.RemoveAll();

	//for (auto data : m_mBuffer) {
	//	if (data.second)data.second->End();
	//}
	//m_mBuffer.clear();
}

void CResourceManager::ReleaseGlobalBuffers() {
	POSITION Pos = m_mGlobalBuffer.GetStartPosition();
	CAtlMap<CString, shared_ptr<CBuffer>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mGlobalBuffer.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->End();
		}
	}
	m_mGlobalBuffer.RemoveAll();
/*
	for (auto data : m_mGlobalBuffer) {
		if (data.second)data.second->End();
	}
	m_mGlobalBuffer.clear();*/
}

void CResourceManager::ReleaseMaterials() {
	POSITION Pos = m_mMaterial.GetStartPosition();
	CAtlMap<CString, shared_ptr<CMaterial>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mMaterial.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->End();
		}
	}
	m_mMaterial.RemoveAll();
	/*for (auto data : m_mMaterial) {
		if (data.second)data.second->End();
	}
	m_mMaterial.clear();*/
}

void CResourceManager::ReleaseAnimaters() {
	POSITION Pos = m_mAnimater.GetStartPosition();
	CAtlMap<CString, shared_ptr<CAnimater>>::CPair*		pOutPair = NULL;
	while (Pos != NULL)
	{
		pOutPair = m_mAnimater.GetNext(Pos);
		if (pOutPair->m_value)
		{
			pOutPair->m_value->End();
		}
	}
	m_mAnimater.RemoveAll();
	//for (auto data : m_mAnimater) {
	//	if (data.second)data.second->End();
	//}
	//m_mAnimater.clear();
}
CResourceManager::CResourceManager() :CSingleTonBase<CResourceManager>("resourcemanager") {

}

CResourceManager::~CResourceManager() {

}
