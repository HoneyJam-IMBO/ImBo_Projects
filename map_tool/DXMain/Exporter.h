#pragma once
//#include "Camera.h"
#include "SingleTon.h"
#include "FileExporter.h"
#include "BinaryExporter.h"
#include "TxtExporter.h"
#include "dds.h"
#include <fstream>
//bitmap
//#define _AFXDLL
//#include <afx.h>
//bitmap

struct Pixel24 {
	byte b;
	byte g;
	byte r;
};

struct Pixel32 {
	byte a;
	byte b;
	byte g;
	byte r;
};
class CExporter : public CSingleTonBase<CExporter> {
public:
	bool Begin(wstring outputPath);//file open�ϱ�
	virtual bool End();//file close
	void ExportFbxObject(CTestObject* pFBXObject);
	void WriteFBXMeshData(CTestObject* pFBXObject);
	void WriteFBXAnimaterData(CTestObject* pFBXObject);
	void WriteAllFBXAnimationInfo(CTestObject* pFBXObject);

	void WriteWCHAR(const WCHAR* data);
	void WriteCHAR(const char* data);
	void WriteWstring(const wstring& data);
	void Writestring(const string& data);
	void WriteUINT(const UINT& data);
	void WriteInt(const int& data);
	void WriteFloat(const float& data);
	void WriteFloat4x4(const XMFLOAT4X4& xmf4x4);
	void WriteXMMatrix(const XMMATRIX& xmMtx);
	void WriteBool(bool& b);
	//txt file ����� ��� �� ������ ���
	void WriteSpace();
	void WriteEnter();

	void MakeBitmap24(const WCHAR* fileName, Pixel24* pData, UINT nWidth, UINT nHeight);
	void MakeBitmap24(const WCHAR* fileName, float* pData, UINT nWidth, UINT nHeight);
	void MakeBitmap32(const WCHAR* fileName, Pixel32* pData, UINT nWidth, UINT nHeight);
	void MakeSRVTexture(ID3D11ShaderResourceView* pSRV, wstring wFileName);
private:
	wstring m_outputPath;

	ofstream m_bitOut;
	CFileExporter* m_pFilrExporter{ nullptr };
public:
	CExporter() : CSingleTonBase<CExporter>("exportersingleton") {}

};
