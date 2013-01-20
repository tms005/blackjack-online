#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED
#define SPACE ' '

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

void wyswietlListe(HWND hLista,int iKey)
{
    char cChat[256] = "janusz zoska wakus homus janusz zoska wakus homus janusz zoska wakus homus ";//dla testow
    /*
    send();//wyslanie zadania o funkcje wysylajaca liste z podanym iKey zeby wiedzial jaka liste
    recv();//odebranie listy w polu cChat
    */
    char cWyraz[256];
    memset(cWyraz, 0, 256);
    int hold,dn=0;
    hold = strlen(cChat);
    for(int d = 0;d<hold;d++){
        if(cChat[d]!=' ') {
                cWyraz[dn]=cChat[d];
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

#endif // FUNKCJE_H_INCLUDED
