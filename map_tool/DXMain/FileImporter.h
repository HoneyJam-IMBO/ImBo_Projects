#pragma once
#include <fstream>

//File Exporter�� ����ϴ� ��ü
class CFileImporter {
public:
	virtual string Readstring() = 0;
	virtual UINT ReadUINT() = 0;
	virtual int ReadInt() = 0;
	virtual float ReadFloat() = 0;
	virtual bool ReadBool() = 0;

	//txt file ����� ��� �� ������ ���
	virtual void ReadSpace() = 0;
	virtual void ReadEnter() = 0;
protected:
	string m_inputPath;
	ifstream m_in;

private:

public:
	CFileImporter(string inputPath) { m_inputPath = inputPath; };
	~CFileImporter() { m_in.close(); };
};

