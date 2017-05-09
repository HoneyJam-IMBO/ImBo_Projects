#pragma once
#include "RenderContainerFactory.h"
#include "PlaneMesh.h"
#include "TestShader.h"

//이 펙토리는 product를 return하는 컨테이너다.
class CPlaneRenderContainerFactory : public CRenderContainerFactory {

private:
	virtual void CreateContainer();
	virtual void CreateMesh();
	virtual void CreateSharedBuffer(int nMaxBuffer);
	virtual void CreateShader();
	virtual void CreateTexture();
	virtual void CreateMaterial();

public:
	CPlaneRenderContainerFactory(shared_ptr<CCamera> pCamera, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CRenderContainerFactory(pCamera, pd3dDevice, pd3dDeviceContext) {}
	virtual ~CPlaneRenderContainerFactory() {}
};
