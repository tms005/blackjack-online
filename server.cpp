#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include <vector>
#include "engine.h"
#include "serv_functions.h"


#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////  POCZ¥TEK GLOWNEGO WATKU PROGRAMU  ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int __cdecl main(void)
{
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
	int iResult;
    int SERVER_ON=1;
	CLEAN_CLIENTS(gracze);
	CLEAN_TABLES(STOLIK);

	for (int i = 0; i < MAX_CLIENTS; i ++)
    {
		active_players[i]=0;
        gracze[i].thread = NULL;
        gracze[i].sock   = INVALID_SOCKET;
    }
	
	struct addrinfo *result = NULL;
    struct addrinfo hints;
	int recvbuflen = DEFAULT_BUFLEN;

 
  

  ///////////////////// // TUTAJ STARTUJEMY Z WINSOCKEM/////////////////////////////////
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Wyst¹pi³ b³¹d modu³u WSAStartup : %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Najpierw uzupelniamy wlasne dane
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("B³ad getaddrinfo(): %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Tworzymy gniazdo nas³uchuj¹ce:
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("B³¹d gniazda: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Nastêpnie je wi¹¿emy 
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
		
        printf("B³¹d wi¹zania gniazda: %d\n", WSAGetLastError());

		system("PAUSE");
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
	
	// teraz oczekujemy na po³¹czenie z klientami// od tad nalezy rozwazac model aplikacji: stoliki
	while(SERVER_ON)
	{
		if (iIfFreeSlot(active_players)){
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
	{
        printf("B³¹d nas³uchuj¹cego gniazda: %d\n", WSAGetLastError());

        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	int free_slot = iFindFreeSlot(active_players);
	active_players[free_slot]=1;
    // Teraz akceptujemy transmisjê
	gracze[free_slot].sock = accept(ListenSocket, NULL, NULL);
    if (gracze[active_players[free_slot]].sock == INVALID_SOCKET) {
        active_players[free_slot]=0;
		printf("Nie powiod³a siê akceptacja transmisji: %d\n", WSAGetLastError());
		
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }// istartujemy nowy watek:]

			iAktywny =1;
	gracze[free_slot].thread = CreateThread (NULL, 0, iSilnikGry, (LPVOID)&gracze[free_slot], 0, NULL);
	if(!gracze[free_slot].thread)
			{		
	            gracze[free_slot].sock = INVALID_SOCKET;
                cout << "Utworzenie watku dla klienta nie powiodlo sie.\n" << endl;
			}

		}
		}
		
	// tutaj wywolac funkcje oczyszczania..
	 HANDLE threads[MAX_CLIENTS];
    int threadsCount = 0;
	 for (int i = 0; i < MAX_CLIENTS; i++) 
        if (gracze[i].thread != NULL)
        {
            threads[threadsCount] = gracze[i].thread;
            threadsCount ++;
        }

		    if (WaitForMultipleObjects(threadsCount, threads, true, 5000) == WAIT_TIMEOUT)
			for (int i = 0; i < MAX_CLIENTS; i ++) TerminateThread (threads[i], 2);
 
    WSACleanup();
    system("pause");



    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////  KONIEC GLOWNEGO WATKU PROGRAMU  //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

