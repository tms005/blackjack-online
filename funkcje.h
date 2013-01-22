#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED
#define SPACE ' '

SOCKADDR_IN saddr;
SOCKET      sock;

HWND hListStoly;
HWND hListGraczy;
HWND hListRanking;
HWND hListRankingPkt;
HWND hListGraczyOn;

HWND hStolyChat;
HWND hStolChat;

struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

struct Stol {
int iIdGraczy[3];//klucze id graczy przy stole
char cNazwyGraczy[3][20];//nazwy graczy przy stole
int iStawka;//stawka postawiona
int iKonto;//stan konto klienta
char cKarty[4];//karty wszystkich graczy wlacznie z krupierem pod indeksem 0
};

void pack(Buffer buff, char (&ref)[512]) //pakowanie - klient
{
    unsigned int i,j=0;

     ref[0]=buff.ID;
     ref[1]=buff.ID_USR;

    for(i=2;i<sizeof(buff.iKey);i++)
    {
        ref[i]=buff.iKey[j];
        j++;
    }

    ref[i]=SPACE;
    i++;

    for(j=0;j<sizeof(buff.cChat);i++)
    {
        ref[i]=buff.cChat[j];
        j++;
    }
}

Buffer unpack(char ref[512]) //odpakowanie - klient
{
     unsigned int i,j=0;
     Buffer tempbuff;

        tempbuff.ID=ref[0];
        tempbuff.ID_USR=ref[1];

    for(i=2;i<sizeof(tempbuff.iKey);i++)
    {
        tempbuff.iKey[j]=ref[i]; j++;
    } i++;

    for(j=0;j<sizeof(tempbuff.cChat);i++)
    {
        tempbuff.cChat[j]=ref[i]; j++;
    }
return tempbuff;
}

void sklejChary(char str1[], char str2[])
{
     unsigned int i, hold;
     hold = strlen(str1);
     str1[hold] = SPACE;
	 for(i=0;i<=strlen(str2);i++)
		 str1[++hold] = str2[i];
}

void przepiszChary(char str1[], char str2[])
{
     unsigned int i;
     memset(str1, 0, 256);
	 for(i=0;i<=strlen(str2);i++)
		 str1[i] = str2[i];
}

void dodajEnter(char str1[])
{
     CHAR cEnter[256]="\r\n";
     unsigned int i, hold;
     hold = strlen(str1);
	 for(i=0;i<=strlen(cEnter);i++)
		 str1[hold++] = cEnter[i];
}

void wyswietlListe(HWND hLista, Buffer tempbuff)
{
    char cWyraz[256];
    memset(cWyraz, 0, 256);
    unsigned int dn=0;
    for(unsigned int d = 0;d<strlen(tempbuff.cChat);d++){
        if(tempbuff.cChat[d]!=' ') {
                cWyraz[dn]=tempbuff.cChat[d];
                dn++;
            }
        else {
                SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)cWyraz);
                memset(cWyraz, 0, 256);
                dn=0;
            }
    }
}

void wyswietlListeGraczyOn(HWND hLista, Stol tempstol)
{
    for(int i=0;i<3;i++)
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)tempstol.cNazwyGraczy[i]);
}

void PrintCard(int iCard, char cKarta[]) {
	using namespace std;
	// Print Rank
	const int kiRank = (iCard % 13);
	if (kiRank == 0) {
		cKarta[0]='A';
	} else if (kiRank < 9) {
		cKarta[0]=(char)(kiRank + 1);
	} else if (kiRank == 9) {
		cKarta[0]='T';
	} else if (kiRank == 10) {
		cKarta[0]='J';
	} else if (kiRank == 11) {
		cKarta[0]='Q';
	} else {
		cKarta[0]='K';
	}
	// Print Suit
	const int kiSuit = (iCard/13);
	if (kiSuit == 0) {
		cKarta[1]='C';
	} else if (kiSuit == 1) {
		cKarta[1]='D';
	} else if (kiSuit == 2) {
		cKarta[1]='H';
	} else {
		cKarta[1]='S';
	}
}

void pobierzListyStolyOkno()
{
    Buffer tempbuff;
    char tempakiet[512] = {0};

    tempbuff.ID=3;
    tempbuff.iKey[0]=1;//graczy w rankingu
    pack(tempbuff,tempakiet);
    send(sock,tempakiet,sizeof(tempakiet),0);
    Sleep(50);
    tempbuff.iKey[0]=2;//graczy online
    pack(tempbuff,tempakiet);
    send(sock,tempakiet,sizeof(tempakiet),0);
}

void pobierzListyStol()
{
    Buffer tempbuff;
    char tempakiet[512] = {0};

    tempbuff.ID=3;
    tempbuff.iKey[0]=0;//info o stolach
    pack(tempbuff,tempakiet);
    send(sock,tempakiet,sizeof(tempakiet),0);
}

#endif // FUNKCJE_H_INCLUDED
