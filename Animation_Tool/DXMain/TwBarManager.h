#pragma once
//#include "Camera.h"
#include "SingleTon.h"

class CTwBarManager : public CSingleTonBase<CTwBarManager> {
public:
	bool Begin(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, const char* def);
	bool End();

	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void Render();

	void AddBar(const char* barName);
	void SetParam(const char* barName, const char* paramName, TwParamValueType type, UINT inValueCnt, const void* inValue);
	//������ ���� ������� �̰ɷ� ���� ������ struct
	// get�Լ����� TwŸ�Կ� �°� ����->ui�� ó��->Set�Լ����� �� struct�� ����
	//client data�� �����̵� �� ���ְ�, Callback�Լ��� value�� ui�� �ٲٴ� ���̴�.
	void AddVarCB(const char* barName, const char* menuName, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void* clientData, const char* def);
	//�ڷ����� ������ �ִ°�� �̰ɷ�
	void AddVarRW(const char* barName, const char* menuName, TwType type, void* var, const char* def);

/*
	����� ���� Ʋ�ε� �̰� Tw�ڷ����� ���� �༮�̾���� ���ʹϾ��̳�. color��
*/
	//rotation float3
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//rotation object float4
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//color 
	void AddColorBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//bool botton o/x 
	void AddBoolBar(const char* barName, const char* groupName, const char* menuName, void* var);
	void AddBoolBarCB(const char* barName, const char* groupName, const char* menuName, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void* clientData);
	void AddMinMaxBarRW(const char* barName, const char* groupName, const char* menuName, void* var,
		float min=0.f, float max=1.f, float step = 0.1f);
	void AddMinMaxBarCB(const char* barName, const char* groupName, const char* menuName, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void* clientData,
		float min = 0.f, float max = 1.f, float step = 0.1f);
	//button ui
	void AddButtonCB(const char* barName, const char* groupName, const char* menuName, TwButtonCallback buttonCallback, void* clientData);
	//saperator ui �׳� ������ ��
	void AddSeparator(const char* barName, const char* groupName, const char* menuName);

	//set param func
	void SetBarSize(const char* barName, int x, int y);
	void SetBarColor(const char* barName, int r, int g, int b);
	void SetBarPosition(const char* barName, int x, int y);
	void SetBarRefresh(const char* barName, float refreshTime);
	void SetBarVisible(const char* barName, bool b);
	void SetBarResizable(const char* barName, bool b);
	void SetBarContained(const char* barName, bool b);
	void SetBarMovable(const char* barName, bool b);

	//custom func
	//rotation float3 bar!
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);
	//rotation quaternion bar!
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);
	//rotation float3 value bar ��Ȯ�� ���� �Է� ������ min max bar
	void AddRotationMinMaxBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);
	//position bar!
	void AddPositionBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj,
		float min = 0.0f, float max = 1.0f, float step = 0.01f);
	void AddOBBBar(const char* barName, const char* groupName, const char* menuName, BoundingOrientedBox* pOBB);
	void AddScaleBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj,
		float min = 0.0f, float max = 1.0f, float step = 0.01f);
	//bounding box active bar
	//void AddBoundingBoxActiveBar(const char* barName, const char* groupName, const char* menuName, CFbxJointData* pJoint);
	//custom func

	void DeleteBar(const char* barName);
	void DeleteAllBars();
	void DeleteVar(const char* barName, const char* menuName);
	void DeleteAllVars(const char* barName);
private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;

	map<string, TwBar*> m_mTwBar;
	using pairTwBar = pair<string, TwBar*>;

public:
	CTwBarManager() : CSingleTonBase<CTwBarManager>("twbarmanagersingleton") {}

};

//callback
void TW_CALL SetQuaternionToTwBar(const void *value, void * clientData);
void TW_CALL GetQuaternionToTwBar(void *value, void * clientData);

//pos
void TW_CALL SetPositionXToTwBar(const void *value, void * clientData);
void TW_CALL GetPositionXToTwBar(void *value, void * clientData);
void TW_CALL SetPositionYToTwBar(const void *value, void * clientData);
void TW_CALL GetPositionYToTwBar(void *value, void * clientData);
void TW_CALL SetPositionZToTwBar(const void *value, void * clientData);
void TW_CALL GetPositionZToTwBar(void *value, void * clientData);

//rotation
void TW_CALL SetRotationXToTwBar(const void *value, void * clientData);
void TW_CALL GetRotationXToTwBar(void *value, void * clientData);
void TW_CALL SetRotationYToTwBar(const void *value, void * clientData);
void TW_CALL GetRotationYToTwBar(void *value, void * clientData);
void TW_CALL SetRotationZToTwBar(const void *value, void * clientData);
void TW_CALL GetRotationZToTwBar(void *value, void * clientData);


void TW_CALL SetScaleToTwBar(const void *value, void * clientData);
void TW_CALL GetScaleToTwBar(void *value, void * clientData);
void TW_CALL SetScaleXToTwBar(const void *value, void * clientData);
void TW_CALL GetScaleXToTwBar(void *value, void * clientData);
void TW_CALL SetScaleYToTwBar(const void *value, void * clientData);
void TW_CALL GetScaleYToTwBar(void *value, void * clientData);
void TW_CALL SetScaleZToTwBar(const void *value, void * clientData);
void TW_CALL GetScaleZToTwBar(void *value, void * clientData);

//bounding box active bar
//void TW_CALL SetBoundingBoxActiveToTwBar(const void *value, void * clientData);
void TW_CALL GetBoundingBoxActiveToTwBar(void *value, void * clientData);
