#pragma once
#include "FileExporter.h"

//File Exporter�� ����ϴ� ��ü
class CTxtExporter :public CFileExporter{
public:
	virtual void WriteWstring(const wstring& data);
	virtual void WriteUINT(const UINT& data);
	virtual void WriteInt(const int& data);
	virtual void WriteFloat(const float& data);
	virtual void WriteBool(bool& b);

	//txt file ����� ��� �� ������ ���
	void WriteSpace();
	void WriteEnter();
private:
	
public:
	CTxtExporter(wstring outputPath);
	virtual ~CTxtExporter();
};
