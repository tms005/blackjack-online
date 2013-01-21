#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED
#define SPACE ' '

    SOCKADDR_IN saddr;
    SOCKET      sock;

struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
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

void wyswietlListeStolow(HWND hLista)
{
    Buffer tempbuff;

    tempbuff.iKey[4];//dla testow
    tempbuff.iKey[0]=1;
    tempbuff.iKey[1]=2;
    tempbuff.iKey[2]=3;
    tempbuff.iKey[3]=4;
    /*
    tempbuff.ID=3;
    tempbuff.iKey[0]=0;
    pack(tempbuff,pakiet);
    send(sock,pakiet,sizeof(pakiet),0);
    recv(sock,pakiet,sizeof(pakiet),0);
    tempbuff=unpack(pakiet);
    */
    char cWyraz[20]="Stol ";
    for(int d = 0;d<4;d++){
        if(tempbuff.iKey[d]!=0) {
                cWyraz[5]=tempbuff.iKey[d]+48;
                SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)cWyraz);
            }
        }
}

void wyswietlListe(HWND hLista,int iKey)
{
    Buffer tempbuff;

    char cChat[256] = "janusz zoska wakus homus janusz zoska wakus homus janusz zoska wakus homus ";//dla testow
    przepiszChary(tempbuff.cChat, cChat);//dla testow - wyjebac
    /*
    tempbuff.ID=3;
    tempbuff.iKey[0]=iKey;
    pack(tempbuff,pakiet);
    send(sock,pakiet,sizeof(pakiet),0);
    recv(sock,pakiet,sizeof(pakiet),0);
    tempbuff=unpack(pakiet);
    */
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

void dodajEnter(char str1[])
{
     CHAR cEnter[256]="\r\n";
     unsigned int i, hold;
     hold = strlen(str1);
	 for(i=0;i<=strlen(cEnter);i++)
		 str1[hold++] = cEnter[i];
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
#endif // FUNKCJE_H_INCLUDED
