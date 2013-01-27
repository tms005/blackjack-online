

#define DEFAULT_BUFLEN					512
#define DEFAULT_PORT						"2000"
#define MAX_CLIENTS						12
#define MAX_TABLES							4
#define MAX_PLAYERS_PER_TABLE		3
#define MAX_NAME_LEN						15
#define MAX_PASS_LEN						40
#define BEGGINERS_CASH					300
#define MIN_BID								10

#define LOGIN_FILE_ERROR				2
#define	SERVER_FULL						2
#define WRONG_USERNAME				1

#define USERNAME_EXIST_OR_ERR	0
#define USERNAME_FINE					1

using namespace std;

struct klient
{
    HANDLE thread;												//dla kazdego klienta tutaj mamy uchwyt watku obslugujacego go
    SOCKET sock;												//dla kazdego klienta tutaj mamy uchwyt gniazda obslugujacego komunikacje z nim
	int ID;															//tutaj przechowujemy ID kazdego zalogowanego gracza odpowiadajacego watkowi
	int IT;																//tutaj przechowujemy IT kazdego graj¹cego gracza odpowiadajacego watkowi
	char cName[MAX_NAME_LEN];
	int iScore;														//wynik danego gracza
	int iCash;														//gotowka danego gracza
	int iMySpot;													//liczba identyfikujaca pola nalezace do gracza w strukturze table np gracz siedzacy przy zerowym miejscu
};																		//przy pierwszym stoliku bedzie mogl sie odwolac do tego miejsca: STOLIK[gracz->IT].ID[gracz->iMySpot]
klient gracze[MAX_CLIENTS];								//w podobny sposob moze sie odwolac do wszystkich elementow ktorych licznosc wynosi MAX_PLAYERS_PER_TABLE

struct table{
int ID[MAX_PLAYERS_PER_TABLE];								// w kazdym stoliku max 4 graczy // std wartosc: 0
bool baCardsDealt[52];											// tutaj talia rozdanych kart
int iHouseCardCount ;												// tutaj punkty zdobyte przez krupiera
int iaHouseHand[12];												// tutaj karty krupiera // zalozenie: -1 oznacza ze gracz dolaczyl do partii przed rozpoczeciem rozgrywki
int iPlayerCardCount[MAX_PLAYERS_PER_TABLE];		// tutaj liczba kart kazdego z graczy
int iPlayerCash[MAX_PLAYERS_PER_TABLE];				// tutaj gotowka kazdego z graczy
int iPlayerBid[MAX_PLAYERS_PER_TABLE];					// tutaj obstawiana kasa przez g
int iaPlayerHand[MAX_PLAYERS_PER_TABLE][12];		// tutaj karty gracza
int iActivePlayer;														// gracz ktory wlasnie w kolejce podejmuje decyzje
int iRoundMask[MAX_PLAYERS_PER_TABLE];				// jesli gracz wlasnie dolaczyl nie moze grac puki nie rozpocznie sie nowe rozdanie.. ta maska chroni przed taka sytuacja
int iSTARTED;
int iActive[MAX_PLAYERS_PER_TABLE];						//mowi o tym czy w danej rundzie gracz zrezygnowal (spasowal)
} ;
table STOLIK[MAX_TABLES];									// istnieja 3 stoliki

struct Buffer {															//struktura wiadomosci s³ó¿¹cej do komunikacji serwer<->klient
int ID;																	//identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR;																// nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika
int iKey[16];															// w tym polu mamy kolejne argumenty dla funkcji 
char cChat[256];
};

int iUzytyStol[MAX_TABLES]	;									// tablica wykorzystanych stolow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! SPRAWDZIC UZYTECZNOSC ZMIENNEJ LUB WYELIMINOWAC!!!!!!!!!!!!
int active_players[MAX_CLIENTS];								// ta tablica reguluje naplywajace polaczenia poprzez limitowanie ich do jej dlugosci		
int iAktywny;
				
struct dane{															//	tutaj przechowujemy pojedynczy kwant danych o klientach zapisany w pliku klientow
int ID;						
char cName[MAX_NAME_LEN];
char cPass[MAX_PASS_LEN];
int iScore;																//wynik danego gracza
int iCash;																//wysokosc konta danego gracza
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////FUNKCJE://///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void sbMssgCLEAR(Buffer *sbMssg); 

 void sConversion(	Buffer *sbMssg,
							char cMessage[]);

 void rConversion(	Buffer *sbMssg,
							char *cMessage);

int iGenerujIdGracza(	FILE* name,
								vector<dane> tab);

int iPorownajHaslo(char haslo1[],
							char haslo2[]);

int iTranslate(Buffer* sbMssg);

void vWybierzImie(	Buffer *sbMssg, 
							char* out);

void vWybierzHaslo(Buffer* sbMssg,
								char* out);

           //////////////////// Funkcje Silnika Gry:
void vNextPlayer(int IT);

void vShuffle(	bool baCardsDealt[]);

void vEmptyHand(	int  (*hand)[3][12],
							int index);

void vUaktualnijDane(klient* gracz);

void vWywal(table *STOL,
					klient* player);

void	vRozeslijWynik(	klient* player,
								int card);

int GetNextCard(bool baCardsDealt[]);

int ifend(	table STOL,
				klient* gracz);

int ScoreHand(	int iaHand[],
						const int kiCardCount);

int vCzyKoniec(	table, 
						klient*);	

void complete_house(	table* STOL,
								int g);

void jump_next(	table *STOL,
						klient *gracz,
						char* cMessage);

void  vFindWinner(	table* STOL,
							int IT,
							char* cMessage);

void bbbIntToChar(	int* source,
								char* destination);

void bbbCharToInt(	char * source,
								int* destination);



int  _2TbbbCharToInt(	char* source,
								int* destination,
								int s_len,
								int d_len);

int  _2TbbbIntToChar(	int* source,
								char* destination,
								int s_len,
								int d_len);

void	vAddTerminalChar(	char sName[],
									int len);

int iFindFreeSlot(int active_players[]);

int iIfFreeSlot(int active_players[]);


void vDebugPrintMssg(Buffer *sbMssg);

void CLEAN_TABLES(table* STOL);

void CLEAN_CLIENTS(klient gracz[]);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////SILNIK GRY///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD WINAPI iSilnikGry(LPVOID ctx)
{

	int ret;					// tutaj przechowujemy uniwersalna zmienna zwracajaca wyniki

	
	klient * gracz=(klient*)ctx;
	cout<<"\nUtworzono watek dla nowego polaczenia (Gniazdo: "<< gracz->sock<<")\n"<<endl;
	gracz->iCash=0;
	gracz->cName[0]='\0';
	gracz->ID=0;
	gracz->iScore=0;
	gracz->iMySpot=-1;
	gracz->IT=-1;

	char cMessage[512];
	Buffer sbMssg; 
	memset (&cMessage,0,512);
	
	while(iAktywny)
	{

	  ret = recv(gracz->sock, cMessage, 512, 0);
	   if(ret == 0)   //polaczenie zamkniete po stronie klienta
		{
			iAktywny=0;
			cout<<"Klient: ";
			if(gracz->ID) cout<<gracz->ID ;
			else cout<< "[brak id klienta]";
			cout<< " zamknal polaczenie z serwerem"<<endl;
		if(gracz->ID != 0) //czyszczenie ew. danych
			{
			if(gracz->IT != -1)
					{
						if(gracz->iMySpot)
							{
								STOLIK[gracz->IT].ID[gracz->iMySpot]=0;
								STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot]=0;
								STOLIK[gracz->IT].iPlayerCardCount[gracz->iMySpot]=0;
								// tutaj jeszcze zaktualizowac pole next oraz active o ile gracz byl jednym z tych graczy
						}			
				}
			}
			return 0;
	   }
	rConversion(&sbMssg, cMessage);										 // za kazdym odebraniem wiadomosci z klienta tlumacze ja z powrotem do postaci ramki struktury

																							// tutaj decydujemy co bedzie sie dzialo 

	vDebugPrintMssg(&sbMssg);
	cout<<"sbMssg:ID"<<sbMssg.ID;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch(sbMssg.ID)
		{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
case 0:
		{																				// wysylamy wiadomosc do swojego stolika lub do wszystkich
			if(gracz->ID>0){
			printf("\n\nCZAT:Gracz ID: %d", gracz->ID);
			string text;
			text.clear();
			text+="[";
			text+= gracz->cName;
			text+="] : ";
			text += sbMssg.cChat;
			strcpy(sbMssg.cChat, text.c_str());
			sbMssg.ID_USR =gracz->ID;
			sConversion(&sbMssg, cMessage);
			if(gracz->IT!=-1) 
			{
				for(int i=0;i<MAX_CLIENTS;i++) if( gracz->IT == gracze[i].IT) send( gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);
			}
			else for(int i=0;i<MAX_CLIENTS;i++) if (gracze[i].IT==-1) send( gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);
			break;
		
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1:
		{
	
			int FILE_DOESNT_EXIST=0;
			int already_exist=0;
			char imie[MAX_NAME_LEN], haslo[MAX_PASS_LEN];
			memset (&imie,0,MAX_NAME_LEN);
			memset (&haslo,0,MAX_PASS_LEN);

			vWybierzImie(&sbMssg, imie);			
				
			vWybierzHaslo(&sbMssg, haslo);

			dane user;
			vector<dane> tab;
			FILE* login= fopen("login.txt", "r+t");
			if (login==NULL) 																	// otwieramy plik z uzytkownikami i informacjami o nich
			{
				FILE_DOESNT_EXIST=1;
				fputs ("\nPlik login nie istnieje! Tworzê nowy plik...\n", stderr);
				FILE* login= fopen("login.txt", "r+t");									//jesli nie ma pliku to tworzymy go
				if (login==NULL) 																	
				{																						// jesli nie mozna utworzyc pliku to informujemy o bledzie klienta i przerywamy proces
				sbMssg.iKey[0]=LOGIN_FILE_ERROR;
				sConversion(&sbMssg, cMessage);
				send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);		 
				iAktywny =0;
				}
			}

			vWybierzImie(&sbMssg, imie);			
			vWybierzHaslo(&sbMssg, haslo);
			cout<<"FILE DOESNT EXIST="<<FILE_DOESNT_EXIST<<endl;
		
			
			if(FILE_DOESNT_EXIST)
			{		
			printf("LOGIN: %d", login);
			sbMssg.ID_USR = 1;																//z wiadomosci otrzymujemy haslo
			fseek(login,0,SEEK_SET);
			printf("\n\nPierwszy wpis: %d %s %s %d %d\n\n", sbMssg.ID_USR, imie, haslo, 0, BEGGINERS_CASH);
			fprintf(login, "%d %s %s %d %d\n",sbMssg.ID_USR,  imie, haslo, 0, BEGGINERS_CASH );			//zapisujemy informacje do pliku
			fclose(login);
			}
			else																																		//jesli jednak plik istnieje:
			{																																	
			while(feof(login) == 0)
				{
				fscanf(login, "%d", &user.ID);
				fscanf(login, "%s", user.cName);
				fscanf(login, "%s", user.cPass);
				fscanf(login, "%d", &user.iScore);
				fscanf(login, "%d", &user.iCash);
				printf("DANE: ID[%d]NAME:[%s]PASS:[%s]SCORE:[%d]CASH:[%d]\n",user.ID, user.cName, user.cPass, user.iScore, user.iCash);
				if( ! strcmp(user.cName , imie ) ) already_exist=WRONG_USERNAME;										// i porownujemy je z istniejacymi
				tab.push_back(user);																										
				}
			
		if(already_exist==0)
				{
				cout<<"Tworze konto dla nowego gracza..\n";
				sbMssg.ID_USR = iGenerujIdGracza(login, tab);
				vWybierzHaslo(&sbMssg, haslo);
				printf("\n\nWYGENEROWANO: %d %s %s %d %d\n", sbMssg.ID_USR, imie, haslo, 0, BEGGINERS_CASH);
				fseek(login,0,SEEK_END);
				fprintf(login, "%d %s %s %d %d\n",sbMssg.ID_USR,  imie, haslo, 0, BEGGINERS_CASH ); 				//  0 jako poczatkowa wartosc wyniku gracza
				fclose(login);	
				}
		else
				{
				cout<<"\n\nImie juz istnieje w pliku!\n\n";
				sbMssg.iKey[0]=USERNAME_EXIST_OR_ERR;
				}
		}
		sConversion(&sbMssg, cMessage);	
		send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
	
		fclose(login);
		break;
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 2:
		{
		
			if(0==gracz->ID){			// NIE JESTESMY ZALOGOWANI, ALE CHCEMY TO ZROBIC I JEST MIEJSCE POWNIEWAZ WATEK ISTNIEJE
			
			int id, cash, score;
			char name[MAX_NAME_LEN], pass[MAX_PASS_LEN];
			char imie[MAX_NAME_LEN], haslo[MAX_PASS_LEN];
			memset (name,0,MAX_NAME_LEN);
			memset (imie,0,MAX_NAME_LEN);
			memset (pass,0,MAX_PASS_LEN);
			memset (haslo,0,MAX_PASS_LEN);
			



			FILE* login= fopen("login.txt", "r+");
			if (login==NULL) 																	//sprawdzic czy gracz juz istnieje w bazie
			{
				fputs ("\nPlik login nie istnieje! Nikt nie ma prawa sie logowac!\n", stderr);
				FILE* login= fopen("login.txt", "a");
				if (login==NULL) 																	
				{
				sbMssg.iKey[0]=0;
				sConversion(&sbMssg, cMessage);
				send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);		 
				break;
				}
			}
	
			if(login!=NULL) 
				{
					while(feof(login) == 0){
			 	 
			 fscanf(login, "%d", &id);
			 fscanf(login, "%s", name);
			 fscanf(login, "%s", pass);
			 fscanf(login, "%d", &score);
			fscanf(login, "%d", &cash);
			
			//vAddTerminalChar(name, MAX_NAME_LEN);
			//vAddTerminalChar(pass, MAX_PASS_LEN);


			vWybierzImie(&sbMssg, imie);
			
			if(!strcmp(name, imie)) break;
		 }

		 vWybierzHaslo(&sbMssg, haslo);
		 cout<<"\n\nPOROWNANIE:"<<  iPorownajHaslo(haslo, pass) <<endl;

		 if( iPorownajHaslo(haslo, pass))  
		 {
			 //zalogowanie poprawne
			 gracz->ID=id;
			 gracz->IT=-1;
			 gracz->iScore = score;
			 gracz->iCash=cash;
			 strcpy(gracz->cName, name);

			 sbMssg.iKey[0]=1;
			 sbMssg.iKey[1] = id;
			 sbMssg.iKey[2] = cash;
			
			 sConversion(&sbMssg, cMessage);
			 send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
		 	}
		 else
		 {//zalogowanie niepoprawne
			  sbMssg.iKey[0]=2;
			  sConversion(&sbMssg, cMessage);
			  send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
		 }
			
			}
			else {
				printf("Nie mo¿na utworzyc pliku!");
		
			
			}
			}
		 else			  // tutaj sie wylogowujemy, wiec odsylamy po prostu co dostalismy
	   {
	   sConversion(&sbMssg, cMessage);																						//  konwersja sbMssg <-> cMessage
	   send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);															//a nastepnie sprawdzamy dane dot. tego czy: jestesmy przy stole i gramy
	   if(gracz->IT!=-1){

		   	if(	STOLIK[gracz->IT].iActivePlayer == gracz->ID ) vNextPlayer(gracz->IT);								// jesli gracz byl wybierajacym graczem, to nalezy zmienic go na nastepnego
			int k,i,j=0;
			for(i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOLIK[gracz->IT].ID[i] == gracz->ID) k =i;		// wybieram ktore pola w strukturze odpowiadaja bierzacemu graczowi

			STOLIK[gracz->IT].ID[k] = 0;																							// i resetujê informacje
			STOLIK[gracz->IT].iPlayerCash[k]=0;																			
			STOLIK[gracz->IT].iPlayerBid[k]=0;
			STOLIK[gracz->IT].iPlayerCardCount[k]=0;
			vEmptyHand(  &STOLIK[gracz->IT].iaPlayerHand, k);
			gracz->iCash=STOLIK[gracz->IT].iPlayerCash[k];																// przy odejsciu od stolika aktualizujemy informacje o stanie konta gracza
			
			for(i=0;i<MAX_PLAYERS_PER_TABLE;i++)  if(STOLIK[gracz->IT].iRoundMask[i]==gracz->ID)    j=i;			//dbamy by cykl rundowy byl poprawny
			for(;j<MAX_PLAYERS_PER_TABLE-1;j++) STOLIK[gracz->IT].iRoundMask[j] = STOLIK[gracz->IT].iRoundMask[j+1];
			if(j==MAX_PLAYERS_PER_TABLE-1) STOLIK[gracz->IT].iRoundMask[j] = 0;							// ostatni zapelniam zerem

			sbMssg.ID_USR = gracz->ID;
			strcpy(sbMssg.cChat, gracz->cName);
			sConversion(&sbMssg, cMessage);
			for(int i=0;i<MAX_CLIENTS;i++) if( gracze[i].ID != gracz->ID && gracze[i].IT == gracz->IT)
			{
				send(gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);													// rozsylam info o opuszczeniu rozgrywki
			}
			vUaktualnijDane(gracz);																									// zapisanie informacji o zarobionej gotowce/pktach
			strcpy(gracz->cName, "\0");																								//oraz czyszczenie danych logowania serwera
			gracz->ID=0;
			gracz->iCash=0;
			gracz->iScore=0;
			gracz->IT=-1;
	   }

	   if(gracz->ID)   printf("\nGRACZ O NICKU: %s [ID:%d] ZALOGOWAL SIE!\nPRZYDZIELONY MU WATEK: %d i Socket: %d\n\n", gracz->cName, gracz->ID, gracz->thread,gracz->sock);
		break;
		}
		if(gracz->ID)   printf("\nGRACZ O NICKU: %s [ID:%d] ZALOGOWAL SIE!\nPRZYDZIELONY MU WATEK: %d i Socket: %d\n\n", gracz->cName, gracz->ID, gracz->thread,gracz->sock);

		break;
	   }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 3:
		{
			if(sbMssg.ID_USR>0) switch(sbMssg.ID_USR)
			{
			case 0: 
				{
				int k=0;
				for(int i=0;i<MAX_TABLES;i++) if(iUzytyStol[i]!= 0)	k++;								
				if(k)																																		// jesli w ogole jest jakikolwiek stol ktory nie jest pusty
				{	
					for(int i=0;i<MAX_TABLES;i++)
						for(int j=0;j<MAX_PLAYERS_PER_TABLE; j++) sbMssg.iKey[(i+1)*(j+1)] = STOLIK[i].ID[j];		// tutaj uzupelniam sbMssg (zakladajac ze robie to dobrze xD)s
				break;
				}
				}
			case 1:
				{
					FILE* login= fopen("login.txt", "rt+");
				 if (login==NULL) 
				 {
					fputs ("File error",stderr);
					send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);													//jesli nie mozna odczytac pliku odsyla sie pusta liste
					break;
				 }
				 vector<dane> ranking;
				 ranking.clear();
				 dane nowy;
				 char trash[256];
				 while(feof(login) == 0)
					{
					fscanf(login, "%d", &nowy.ID);
					fscanf(login, "%s", &nowy.cName);
					fscanf(login, "%s", trash);
					fscanf(login, "%d", nowy.iScore);
					fscanf(login, "%d", &nowy.iCash);
					ranking.push_back(nowy);	
				 }
				 //mamy zapelniona liste, wybieramy teraz najlepsze 10 wynikow:
				 vector<dane>::iterator it = ranking.begin();
				 while(ranking.size() >10)
				 {
					 it = ranking.begin();
					 for(vector<dane>::iterator x=ranking.begin()+1; x<ranking.end(); x++)
					 {
						 if(x->iScore < it->iScore) it = x;
					  }
					 ranking.erase(it);
				 }
				 //teraz ranking zawiera jedynie najlepsze wyniki
				 it=ranking.begin();
				 int k=1;
				string best;
				best.clear();
				 while(ranking.size()){
					 it = ranking.begin();
					for(vector<dane>::iterator x=ranking.begin()+1; x<ranking.end(); x++)
						 {
						 if(x->iScore > it->iScore) it = x;
						 }
					sbMssg.iKey[k] = it->iScore;
					best+=it->cName;
					best+=' ';
					k++;
				 }
				 strcpy(sbMssg.cChat, best.c_str() );
				 break;
				}

			case 2:
				{
					string zalogowani;
					zalogowani.clear();
				//	2 - przeslij informacje o zalogowanych graczach
					for(int i=0;i<MAX_CLIENTS;i++)
					{
						if(gracze[i].ID >0)
						{
							//zalogowany
							zalogowani+=gracze[i].cName;
							zalogowani+=' ';
						}

					}
					strcpy(sbMssg.cChat, zalogowani.c_str());
				break;
				}
		}
			  sConversion(&sbMssg, cMessage);
			  send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
		
		break;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 4:
		{ // przylaczenie sie do stolu..
			if(sbMssg.ID_USR >=0 && sbMssg.ID_USR < MAX_TABLES)
			{//rozwazamy tylko mozliwosc w ktorej stolik jest wybrany poprawnie
				int k=0,i=0,j=0;
				int tab=sbMssg.ID_USR;
				for(k=0;k<MAX_PLAYERS_PER_TABLE;k++) if(STOLIK[tab].ID[k]) i++;
					if(i<MAX_PLAYERS_PER_TABLE)
					{		//nie ma sensu rozwazac przylaczenia jesli graczy jest po prostu za duzo..
						if(i==0)
						{			// inicjacja jesli stolik byl pusty
							for(int ii=0;ii<MAX_PLAYERS_PER_TABLE;ii++) 
								{
									STOLIK[tab].iPlayerCardCount[ii]=0;
									STOLIK[tab].iPlayerCash[ii]=0;			
									STOLIK[tab].iPlayerBid[ii]=0;				
									for(j=0;j=12;j++) STOLIK[tab].iaPlayerHand[ii][j]=0;

								}
							STOLIK[tab].iActivePlayer=0;
							vShuffle( STOLIK[tab].baCardsDealt);
							STOLIK[tab].iHouseCardCount=0;
							for(j=0;j=12;j++) STOLIK[tab].iaHouseHand[j]=0;
							STOLIK[tab].iSTARTED=0;										//tylko pierwszy gracz moze rozpoczac rozgrywke, flaga iSTARTED mowi o tym, czy sie rozpoczela
						}
						if(!STOLIK[tab].iSTARTED)	STOLIK[tab].iRoundMask[i]=gracz->ID;	// mozesz dolaczyc tylko przed rozpoczeciem gry, lub po.
						int chosen;
						for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++)  if(!STOLIK[tab].ID[i]) chosen=i; // znajdujemy wolne miejsce przy stoliku
						STOLIK[tab].ID[chosen] = gracz->ID;			//rezerwujemy miejsce przy stole dla gracza i rozsylamy pozostalym ziomkom jego id
						gracz->IT=tab;											//uzupelniam sbMssg danymi dla pozostalych graczy i informujemy pozostalych graczy:
						sbMssgCLEAR(&sbMssg);
						sbMssg.ID_USR=gracz->ID;
						strcpy(sbMssg.cChat, gracz->cName);
						sConversion(&sbMssg, cMessage);
						for(int i=0;i<MAX_CLIENTS;i++) if( gracze[i].ID != gracz->ID && gracze[i].IT == gracz->IT)
								{
								send(gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);
								}
				}
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
case 5:
		{	
			if(gracz->ID>0){
				if(gracz->IT !=-1){

				if(	STOLIK[gracz->IT].iActivePlayer == gracz->ID ) vNextPlayer(gracz->IT);							// jesli gracz byl wybierajacym graczem, to nalezy zmienic go na nastepnego
				int k,i,j=0;
				for(i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOLIK[gracz->IT].ID[i] == gracz->ID) k =i;		// wybieram ktore pola w strukturze odpowiadaja bierzacemu graczowi

				STOLIK[gracz->IT].ID[k] = 0;																							// i resetujê informacje
				STOLIK[gracz->IT].iPlayerCash[k]=0;																			
				STOLIK[gracz->IT].iPlayerBid[k]=0;
				STOLIK[gracz->IT].iPlayerCardCount[k]=0;
				vEmptyHand(  &STOLIK[gracz->IT].iaPlayerHand, k);
				gracz->iCash=STOLIK[gracz->IT].iPlayerCash[k];																// przy odejsciu od stolika aktualizujemy informacje o stanie konta gracza
			
				for(i=0;i<MAX_PLAYERS_PER_TABLE;i++)  if(STOLIK[gracz->IT].iRoundMask[i]==gracz->ID)    j=i;			//dbamy by cykl rundowy byl poprawny
				for(;j<MAX_PLAYERS_PER_TABLE-1;j++) STOLIK[gracz->IT].iRoundMask[j] = STOLIK[gracz->IT].iRoundMask[j+1];
				if(j==MAX_PLAYERS_PER_TABLE-1) STOLIK[gracz->IT].iRoundMask[j] = 0;							// ostatni zapelniam zerem

				sbMssg.ID_USR = gracz->ID;
				strcpy(sbMssg.cChat, gracz->cName);
				sConversion(&sbMssg, cMessage);
				for(int i=0;i<MAX_CLIENTS;i++) if( gracze[i].ID != gracz->ID && gracze[i].IT == gracz->IT)
					{
					send(gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);													// rozsylam info o opuszczeniu stolu
					}
				}
			}
		break;
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
case 6:
		{
			if(gracz->ID) // jesli gracz byl zalogowany
				{
				if(gracz->IT!=-1)
					{ // jesli gracz byl przy stoliku
					if(	STOLIK[gracz->IT].iActivePlayer == gracz->ID ) vNextPlayer(gracz->IT);							// jesli gracz byl wybierajacym graczem, to nalezy zmienic go na nastepnego
					int k,i,j=0;
					for(i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOLIK[gracz->IT].ID[i] == gracz->ID) k =i;		// wybieram ktore pola w strukturze odpowiadaja bierzacemu graczowi

					STOLIK[gracz->IT].ID[k] = 0;																							// i resetujê informacje
					STOLIK[gracz->IT].iPlayerCash[k]=0;																			
					STOLIK[gracz->IT].iPlayerBid[k]=0;
					STOLIK[gracz->IT].iPlayerCardCount[k]=0;
					vEmptyHand(  &STOLIK[gracz->IT].iaPlayerHand, k);
					gracz->iCash=STOLIK[gracz->IT].iPlayerCash[k];																// przy odejsciu od stolika aktualizujemy informacje o stanie konta gracza
			
					for(i=0;i<MAX_PLAYERS_PER_TABLE;i++)  if(STOLIK[gracz->IT].iRoundMask[i]==gracz->ID)    j=i;			//dbamy by cykl rundowy byl poprawny
					for(;j<MAX_PLAYERS_PER_TABLE-1;j++) STOLIK[gracz->IT].iRoundMask[j] = STOLIK[gracz->IT].iRoundMask[j+1];
					if(j==MAX_PLAYERS_PER_TABLE-1) STOLIK[gracz->IT].iRoundMask[j] = 0;							// ostatni zapelniam zerem

					sbMssg.ID_USR = gracz->ID;
					strcpy(sbMssg.cChat, gracz->cName);
					sConversion(&sbMssg, cMessage);
					for(int i=0;i<MAX_CLIENTS;i++) if( gracze[i].ID != gracz->ID && gracze[i].IT == gracz->IT)
						{
						send(gracze[i].sock  , cMessage, DEFAULT_BUFLEN, 0);													// rozsylam info o opuszczeniu rozgrywki
						}
					// zapisanie informacji o zarobionej gotowce/pktach
					vUaktualnijDane(gracz);
					//oraz czyszczenie danych logowania serwera
					}
				strcpy(gracz->cName, " ");
				gracz->ID=0;
				gracz->iCash=0;
				gracz->iScore=0;
				gracz->IT=-1;
			
			}
			int l;
			for(l=0; l<MAX_CLIENTS;l++) if( gracze[l].ID == gracz->ID) break; //szukam pozycji na tablicy polaczen odpowiadajacej watkowi
			active_players[l]=0;
			closesocket(gracz->sock);
			iAktywny=0;
			break;
		
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
case 7:
		{													//glowna funkcja gry: zalozenia: jestesmy przy stoliku i wiadomosc przychodzi od gracza bedacego graczem aktywnym
			int karta=-1;
			for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOLIK[gracz->IT].ID[i] = gracz->ID) gracz->iMySpot = i; // w tabeli STOLIK ten numer indeksuje zmienne tego gracza
			if(gracz->iCash >= STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot] ) // czy gracz ma dostatecznie duzo kasy by zagrac?
			{	// tak
				//czy gracz ma prawo zagrac(czy sie zaczela kolejka?
				if(STOLIK[gracz->IT].iRoundMask[gracz->iMySpot] == gracz->ID && STOLIK[gracz->IT].iActivePlayer == gracz->ID) // gracz ma prawo do gry i jest jego kolejka
				{
				//ok mozna grac!!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////   MECHANIZM KRUPIERA ///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				switch(sbMssg.iKey[0])
					{
					case 2:
						{
						if(STOLIK[gracz->IT].iSTARTED == 0)
						{
						// jesli to jest pierwszy ruch w pierwszej rundzie jesli
						STOLIK[gracz->IT].iSTARTED=2;
						vShuffle(STOLIK[gracz->IT].baCardsDealt);
						

						//losuje pierwsze dwie dla kazdego :
						STOLIK[gracz->IT].iHouseCardCount=2;
						STOLIK[gracz->IT].iaHouseHand[0]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
						STOLIK[gracz->IT].iaHouseHand[1]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
						for(int t=0;t<12;t++) STOLIK[gracz->IT].iaHouseHand[t] =0;
						

						STOLIK[gracz->IT].iActivePlayer = gracz->ID;							//sprawdzic czy prawidlowo zadeklarowane
						if(STOLIK[gracz->IT].iRoundMask[gracz->iMySpot]) STOLIK[gracz->IT].iActive[gracz->iMySpot] = 1; // jesli moze grac, to zostaje aktywowany do gry
						for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++)
							{
						
								STOLIK[gracz->IT].iPlayerCardCount[i]=2;
								STOLIK[gracz->IT].iaPlayerHand[i][0]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
								STOLIK[gracz->IT].iaPlayerHand[i][1]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
								for(int t=2;t<12;t++) STOLIK[gracz->IT].iaPlayerHand[i][t] =0;	
						}
						STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot]+=MIN_BID; // GRAMY ZA PODWOJONA STAWKE< ALE TYLKO W 1 LOSOWANIU!
						STOLIK[gracz->IT].iPlayerCash[gracz->iMySpot] = gracz->iCash - STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot];
						
						}
						else
						{
							if(STOLIK[gracz->IT].iSTARTED==2)
							{
							//TYLKO TERAZ MOZNA GRAC PODWOJNIE!!
							STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot]+=MIN_BID*2;	
							STOLIK[gracz->IT].iPlayerCash[gracz->iMySpot] = gracz->iCash - STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot];
							STOLIK[gracz->IT].iaPlayerHand[gracz->IT][++STOLIK[gracz->IT].iPlayerCardCount[gracz->iMySpot]]	= GetNextCard(STOLIK[gracz->IT].baCardsDealt);			
							STOLIK[gracz->IT].iSTARTED=1;
							sbMssg.iKey[1] = STOLIK[gracz->IT].iaPlayerHand[gracz->IT][STOLIK[gracz->IT].iPlayerCardCount[gracz->iMySpot]];	
							sConversion(&sbMssg, cMessage);	
							send( gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
							}//else: impossible!
						}
							
						break;
						 }
					case 1:
						{
						if(STOLIK[gracz->IT].iSTARTED == 0)
						{
						// jesli to jest pierwszy ruch w pierwszej rundzie jesli
						STOLIK[gracz->IT].iSTARTED=2;
						vShuffle(STOLIK[gracz->IT].baCardsDealt);
						

						//losuje pierwsze dwie dla kazdego :
						STOLIK[gracz->IT].iHouseCardCount=2;
						STOLIK[gracz->IT].iaHouseHand[0]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
						STOLIK[gracz->IT].iaHouseHand[1]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
						for(int t=0;t<12;t++) STOLIK[gracz->IT].iaHouseHand[t] =0;
						

						STOLIK[gracz->IT].iActivePlayer = gracz->ID;							//sprawdzic czy prawidlowo zadeklarowane
						if(STOLIK[gracz->IT].iRoundMask[gracz->iMySpot]) STOLIK[gracz->IT].iActive[gracz->iMySpot] = 1; // jesli moze grac, to zostaje aktywowany do gry
						for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++)
							{
						
								STOLIK[gracz->IT].iPlayerBid[i]+=10;
								STOLIK[gracz->IT].iPlayerCash[i] = gracz->iCash - STOLIK[gracz->IT].iPlayerBid[i];
								STOLIK[gracz->IT].iPlayerCardCount[i]=2;
								STOLIK[gracz->IT].iaPlayerHand[i][0]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
								STOLIK[gracz->IT].iaPlayerHand[i][1]= GetNextCard(STOLIK[gracz->IT].baCardsDealt);
								for(int t=2;t<12;t++) STOLIK[gracz->IT].iaPlayerHand[i][t] =0;	
						}
						
						}
						else // kolejny ruch w rundzie
						{
							if(STOLIK[gracz->IT].iSTARTED==2) STOLIK[gracz->IT].iSTARTED=1;
							STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot]+=MIN_BID;	
							STOLIK[gracz->IT].iPlayerCash[gracz->iMySpot] = gracz->iCash - STOLIK[gracz->IT].iPlayerBid[gracz->iMySpot];
							STOLIK[gracz->IT].iaPlayerHand[gracz->IT][++STOLIK[gracz->IT].iPlayerCardCount[gracz->iMySpot]]	= GetNextCard(STOLIK[gracz->IT].baCardsDealt);			
							STOLIK[gracz->IT].iSTARTED=1;
						}
						STOLIK[gracz->IT].iActive[gracz->iMySpot]=1;
						break;

							sbMssg.iKey[1] = STOLIK[gracz->IT].iaPlayerHand[gracz->IT][STOLIK[gracz->IT].iPlayerCardCount[gracz->iMySpot]];	
							sConversion(&sbMssg, cMessage);	
							send( gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
						}
					case 0:
						{
							if(STOLIK[gracz->IT].iSTARTED==2) STOLIK[gracz->IT].iSTARTED=1;
							STOLIK[gracz->IT].iActive[gracz->iMySpot]=0;
							break;
						}
					default:
						{
							STOLIK[gracz->IT].iActive[gracz->iMySpot] = ifend(STOLIK[gracz->IT], gracz);					//spr czy w losowaniu przekroczono 21
							int y= vCzyKoniec(STOLIK[gracz->IT], gracz);																	//spr czy caly stolik zakonczyl rozgrywke?
							if(y){
							complete_house(STOLIK, gracz->IT);																				//jesli kompletny, to wybierz wartosci dla krupiera
							vFindWinner(STOLIK,gracz->IT, cMessage);																	//nakaz klientom importowac dane o house
							}																																	//oraz oblicz kto zwyciezyl, a kto przegral i przeslij wyniki do struktur							
							else 
							jump_next(STOLIK, gracz, cMessage);																													//jesli nadal nie ma zwyciezcy przejdz do kolejnego gracza
						}
					}

				}
			}
			else// czy gracz ma dostatecznie duzo kasy by zagrac?:  nie
			{
				vWywal( STOLIK, gracz);
			}
			vRozeslijWynik(gracz, karta); // poinformuj innych graczy o zdarzeniu i zapros kolejnego
	
		break;
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	case 8:
		{
		
			if(sbMssg.iKey[0] == 0) // pobieramy dane o gotowce danego gracza
			{
			int found=-1, id=sbMssg.ID_USR;
			for(int i=0;i<MAX_CLIENTS;i++) if(gracze[i].ID=id) found = i;
			if(found>=0)
				{ // jesli gracz gra, to jego gotowka mogla sie juz zmienic
				if(gracze[found].IT!=-1) sbMssg.iKey[1]= STOLIK[gracze[found].IT].iPlayerCash[gracze[found].iMySpot];
				else sbMssg.iKey[1]= gracze[found].iCash;
				}
			//else: zalozenie dane sa poprawnie wypelnione przez klienta
			}
			else// pobieramy dane o kartach
			{
				if(sbMssg.iKey[0] == -1) // info o kartach krupiera
					{
					sbMssg.iKey[1] = STOLIK[gracz->IT].iHouseCardCount;
					for(int i=0;i<STOLIK[gracz->IT].iHouseCardCount;i++) sbMssg.iKey[i+2] = STOLIK[gracz->IT].iaHouseHand[i];
					}
				else// info o kartach gracza z id w ID_USR
					{
						int playah=sbMssg.ID_USR, found=-1, i;
						for(i=0;i<MAX_CLIENTS;i++) if(gracze[i].ID == playah) found =i;
						if(found>=0)
							{		//w danych znalezionego gracza wybieramy informacje o stoliku i jego kartach
								int t=gracze[found].IT;
								sbMssg.iKey[i+1] = STOLIK[t].iPlayerCardCount[gracze[found].iMySpot];
								for(int i=0;i<12;i++)	sbMssg.iKey[i+2] = STOLIK[t].iaPlayerHand[gracze[found].iMySpot][i];
							}
						//else: nie wymagane, dane powinny byc poprawne
					}
			}
			sConversion(&sbMssg, cMessage);	
			send( gracz->sock  , cMessage, DEFAULT_BUFLEN, 0);
		}
	}//koniec glownego switcha watku

	}//koniec nieskonczonej petli while dla switcha


		return 0;
}




////////////////////////////////FUNKCJA CZYSCI ZAWARTOSC STRUKTURY sbMssg//////////////////////////////////////////////////////////////////////////////////////
void sbMssgCLEAR(Buffer* x)
{
	for(int i=0;i<256;i++) x->cChat[i]=0;
	x->ID=0;
	x->ID_USR= 0;
	for(int i=0;i<16;i++)	x->iKey[i] =0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sConversion(Buffer *sbMssg, char cMessage[])
{// Funkcja konwersji nadawczej - > koduje wartoœci na podstawie struktury Buffer do tablicy znakow, która ma zostaæ przes³ana
	bbbIntToChar(&sbMssg->ID, &cMessage[0]);
	bbbIntToChar(&sbMssg->ID_USR, &cMessage[4]);
	int k;
	_2TbbbIntToChar(	sbMssg->iKey, &cMessage[8], 16,64); // funkcja zapewnia prawid³owe kopiowanie bit po bicie informacjiz sbMssg->iKey do tablicy cMessage
	for(k=0;k<256;k++) cMessage[k+72] = sbMssg->cChat[k]; // tutaj przesuniecie z powodu szerokosci poprzednich pol: (1+1+16)*4bajty = 72 bajty
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rConversion(Buffer *sbMssg, char *cMessage)
{// Funkcja konwersji odbiorczej - > dekoduje wartoœci do struktury Buffer na podstawie odebranej tablicy znakow odebranej z po³¹czenia
	bbbCharToInt(&cMessage[0], &sbMssg->ID);
	bbbCharToInt(&cMessage[4], &sbMssg->ID_USR);
	int k;
	_2TbbbCharToInt(&cMessage[8], sbMssg->iKey, 64, 16); // funkcja zapewnia prawid³owe kopiowanie bit po bicie informacji z sbMssg->iKey do tablicy cMessage
	for(k=0;k<256;k++) sbMssg->cChat[k] = cMessage[k+72]; 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int iGenerujIdGracza(FILE* name, vector<dane> tab)
{
	cout<<endl<<endl<<"GENERUJE ID NOWEGO GRACZA: "<<endl;
	int ID =1;


	for(unsigned int k=0;k<tab.size();k++)
	{
		if(tab[k].ID == ID) ID++;

	}

	 
	 	cout<<"\n\nNOWY GRACZ MA MIEC ID: "<<ID;

return ID;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////FUNKCJA ZWRACA CIAG BEDACY HASLEM GRACZA////////////////////////////////////////////////////////////////////////////////////////
void  vWybierzHaslo(Buffer* sbMssg, char* out)
{
	
	char pass[MAX_PASS_LEN];

	int start=0, end;
	int i=0;

	while(sbMssg->cChat[i] != ' ' && i <MAX_PASS_LEN-1)i++;
	start=i+1;
	i++;
	while((sbMssg->cChat[i] !=' '||sbMssg->cChat[i] !='\0')  && i <MAX_PASS_LEN)i++;
	end=i-1;
	int k=0;

	if(start<end) 
	{
		while(start!=end)
		{
			*(out+k)=sbMssg->cChat[start];
			k++;
			start++;
		}
	*(out+k+1)='\0';
}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////FUNKCJA ZWRACA CIAG BEDACY NAZWA GRACZA////////////////////////////////////////////////////////////////////////////////////////
void vWybierzImie(Buffer *sbMssg, char* out)
{
	

	char imie[MAX_NAME_LEN];
	
	for(int i=0; i<MAX_NAME_LEN;i++) imie[i]=0;
	
	int start=0, end;
	int i=0;
	
	while(sbMssg->cChat[i] != ' ')i++;
	end=i;
	int k=0;
	while(start!=end) imie[k++]=sbMssg->cChat[start++];
	while(k>=0) *(out+k)=*(imie+(k--));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////FUNKCJA POROWNUJE ZAWARTOSC HASEL I ZWRACA 1 JESLI SIE ZGADZAJA LUB 0 JESLI NIE///////////////////////////////////////
int iPorownajHaslo(char haslo1[], char haslo2[])
{
	
	return (!strcmp(haslo1,haslo2));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FUNKCJA ZWRACA DECYZJE O WYKONYWANEJ CZYNNOSCI////////////////////////////////////////////////////////////////////////////////////////
int iTranslate(Buffer* sbMssg)
{
return sbMssg->ID;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNKCJA NADPISUJE NOWE DANE O GRACZU NA PODSTAWIE STRUKTURY KLIENT ODPOWIADAJACEJ WATKOWI//////////////////////////////

void vUaktualnijDane(klient* gracz)
{
dane updated;
updated.cName[0]='\0';
updated.cPass[0]='\0';
updated.iCash=0;
updated.ID=0;
updated.iScore=0;
vector<dane> t;
FILE* login= fopen("login.txt", "rt+");
while(feof(login) == 0){
	fscanf(login, "%d", updated.ID);
	fscanf(login, "%s", updated.cName);
	fscanf(login, "%s", updated.cPass);
	fscanf(login, "%d", updated.iScore);
	fscanf(login, "%d", updated.iCash);
	if(updated.ID == gracz->ID) 
		{
			updated.iCash = gracz->iCash;
			updated.iScore = gracz->iScore;
		}
	t.push_back(updated);
}
fclose(login);
login= fopen("login.txt", "wt+");
fprintf(login, "%d %s %s %d %d\n", updated.ID , updated.cName, updated.cPass, updated.iScore, updated.iCash ); 
fclose(login);
login= fopen("login.txt", "wt+");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FUNKCJA ZASTEPUJE OBECNEGO GRACZA PRZY DANYM STOLIKU NA NASTEPNEGO////////////////////////////////////////////////////////////

void vNextPlayer(int IT)
{
	STOLIK[IT].iActivePlayer++;
	STOLIK[IT].iActivePlayer%=MAX_PLAYERS_PER_TABLE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vShuffle(bool baCardsDealt[]) {
	for (int iIndex = 0; iIndex < 52; ++iIndex) {
		baCardsDealt[iIndex] = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vEmptyHand(int  (*hand)[3][12], int index)
{
for(int i=0;i<12;i++) *hand[index][i]=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vWywal(table *STOL, klient* player)
{
	if(STOL[player->IT].iActivePlayer== player->ID) vNextPlayer(player->IT);
	if(STOL[player->IT].iRoundMask[player->iMySpot] == player->ID) 
	{
		for (int i=player->iMySpot;i<MAX_PLAYERS_PER_TABLE-1;i++)
			{
				STOL[player->IT].iRoundMask[i] = STOL[player->IT].iRoundMask[i +1];
				
			}
		STOL[player->IT].iRoundMask[MAX_PLAYERS_PER_TABLE-1]=0; // zapelniam ostatni index zerem by nowo dolaczajacy gracze znalezli sie na koncu
		player->IT=-1;
		player->iCash=300; // odnowienie standardowe zasobow pienieznych dla konta ktore spadlo ponizej limitu
}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	vRozeslijWynik(klient* player, int card)
{

	return;
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetNextCard(bool baCardsDealt[]) {
	bool bCardDealt	= true;
	int iNewCard	= -1;
	do {
		iNewCard = (rand() % 52);
		if (!baCardsDealt[iNewCard]) {
			bCardDealt = false;
		}
	} while (bCardDealt);
	return iNewCard;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ScoreHand(int iaHand[], const int kiCardCount) {
	int iAceCount	= 0;
	int iScore		= 0;
	for (int iCardIndex = 0; iCardIndex < kiCardCount; ++iCardIndex) {
		const int kiNextCard = iaHand[iCardIndex];
		const int kiRank = (kiNextCard % 13);
		if (kiRank == 0) {
			++iAceCount;
			++iScore;
		} else if (kiRank < 9) {
			iScore = iScore + (kiRank + 1);
		} else {
			iScore = iScore + 10;
		}
	}
	while (iAceCount > 0 && iScore < 12) {
		--iAceCount;
		iScore = iScore + 10;
	}
	return iScore;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//spr czy karty danego gracza pozwalaja mu kontynuowac gre
int ifend(table STOL, klient* gracz)
{
	if(ScoreHand(STOL.iaPlayerHand[gracz->iMySpot], STOL.iPlayerCardCount[gracz->iMySpot] ) < 22) return 1; // nie przekoroczono 21, mozna dalej grac
	else return 0;		//przekroczono 21, dla tego zawodnika gra sie wlasnie skonczyla
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//spr czy caly stolik zakonczyl rozgrywke?
int vCzyKoniec(table STOL, klient* gracz)														
{
	int p=0;
for(int i=0;i>MAX_PLAYERS_PER_TABLE;i++)
	p+= STOL.iActive[i];
if(p) return 0;
else return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uzupelniam karty krupiera
void complete_house(table* STOL, int g)
{		
	for(int i=2;i<12;i++) STOL[g].iaHouseHand[i]=	GetNextCard(STOL[g].baCardsDealt);
	STOL[g].iHouseCardCount=12;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// funkcja: oblicza ktory z graczy wygral a ktory przegral z krupierem a nastepnie wyniki graczy zapisuje do struktur im odpowiadajacym
// i rozsyla informacje o wygranych po klientach
void  vFindWinner(table* STOL, int IT, char* cMessage)
{
int k;
int g[MAX_PLAYERS_PER_TABLE];

k = ScoreHand(STOL[IT].iaHouseHand, STOL[IT].iHouseCardCount);
for (int i=0;i<MAX_PLAYERS_PER_TABLE;i++) 
	{
		g[i] = ScoreHand(STOL[IT].iaPlayerHand[i], STOL[IT].iPlayerCardCount[i]);
		//odnajdujemy odpowiedniego gracza
		int o,u, tmp;
		for(u=0;u<MAX_PLAYERS_PER_TABLE;u++)
		for(o=0;o<MAX_CLIENTS;o++) if(gracze[o].ID ==STOL[IT].ID[i]) tmp = o;
		Buffer RTRN;
		RTRN.ID=7;
		RTRN.iKey[0]=3;
		RTRN.iKey[1]=0;
		if(g[i]>k && g[i] < 22) //wygrana
		{
		gracze[tmp].iCash +=2* STOL[IT].iPlayerBid[i];	//	dodajemy wygrane
		gracze[tmp].iScore +=STOL[IT].iPlayerBid[i];		//	oraz 
		RTRN.iKey[1]=1;		
		}
	 sConversion(&RTRN, cMessage);
	 send(gracze[tmp].sock  , cMessage, DEFAULT_BUFLEN, 0);

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void jump_next(table *STOL, klient *gracz, char* cMessage)
{
	
	int next =0;
	// po pierwsze sprawdzamy czy sa inni gracze ktorzy moga grac w ta runde:
	for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOL[gracz->IT].iRoundMask[i]) next++;
	if(next >1){ // sa inni gracze
	//wyszukujemy kolejnego
		int w;
	do{
		vNextPlayer(gracz->IT);																																//ustalamy id kandydata na kolejn¹ rundê
		for(int i=0;i<MAX_PLAYERS_PER_TABLE;i++) if(STOL[gracz->IT].iActivePlayer == STOL[gracz->IT].ID[i]) w=i;						
	}
	while(!(STOL[gracz->IT].iActivePlayer && STOL[gracz->IT].iRoundMask[w]));														//spr czy kandydat ma prawo do prowadzenia rundy
	//w tym momencie wiemy, ktory gracz ma prawo do gry
	
	for(int i=0;i<MAX_CLIENTS;i++)  if(gracze[i].ID == STOL[gracz->IT].iActivePlayer) w= i; //odnajdujemy odpowiednia strukture w tablicy odpowiadajaca id aktywnego gracza

	Buffer RTRN;
	RTRN.ID=9;		// w przygotowanej strukturze wstawiamy informacje o oczekiwaniu na zapytanie silnika gry
	 sConversion(&RTRN, cMessage);
	 send(gracze[w].sock  , cMessage, DEFAULT_BUFLEN, 0); // i wysylamy wiadomosc do odpowiedniego kandydata
}
else
{		//tylko  gracz przyporzadkowany bierzacemu procesowi moze podejmowac dzialania
	Buffer RTRN;
	RTRN.ID=9;		// w przygotowanej strukturze wstawiamy informacje o oczekiwaniu na zapytanie silnika gry
	sConversion(&RTRN, cMessage);
	send(gracz->sock  , cMessage, DEFAULT_BUFLEN, 0); // i wysylamy wiadomosc do samego siebie :)
}
}


void ShutdownServer(struct klient clients[])
{
    // zamykamy niezamkniête socket'y klientów
    for (int i = 0; i < MAX_CLIENTS; i++) 
        if (clients[i].sock != INVALID_SOCKET)
        {
            closesocket (clients[i].sock);
            clients[i].sock = INVALID_SOCKET;
        }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int iIfFreeSlot(int active_players[])
{
int i=0;
for(int x=0;x<MAX_CLIENTS;x++)
	if(active_players[x]) i++;
return MAX_CLIENTS - i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int iFindFreeSlot(int active_players[])
{
int i=0;
while(active_players[i])i++;
return i+1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void vDebugPrintMssg(Buffer *sbMssg)
{
cout<<endl<<"ID\tID_USR\t"<<endl;
cout<<sbMssg->ID<<"\t"<<sbMssg->ID_USR<<endl;
cout<<"iKey:"<<endl;
for(int i=0;i<16;i++) cout<<sbMssg->iKey[i];
int x=0;
printf("CIAG cCHAT:\"");
while(sbMssg->cChat[x]  != '\0') 
{
	printf("%c", sbMssg->cChat[x]);
	if(x%16==15)cout <<endl;
	x++;
}
printf("\"\n\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLEAN_TABLES(table* STOL)
{
	for(int i=0;i<MAX_TABLES;i++)
	{
		for(int j=0;j<52;j++) STOL[i].baCardsDealt[j]=false;
		for(int j=0;j<MAX_PLAYERS_PER_TABLE;j++)STOL[i].iActive[j]=0;
		STOL[i].iSTARTED=0;
		STOL[i].iActivePlayer=0;
		for(int j=0;j<52;j++) STOL[i].baCardsDealt[j]=false;
		for(int j=0;j<12;j++) STOL[i].iaHouseHand[j] =0;
		for(int x=0;x<MAX_PLAYERS_PER_TABLE;x++)
			{
				STOL[i].ID[x]=0;
				STOL[i].iPlayerCardCount[x]=0;
				STOL[i].iPlayerCash[x]=0;
				STOL[i].iPlayerBid[x] = 0;
				for(int j=0;j<12;j++) STOL[i].iaPlayerHand[x][j]=0;
				STOL[i].iRoundMask[x] =0;
				STOL[i].iActive[x]=0;
			}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLEAN_CLIENTS(klient gracz[])
{
	for(int i=0;i<MAX_CLIENTS;i++)
	{
		gracz[i].thread=NULL;
		gracz[i].sock=INVALID_SOCKET;
		gracz[i].ID=0;
		gracz[i].IT=0;
		strcpy(gracz[i].cName, "\0");
		gracz[i].iScore=0;
		gracz[i].iCash=0;
		gracz[i].iMySpot=-1;
	}
}
