#include "stdafx.h"
#include "Scene.h"
#include "DirectXFramework.h"

bool CScene::Begin(string path){


	return true;
}

bool CScene::End(){
	
	return true;
}

void CScene::Animate(float fTimeElapsed){

}

CScene* CScene::CreateScene(string name, CDirectXFramework* pFramework, SCENE_ID eID){
	CScene* pScene = new CScene(eID);
	pScene->LoadScene(name);
	pScene->SetFrameWork(pFramework);
	return pScene;
	//return nullptr;
}

void CScene::LoadScene(string path){

	//load scene
	string name = GetFileName(path);
	char meshRoot[256] = { "../../Assets/SceneResource/" };
	wstring wsMeshRoot;
	strcat(meshRoot, name.c_str());
	string sMeshRoot{ meshRoot };

	wsMeshRoot.assign(sMeshRoot.cbegin(), sMeshRoot.cend());
	LoadResource(wsMeshRoot);
}

void CScene::LoadResource(wstring wsMeshRoot){
	//read resource root 

	//RESOURCEMGR->ReleaseAllResource();
	//RCSELLER->ClearStempRenderContainer();

	//�ش� dir�Ʒ��� ��� gjm������ �ε�
	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, wsMeshRoot, true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, wsMeshRoot, true, true, L".GJM");

	string name;
	for (auto fileName : vFile) {
		string sPath{ "" };
		sPath.assign(fileName.begin(), fileName.end());
		/*���⼭ file �̸��� ������ name�� ���ϵ��� �Ѵ�.
		file ���� Ȯ���ڸ� �����ϸ� �� ���̴�.
		sprintf(name, "�� �� ��", i);*/
		name = GetFileName(sPath);
		RESOURCEMGR->CreateMultiMesh(sPath, name);
		//sprintf(name, "StempMesh%d", i++);
	}

	RCSELLER->CreateStempRenderContainer();
}
