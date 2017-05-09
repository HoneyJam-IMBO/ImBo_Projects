
#include "stdafx.h"
#include "NetworkMgr.h"

void clienterror()
{
	exit(-1);
}

bool CNetworkManager::Begin(){
	WSADATA   wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	ClientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
		
	return true;
}

bool CNetworkManager::End(){
	closesocket(ClientSocket);

	WSACleanup();

	return true;
}

bool CNetworkManager::Connect(string ip){
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	int Result = WSAConnect(ClientSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	if (Result != 0) {
		cout << "Connect Error!" << endl;
		return false;
	}

	WSAAsyncSelect(ClientSocket, GLOBALVALUEMGR->GethWnd(), WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;

	cout << "Connect !" << endl;
	return true;
}

bool CNetworkManager::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
	switch (nMessageID) {

	case WM_SOCKET:
		if (WSAGETSELECTERROR(lParam)) {
			closesocket((SOCKET)wParam);
			clienterror();
			return true;
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			ReadPacket((SOCKET)wParam);
			return true;
			break;

		case FD_WRITE:
			break;

		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			clienterror();
			return true;
			break;
		}
	}

	return false;
}

void CNetworkManager::ProcessPacket(char * ptr){
	int iPacketSize = 0;
	int iPacketType = 0;

	memcpy(&iPacketSize, ptr, PACKET_SIZE_SIZE);
	memcpy(&iPacketType, ptr + PACKET_SIZE_SIZE, PACKET_TYPE_SIZE);
	sc_packet_client_info_in_room *pClientRoomPacket;
	sc_packet_player_position *pPosPacket;
	switch (iPacketType) // ������� Ÿ�� 2����Ʈ ó������ �ߴ�
	{
	case SC_ID:
		memcpy(&g_myid, ptr + 4, 4);
		cout << g_myid << endl;
		break;
	case SC_NOTICE:
		
		break;

	case SC_ROOM_INFO:
		//sc_packet_
		break;

	case SC_CREATE_ROOM_SUCCESS:
		break;

	case SC_JOIN_ROOM_SUCCESS:
		break;

	case SC_CLIENT_IN_ROOM://����, ����, �� ���� �� �� �� �޽��� �� ó �� �� �� ��.
		pClientRoomPacket = reinterpret_cast<sc_packet_client_info_in_room*>(ptr);
		if (g_myid == pClientRoomPacket->Id) {//�׷��� �� �Ƶ����� Ȯ���ؼ� 
			//�� ���̵��
			int character = pClientRoomPacket->Character;
			bool is_ready = pClientRoomPacket->isReady;
			SCENEMGR->GetPresentScene()->SetSelSceneInfo(0, character, is_ready);
			//�� �� �ٲ��� ���� 0����
			break;
		}

		//�� �Ƶ� �ƴϸ�
		//int nCount = 0;//���� ���� ������ �ƴ��� �˾ƺ����� �̰��ؾ��Ѵ�.
		for (int i = 0; i < 3; ++i) {
			//0���� �ٸ� ��� ���� ���鼭
			if (other_id[i] == pClientRoomPacket->Id) {
				//�ִ��� �˻�
				//nCount++;
				int character = pClientRoomPacket->Character;
				bool is_ready = pClientRoomPacket->isReady;
				SCENEMGR->GetPresentScene()->SetSelSceneInfo(i+1, character, is_ready);
				//������ ���ڽ� ������ �ٲ��ش�. 
			}
			else {
				if (other_id[i] == -1) {
					other_id[i] = pClientRoomPacket->Id;//���� ���³��� id�� �ο��ϰ�
					int character = pClientRoomPacket->Character;
					bool is_ready = pClientRoomPacket->isReady;
					SCENEMGR->GetPresentScene()->SetSelSceneInfo(i + 1, character, is_ready);//0��° �ٸ����� 1���� �ڱ� �ڸ���.
					break;
																										//�װ��� ready�� �ʱ�ȭ �Ѵ�. 
				}
			}
		}
		
		break;

	case SC_ALL_READY_COMPLETE:
		// ������� �ε��ϼ���
		SCENEMGR->ChangeScene(SC_ORITOWN);
		break;

	case SC_GAME_START:
		// ������� ���� �����ϼ���
		break;

	case SC_PLAYER_POS:
		pPosPacket = reinterpret_cast<sc_packet_player_position*>(ptr);
		sc_packet_player_position temp;
		temp.Id = pPosPacket->Id;
		temp.PosX = pPosPacket->PosX;
		temp.RotY = pPosPacket->RotY;
		temp.AnimNumber = pPosPacket->AnimNumber;

		break;
	case SC_REMOVE_PLAYER:
		sc_packet_remove_player RemovePacket;
		memcpy(&RemovePacket, ptr, iPacketSize);
		//for (int i = 0; i < 3; ++i) {
		//	if (OtherPlayers[i].Id == RemovePacket.Id)
		//		memset(&OtherPlayers[i], 0, sizeof(ClientInfo));
		//}
		break;
		//case SC_CHAT:
		//   sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat *>(ptr);
		//   int other_id = my_packet->id;
		//   if (other_id == g_myid) {
		//      wcsncpy_s(player.message, my_packet->message, 256);
		//      player.message_time = GetTickCount();
		//   }
		//   else if (other_id < NPC_START) {
		//      wcsncpy_s(skelaton[other_id].message, my_packet->message, 256);
		//      skelaton[other_id].message_time = GetTickCount();
		//   }
		//   else {
		//      wcsncpy_s(npc[other_id - NPC_START].message, my_packet->message, 256);
		//      npc[other_id - NPC_START].message_time = GetTickCount();
		//   }
		//   break;


	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}



void CNetworkManager::SendPacket(void* pData, int iDataSize) {
	WSABUF SendWSABuf;
	SendWSABuf.buf = (char*)pData;
	SendWSABuf.len = iDataSize;
	DWORD dwSendBytes;
	int ret = WSASend(ClientSocket, &SendWSABuf, 1, &dwSendBytes, 0, NULL, NULL);
	if (ret == SOCKET_ERROR) {
		// WSAGetLastError();
	}

}

void CNetworkManager::ReadPacket(SOCKET sock) {
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret == SOCKET_ERROR) {
		//WSAGetLastError();
	}


	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) memcpy(&in_packet_size, ptr, PACKET_SIZE_SIZE);
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

CNetworkManager::CNetworkManager() : CSingleTonBase<CNetworkManager>("networkmanagersingleton"){

}

CNetworkManager::~CNetworkManager(){

}
