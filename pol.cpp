#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

void parse(Buffer buff, char (&ref)[512]) //parsowanie - klient
{
     int i,j=0;


        ref[0]=buff.ID;
        ref[1]=' ';
        ref[2]=buff.ID_USR;
        ref[3]=' ';

    for(i=4;i<sizeof(buff.iKey);i++)
    {
        ref[i]=buff.iKey[j]; j++;
    } ref[i]=' '; i++;

    for(j=0;j<sizeof(buff.cChat);i++)
    {
        ref[i]=buff.cChat[j]; j++;
    }


}

/*int ClientThread()
{
	Buffer sbuffer;

	char buffer[sizeof(sbuffer)] = {0};



	for(;; Sleep(10))
	{
		// The server will send a struct to the client
		// containing message and ID
		// But send only accepts a char as buffer parameter
		// so here we need to recv a char buffer and then
		// we copy the content of this buffer to our struct
		if(recv(sConnect, buffer, sizeof(sbuffer), NULL))
		{
			memcpy(&sbuffer, buffer, sizeof(sbuffer));
			cout << "<Client " << sbuffer.ID << ":> " << sbuffer.cChat <<endl;
		}
	}*/

int main(int argc, char* argv[])
{
    //laczenie z serwerem
    Buffer sbuffer;
    int RetVal = 0;
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2,1);
	RetVal = WSAStartup(DllVersion, &wsaData);
	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

    SOCKADDR_IN saddr;
    SOCKET      sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port        = htons(27015);
	saddr.sin_family      = AF_INET;

    if ( connect(sock, (sockaddr*)&saddr, sizeof(sockaddr)) == SOCKET_ERROR )
    {
        cout<<"lipa";
        sock = 0;
        return -1;
    }

    char pakiet[512]= {0};
    int tm;
    cout<<pakiet[0]<<endl;
    sbuffer.ID=20;
    cout<<sbuffer.ID<<endl;
    sbuffer.ID_USR=1;

    parse(sbuffer,pakiet);
    cout<<pakiet[0]<<endl;
    tm=(int)pakiet[0];
    cout<<(int)pakiet[0]<<pakiet[1]<<(int)pakiet[2]<<endl;

  //  send(sock,pakiet,sizeof(pakiet),0);
  //  recv(sock,pakiet,sizeof(pakiet),0);


    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}

/*
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
using namespace std;


SOCKADDR_IN addr;

SOCKET sConnect;

// For this we need to send two information at one time:
// 1. The main message
// 2. The ID

// To send more than one information I will use a struct
struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

int ClientThread()
{
	Buffer sbuffer;

	char buffer[sizeof(sbuffer)] = {0};



	for(;; Sleep(10))
	{
		// The server will send a struct to the client
		// containing message and ID
		// But send only accepts a char as buffer parameter
		// so here we need to recv a char buffer and then
		// we copy the content of this buffer to our struct
		if(recv(sConnect, buffer, sizeof(sbuffer), NULL))
		{
			memcpy(&sbuffer, buffer, sizeof(sbuffer));
			cout << "<Client " << sbuffer.ID << ":> " << sbuffer.cChat <<endl;
		}
	}

	return 0;
}*/








/*
int main()
{
	system("cls");

	int RetVal = 0;

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2,1);
	RetVal = WSAStartup(DllVersion, &wsaData);
	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(27015);
	addr.sin_family      = AF_INET;

	cout << "Connect to Masterserver? [ENTER]" <<endl;
	getchar();
	RetVal = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
		main();
	}
	else
	{
		int ID;
		char* cID = new char[64];
		ZeroMemory(cID, 64);

		recv(sConnect, cID, 64, NULL);
		ID = atoi(cID);

		cout << "Connected" <<endl;
		cout << "You are Client No: " << ID <<endl;

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ClientThread, NULL, NULL, NULL);

		for(;; Sleep(10))
		{
			char* buffer = new char[256];
			ZeroMemory(buffer, 256);

			cin >> buffer;
			getchar();

			send(sConnect, buffer, 256, NULL);
		}
	}

	return 0;
}*/
