#pragma once
#include "FileImporter.h"

//File Exporter�� ����ϴ� ��ü
class CBinaryImporter :public CFileImporter {
public:
	virtual string Readstring();
	virtual UINT ReadUINT();
	virtual int ReadInt();
	virtual float ReadFloat();
	virtual bool ReadBool();

	//txt file ����� ��� �� ������ ���
	virtual void ReadSpace();
	virtual void ReadEnter();
private:

public:
	CBinaryImporter(string inputPath);
	~CBinaryImporter();
};
