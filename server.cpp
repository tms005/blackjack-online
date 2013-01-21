#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define MAX_CLIENTS     12
#define MAX_TABLES 3


using namespace std;
typedef struct klient
{
    HANDLE thread;
    SOCKET sock;
	int ID;
	int IT;
};
klient gracze[MAX_CLIENTS];




//////////////////////////////////////////////
////////////////////////////////ZMIENNE GRY:
typedef struct table{
int ID[4]; // w kazdym stoliku max 4 graczy // std wartosc: 0
bool baCardsDealt[52];  // tutaj talia rozdanych kart
int iHouseCardCount ;  // tutaj punkty zdobyte przez krupiera
int iaHouseHand[12];	///tutaj karty krupiera // zalozenie: -1 oznacza ze gracz dolaczyl do partii przed rozpoczeciem rozgrywki
int iPlayerCardCount[4];	//tutaj liczba kart kazdego z graczy
int iPlayerCash[4];	// tutaj gotowka kazdego z graczy
int iPlayerBid[4]; // tutaj obstawiana kasa przez g
int iaPlayerHand[4][12]; // tutaj karty gracza
int iActivePlayer; // gracz ktory wlasnie w kolejce podejmuje decyzje
int iNextPlayer; // kolejny gracz ktory podejmie decyzje
} ;
table STOLIK[MAX_TABLES]; // istnieja 3 stoliki

//struktura wiadomosci
typedef struct Buffer {
short ID; //identyfikator funkcji , patrz dalej dostepne klucze
short ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika
short iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji 
char cChat[256];
};

int iZalogowany[MAX_CLIENTS]; // tablica zalogowanych graczy
int iUzytyStol[4];							//tablica wykorzystanych stolow


///////////////////////////////////////////
////////////////////////////////////////FUNKCJE:
void sbMssgCLEAR(Buffer *sbMssg); 
 void sConversion(Buffer *sbMssg, char cMessage[]);
 void rConversion(Buffer *sbMssg, char *cMessage);
 void cMessageForward(Buffer *sbMssg);
short iGeneruIdGracza(void);
int iPorownajHaslo(Buffer *sbMssg, char napis[]);
char* sWybierzHaslo(Buffer* sbMssg);
int iTranslate(Buffer* sbMssg);
///////////////////////////////////////////////////////////////
	DWORD WINAPI iSilnikGry(LPVOID ctx)
{
	int thread_ID =-1; //  tutaj przechowujemy index gracza ktory wywolal watek serwera (index w tablicy gracze)
	int iTable = -1;		//tutaj przechowujemy informacje o tym w ktorym aktualnie stoliku jest gracz ktory wywolal watek .. lub -1 gdy gracz nie nalezy do stolika
	int idStolik = -1; // a tutaj przechowujemy informacje o tym, ktore aktualnie pole ID  oraz pozostale pola odpowiadajace mu w strukturze STOLIK posiada gracz (-1 - gracz nie "dosiadl sie" jeszcze do nowej rozgrywki)
	int ret; // tutaj przechowujemy uniwersalna zmienna zwracajaca wyniki
	int iMode; // aktualnie wybierane przez gracza dzialanie
	klient * gracz=(klient*)ctx;
	int iAktywny=1; // ta zmienna reguluje do kiedy ma dzialac silnik gry
	char cMessage[275];
	Buffer sbMssg; 
	while(iAktywny)
	{
	  ret = recv( gracz->sock, cMessage, 257, 0);
	   rConversion(&sbMssg, cMessage);  // za kazdym odebraniem wiadomosci z klienta tlumacze ja z powrotem do postaci ramki struktury
	   if(ret == 0)
		{
	   //polaczenie zamkniete po stronie klienta
			if(thread_ID != -1) //czyszczenie ew. danych
			{
				if(iTable != -1)
					{
						if(idStolik!=-1)
							{
								STOLIK[iTable].ID[idStolik]=0;
								STOLIK[iTable].iPlayerBid[idStolik]=0;
								STOLIK[iTable].iPlayerCardCount[idStolik]=0;
								// tutaj jeszcze zaktualizowac pole next oraz active o ile gracz byl jednym z tych graczy
						}			
				}
			}
			return 0;
	   }
	iMode=	iTranslate(&sbMssg); // tutaj decydujemy co bedzie sie dzialo ;d
	

	//tutaj : glowna decyzja serwera .
	switch(iMode)
		{
	case 0: // tutaj wysylamy wiadomosc do swojego stolika lub do wszystkich
			sbMssgCLEAR(&sbMssg);
			cMessageForward(&sbMssg); // tutaj modyfikacja nieznaczna jedynie potrzebna do dalszego rozeslania (do zaimplementowania)
			for(int i=0;i<MAX_CLIENTS;i++) if( iTable == gracze[i].IT ) send( gracze[i].sock  , cMessage, 513, 0);
			break;

	case 1:
		{
		FILE* login= fopen("login.txt", "wt+");
		if (login==NULL) 
			{fputs("File error",stderr); exit (1);} 
		 fseek (login , 0 , SEEK_END);
		sbMssg.ID_USR = iGeneruIdGracza();
		fprintf(login, "%d %s\n",sbMssg.ID_USR, sWybierzHaslo(&sbMssg)); // tutaj do wymiany
		fclose(login);
		sConversion(&sbMssg, cMessage);	
		send(gracz->sock  , cMessage, 513, 0);
		break;


		}

	case 2:
		{
		int wolny=0;
		for(int i=0;i<MAX_CLIENTS;i++) if(iZalogowany[i]==0) wolny=i;

		if(-1==thread_ID && wolny ){
		int k;
		char napis[512];
		FILE* login= fopen("login.txt", "rt");
		 if (login==NULL) {fputs ("File error",stderr); exit (1);}
		 while( fscanf(login, "%d", &k) && fscanf(login, "%s", &napis) )  if(k ==sbMssg.ID) break;
		 if( iPorownajHaslo(&sbMssg, napis))  
		 {
			 //zalogowanie poprawne
			 gracze[wolny].ID=sbMssg.ID;
			 gracze[wolny].IT=-1;
			 thread_ID=wolny;
			iZalogowany[wolny]=1; // czyli odswierzamy informacje o zalogowanych osobach
		 }
		 else
		 {//zalogowanie niepoprawne
			 sbMssg.ID=3;
			  sConversion(&sbMssg, cMessage);
			  send(gracze[thread_ID].sock  , cMessage, 275, 0);
		 }
		}
	   else
	   {
	   if(wolny>=MAX_CLIENTS-1) 
	   {
		  sbMssg.ID=2; // brak wolnych slotow na serwerze
		 sConversion(&sbMssg, cMessage);	
		  send(gracze[thread_ID].sock  , cMessage, 275, 0);

		}
	  // tutaj sie wylogowujemy, wiec odsylamy po prostu co dostalismy
	   sConversion(&sbMssg, cMessage); //  konwersja sbMssg <-> cMessage
	   send(gracze[thread_ID].sock  , cMessage, 275, 0);
	   }

		 break;
		   }
	case 3:
		{
			int k=0;
		int t[4];
			for(int i=0;i<MAX_TABLES;i++) if(iUzytyStol[i]!= 0) k++;	
		if(k)// jesli w ogole jest jakikolwiek stol ktory nie jest pusty
		{
		for(int i=0;i<MAX_TABLES;i++) 
					if(iUzytyStol[i]!= 0)
						{
							t[i]=1;
						}
		
		sbMssg.ID=k;
		sbMssg.ID_USR = t[0];
		sbMssg.iKey[0] = t[1];
		sbMssg.iKey[1] = t[2];
		sbMssg.iKey[2] = t[3];

		for(int i=0;i<MAX_TABLES;i++)
			for(int j=0;j<4;j++)
			
		sbMssg.iKey[(i+1)*(j+1) + 3] = STOLIK[i].ID[j]; // tutaj uzupelniam sbMssg (zakladajac ze robie to dobrze xD)s
		}
			break;
		}
	case 4:
		if(sbMssg.ID == gracz->ID) {

		/*	4 - 	       : ID_USR - (0) jesli przylaczyles sie do stolu i wszystko jest ok
				  (wartosc niezerowa) - jesli do stolu dolacza gracz, to jest to jednoczesnie jego identyfikator
				  zakladam ze w ID_USR klient wysyla nr stolu do ktorego sie chce przylaczyc
				  
				  */

		{
			if(sbMssg.ID_USR >=0 && sbMssg.ID_USR < MAX_TABLES)
			{//rozwazamy tylko mozliwosc w ktorej stolik jest wybrany poprawnie
			int chosen;
				for(int i=0;i<4;i++)  if(!STOLIK[sbMssg.ID_USR].ID[i]) chosen=i;
				if( chosen>3) exit(1); //blad
				STOLIK[sbMssg.ID_USR].ID[chosen] = gracz->ID; //rezerwujemy miejsce przy stole dla gracza i rozsylamy pozostalym ziomkom jego id
				iTable=sbMssg.ID_USR;
				gracz->IT=sbMssg.ID_USR;
				//uzupelniam sbMssg danymi dla pozostalych graczy i rozsylam:
				sbMssgCLEAR(&sbMssg);
				
				sbMssg.ID_USR=gracz->ID;
				for(int i=0;i<MAX_CLIENTS;i++) if(gracze[i].ID != gracz->ID) if(gracze[i].IT == iTable)
				{
				sbMssg.ID = gracze[i].ID;
				sConversion(&sbMssg, cMessage);
				send(gracze[i].sock  , cMessage, 275, 0);
				}

			}
		
			
		}}
		else
		{//wiadomosc od serwera ze inny gracz dolacza do stolika
		
		
		}

	}//koniec switcha
	
	
	
	
	
	
	
	}//koniec nieskonczonej petli while dla switcha


		return 0;
}









void ShutdownServer(struct klient clients[])
{
    // zamykamy niezamknięte socket'y klientów
    for (int i = 0; i < MAX_CLIENTS; i++) 
        if (clients[i].sock != INVALID_SOCKET)
        {
            closesocket (clients[i].sock);
            clients[i].sock = INVALID_SOCKET;
        }
 int i=0;
 while(i<MAX_CLIENTS){
   if(clients[i].sock != INVALID_SOCKET)  closesocket (clients[i].sock);
    
}
}


int iIfFreeSlot(int active_players[])
{
int i=0;
for(int x=0;x<MAX_CLIENTS;x++)
	if(active_players[x]) i++;
return MAX_CLIENTS - i;
}
int iFindFreeSlot(int active_players[])
{
int i=0;
while(active_players[i])i++;
return i+1;
}
///////////////////////////////////////////////// FUNKCJA WATKA SILNIKA GRY (oddzielna dla kazdego klienta) //////////////////////////////////////



int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;
    SOCKET ListenSocket = INVALID_SOCKET;

	int SERVER_ON=1;


	int active_players[MAX_CLIENTS];
	for(int i =0; i<MAX_CLIENTS;i++)  iZalogowany[i]=0;
	
	    for (int i = 0; i < MAX_CLIENTS; i ++)
    {
		active_players[i]=0;
        gracze[i].thread = NULL;
        gracze[i].sock   = INVALID_SOCKET;
    }


	for(int i=0;i<MAX_CLIENTS;i++) gracze[i].sock =INVALID_SOCKET;

	struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

  ///////////////////// // TUTAJ STARTUJEMY Z WINSOCKEM/////////////////////////////////
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Wystąpił błąd modułu WSAStartup : %d\n", iResult);
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
        printf("Bład getaddrinfo(): %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Tworzymy gniazdo nasłuchujące:
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Błąd gniazda: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Następnie je wiążemy 
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Błąd wiązania gniazda: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
	
	// teraz oczekujemy na połączenie z klientami// od tad nalezy rozwazac model aplikacji: stoliki
	while(SERVER_ON)
	{
		if (iIfFreeSlot(active_players)){
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("Błąd nasłuchującego gniazda: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	int free_slot = iFindFreeSlot(active_players);
	active_players[free_slot]=1;
    // Teraz akceptujemy transmisję
	gracze[free_slot].sock = accept(ListenSocket, NULL, NULL);
    if (gracze[active_players[free_slot]].sock == INVALID_SOCKET) {
        active_players[free_slot]=0;
		printf("Nie powiodła się akceptacja transmisji: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }// istartujemy nowy watek:
	gracze[free_slot].thread = CreateThread (NULL, 0, iSilnikGry, (LPVOID)&gracze[free_slot], 0, NULL);
	if(!gracze[free_slot].thread)
			{
	            gracze[free_slot].sock = INVALID_SOCKET;
                cout << "Utworzenie watku dla klienta nie powiodlo sie." << endl;
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
        for (int i = 0; i < MAX_CLIENTS; i ++)
            TerminateThread (threads[i], 2);
 
    WSACleanup();
    system("pause");



    return 0;
}


void sbMssgCLEAR(Buffer* x)
{
	for(int i=0;i<256;i++) x->cChat[i]=0;
	x->ID=0;
	x->ID_USR= 0;
	for(int i=0;i<16;i++)	x->iKey[i] =0;
}


void sConversion(Buffer *sbMssg, char cMessage[])
{
cMessage[0] = (char)sbMssg->ID;
cMessage[1] = (char)sbMssg->ID_USR;
int k;
for(k=2;k<18;k++) cMessage[k] = (char)sbMssg->iKey[k-2];

for(k=18;k<275;k++) cMessage[k] = sbMssg->cChat[k-18];

}


void rConversion(Buffer *sbMssg, char *cMessage)
{
sbMssg->ID =(int)cMessage[0];
sbMssg->ID_USR =(int) cMessage[1];
int k;
for(k=2;k<18;k++) sbMssg->iKey[k-2] = (int)cMessage[k];
for(k=18;k<275;k++) sbMssg->cChat[k-18]=  cMessage[k];
}



short iGeneruIdGracza()
{
	int ID;
	char buf[513];
	FILE* X= fopen("login.txt", "rt");
	while(fscanf(X, "%d", &ID)) {
	fscanf(X, "%s", buf);
	
	}

return 0;
}


int wybierzHaslo(Buffer* sbMssg)
{

return 0;
}

void cMessageForward(Buffer *sbMssg)
{

	return;
}

char* sWybierzHaslo(Buffer* sbMssg)
{
char napis[]=" ";

return napis;
}


int iPorownajHaslo(Buffer *sbMssg, char napis[])
{

	return 0;
}

int iTranslate(Buffer* sbMssg)
{

return 0;
}