#include "stdafx.h"
#include "Camera.h"

//DI
bool CCamera::Begin() {

	//���� �ʱ�ȭ
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4ReflectionView, XMMatrixIdentity());

	////ProjectionMtx ����
	//GenerateProjectionMatrix(fFov, fWidth / fHeight, fNear, fFar);

	m_xmf3Right = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
	m_xmf3Up = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	m_xmf3Look = XMFLOAT3{ 0.0f, 0.0f, 1.0f };
	m_xmf3Pos = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

	//������� ����
	D3D11_BUFFER_DESC CameraBufferDesc;
	ZeroMemory(&CameraBufferDesc, sizeof(CameraBufferDesc));
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.ByteWidth = sizeof(stCameraBuffer);
	CameraBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	CameraBufferDesc.CPUAccessFlags = 0;
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&CameraBufferDesc, nullptr, &m_pViewProjectionBuffer);

	return true;
}
bool CCamera::End() {
	if (m_pViewProjectionBuffer) m_pViewProjectionBuffer->Release();

	return true;
}
void CCamera::SetPosition(XMVECTOR pos) {

	//�ٲ� ���� ���� �� 
	XMStoreFloat3(&m_xmf3Pos, pos);


	//viewMtx ������Ʈ
	UpdateViewMtx();
}
XMMATRIX CCamera::GetWorldMtx() {
	XMFLOAT4X4 xmf4x4World{
		m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0,
		m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0,
		m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0,
		m_xmf3Pos.x, m_xmf3Pos.y, m_xmf3Pos.z, 1
	};
	return XMLoadFloat4x4(&xmf4x4World);
}


void CCamera::GenerateOrthoMatrix(float fWidth, float fHeight, float fNear, float fFar){

	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixOrthographicLH(fWidth, fHeight, fNear, fFar));
	m_fFarClip = fFar;
	m_fNearClip = fNear;
	//create oriented frustum
	BoundingFrustum::CreateFromMatrix(m_OriBoundingFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
}

//projection Mtx ����
void CCamera::GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar) {
	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixPerspectiveFovLH(fFov, fRatio, fNear, fFar));

	m_fFarClip = fFar;
	m_fNearClip = fNear;
	//create oriented frustum
	BoundingFrustum::CreateFromMatrix(m_OriBoundingFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
}
//viewmtx ����
void CCamera::UpdateViewMtx() {


	XMStoreFloat4x4(&m_xmf4x4View,
		XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Pos),
			XMVectorAdd(XMLoadFloat3(&m_xmf3Pos), XMLoadFloat3(&m_xmf3At)),
			XMLoadFloat3(&m_xmf3UpDefault)));

	//update frustum
	m_OriBoundingFrustum.Transform(m_BoundingFrustum, GetWorldMtx());

	m_xmf3Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_xmf3Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_xmf3Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);
}

void CCamera::UpdateReflectionViewMtx()
{
	XMFLOAT3 xmReflePos = { m_xmf3Pos.x, -m_xmf3Pos.y, m_xmf3Pos.z };
	XMFLOAT3 xmRefleAt = { m_xmf3At.x, -m_xmf3At.y, m_xmf3At.z };
	XMStoreFloat4x4(&m_xmf4x4ReflectionView,
		XMMatrixLookAtLH(XMLoadFloat3(&xmReflePos),
			XMVectorAdd(XMLoadFloat3(&xmReflePos), XMLoadFloat3(&xmRefleAt)),
			XMLoadFloat3(&m_xmf3UpDefault)));

}

bool CCamera::IsInFrustum(BoundingBox& boundingBox) {
	//if (DISJOINT == m_BoundingFrustum.Contains(boundingBox)) return false;
	//return true;
	bool result = m_BoundingFrustum.Intersects(boundingBox);
	return result;
	//return true;
}

void CCamera::SetLookAt(XMVECTOR xmvPos, XMVECTOR xmvLookAt, XMVECTOR xmvUp)
{
	XMVECTOR vATpos = XMVectorAdd(xmvPos, xmvLookAt);

	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(xmvPos, vATpos, xmvUp));

	XMStoreFloat3(&m_xmf3Pos, xmvPos);
	XMStoreFloat3(&m_xmf3At, xmvLookAt);
	//XMStoreFloat3(&m_xmf3Pos, xmvPos);

	m_xmf3Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_xmf3Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_xmf3Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);
}

void CCamera::SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ) {
	//0,0, r, b
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	GLOBALVALUEMGR->GetDeviceContext()->RSSetViewports(1, &m_d3dViewport);

	m_rcClient.bottom = nHeight;
	m_rcClient.right = nWidth;
	m_rcClient.top = 0;
	m_rcClient.left = 0;
}
//������� ����
void CCamera::UpdateShaderState() {
	//���� ����
	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	UpdateViewMtx();
	XMMATRIX xmMtxView = XMLoadFloat4x4(&m_xmf4x4View);

	XMStoreFloat4x4(&m_stCameraBufferData.m_xmf4x4ViewProjection, XMMatrixTranspose(XMMatrixMultiply(xmMtxView, xmMtxProjection)));
	//���� ����

	//������� ������Ʈ
	GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(m_pViewProjectionBuffer, 0, NULL, &m_stCameraBufferData, 0, 0);
}

CCamera::CCamera() : DXObject("camera") {
	//ī�޶� �������
	m_pViewProjectionBuffer = nullptr;

	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixIdentity());
}
CCamera::~CCamera() {

}