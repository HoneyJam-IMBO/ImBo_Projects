#pragma once
#include "FileExporter.h"

//File Exporter�� ����ϴ� ��ü
class CTxtExporter :public CFileExporter{
public:
	void WriteWstring(wofstream& out, const wstring data);
	//void WriteCHAR(wofstream& out, const char* data, int size);
	void WriteUINT(wofstream& out, const UINT data);
	void WriteInt(wofstream& out, const int data);
	void WriteFloat(wofstream& out, const float data);

	//txt file ����� ��� �� ������ ���
	void WriteSpace(wofstream& out);
	void WriteEnter(wofstream& out);
	void WriteBool(wofstream& out, const bool b);
private:
	
public:
	CTxtExporter();
	~CTxtExporter();
};
