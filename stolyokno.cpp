#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

extern SOCKET sock;

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolWinMain();
extern HWND stolOkno;

HWND stolyOkno;
CONST CHAR ClassName[]="Lista sto³ów";
CONST CHAR MenuName[]="Menu_Window";

HWND hListStoly;
HWND hListGraczy;
HWND hListRanking;
HWND hListRankingPkt;

HWND hDolacz;
HWND hUtworz;

HWND hStolyChat;
HWND hStolyMail;
HWND hStolyWyslij;

HWND hStolyWyloguj;

HWND hlOnline;
HWND hlRanking;
HWND hlChat;
HWND hlRankingPkt;
HWND hlStoly;

struct Buffer{
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka�dego po��czonego z serwerem u�ytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

Buffer sbufferStoly;///////////obsluga naszej ramki dla okna Stoly
char pakiet[512]= {0};

/////////////////////////////////////////////////////////deklaracje////////////////////////////////////////////
void wyswietlListe(HWND hLista, int iKey);//pobiera odpowiednia liste poprzez iKey i wkleja do odpowiedniego hwnd
void wyswietlListeStolow(HWND hLista);
void dodajEnter(char str1[]);
void sklejChary(char str1[], char str2[]);
void pack(Buffer buff, char (&ref)[512]);
Buffer unpack(char ref[512]);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*

HWND CreateWindowEx
(
   DWORD dwExStyle,                 //rozszerzony styl okna
   LPCTSTR lpClassName,             //nazwa klasy okna do jakiej ma naleÂ¿eÃ¦ okno
   LPCTSTR lpWindowName,            //tytuÂ³ okna, bÃªdzie wyÅ“wietlany na pasku tytuÂ³u
   DWORD dwStyle,                   //podstawowy styl okna
   INT x,                           //wspÃ³Â³rzÃªdna x okna w pikselach
   INT y,                           //wspÃ³Â³rzÃªdna y okna w pikselach
   INT nWidth,                      //szerokoÅ“Ã¦ okna w pikselach
   INT nHeight,                     //wysokoÅ“Ã¦ okna w pikselach
   HWND hWndParent,                 //uchwyt okna rodzica, ma byÃ¦ to gÂ³Ã³wne okno, naszym rodzicem bÃªdzie pulpit, wiÃªc podajemy 0
   HMENU hMenu,                     //uchwyt do menu okna
   hInstMainANCE hInstMainance,     //uchwyt procesu do ktÃ³rego ma naleÂ¿eÃ¦ nasze okno, podajemy tu uchwyt swojego programu
   LPVOID lpParam                   //wskaÅ¸nik na dodatkowe informacje, ktÃ³re zostanÂ¹ przekazane z komunikatem tworzÂ¹cym okno
);

*/

LRESULT CALLBACK stolyWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         /*case WM_CREATE:
         {
            break;
         }*/
         case WM_CLOSE:       //polecenia dla komunikatu WM_CLOSE
         {
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hStolyWyslij)
             {
                GetWindowText(hStolyMail, sbufferStoly.cChat, 256);
                dodajEnter(sbufferStoly.cChat);
                /*
                sbufferStoly.ID=0;
                pack(sbufferStoly,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbufferStoly=unpack(pakiet);
                */
                SendMessage(hStolyChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbufferStoly.cChat) );
                SetWindowText(hStolyMail,"");
                SetFocus(hStolyMail);
             }
             else if((HWND)lPar==hDolacz)
             {
                int x = SendMessage(hListStoly, LB_GETANCHORINDEX ,LVNI_SELECTED,LVNI_SELECTED)+49;
                /*
                sbufferStoly.ID=4;
                sbufferStoly.iKey[0]=x;
                pack(sbufferStoly,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbufferStoly=unpack(pakiet);
                */
                sbufferStoly.ID_USR=0;//dla testow
                if(sbufferStoly.ID_USR==0)
                {
                ShowWindow(stolOkno,SW_SHOW);
                ShowWindow(stolyOkno,SW_HIDE);
                UpdateWindow(stolOkno);
                }
                else MessageBox(0,"Wystapil nieoczekiwany blad!","Ha!",MB_OK);
             }
             else if((HWND)lPar==hUtworz)
             {
                /*
                sbufferStoly.ID=5;
                pack(sbufferStoly,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbufferStoly=unpack(pakiet);
                */
                sbufferStoly.ID_USR=4;//dla testow serwer odsyla id stolu
                if(sbufferStoly.ID_USR!=0)
                {
                   int z=sbufferStoly.ID_USR+48;
                   char cWyraz[8]="Stol";
                   cWyraz[4]=' '; cWyraz[5]=z;
                   SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)cWyraz);
                }
                else MessageBox(0,"Nie udalo sie utworzyc stolu!","Ha!",MB_OK);
             }
             else if((HWND)lPar==hStolyWyloguj)
             {
                /*
                sbufferStoly.ID=2;
                pack(sbufferStoly,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbufferStoly=unpack(pakiet);
                */
                ShowWindow(Okno,SW_SHOW);
                ShowWindow(stolyOkno,SW_HIDE);
                UpdateWindow(Okno);
             }
             else if(wPar==10)
             {
                 //ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(Okno,SW_SHOW);
             }
             else if(wPar==11)
             {
                 ShowWindow(Okno,SW_HIDE);
                 //ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_SHOW);
             }
             else if(wPar==12)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
             }
             else if(wPar==13)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 //ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_SHOW);
             }
             else if(wPar==14)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
             }
             break;
         }
         default:
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyÅ“lna obsÂ³uga reszty komunikatÃ³w
        }
        return 0;
}

int WINAPI stolyWinMain ()
{
    //MSG stolymsgs;
    WNDCLASSEX stolywc;

    stolywc.hInstance = hInstMain;                                     //uchwyt programu
    stolywc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    stolywc.lpfnWndProc = stolyWndProc;                                //
    stolywc.style = 0;                                                 //
    stolywc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiÃªci
    stolywc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolywc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt maÂ³ej ikony okna
    stolywc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - sÂ³uÂ¿y do zaÂ³adowania kursora tzw. "strzaÂ³ki"
    stolywc.lpszMenuName = "Menu_Window";                              //nazwa menu
    stolywc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pÃªdzla" z tÂ³em
    stolywc.cbClsExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna klasy
    stolywc.cbWndExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna utworzona z tej klasy

    if(RegisterClassEx(&stolywc)==0) return 0;
    stolyOkno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);

    hlStoly=CreateWindowEx(0,"STATIC","Stoly",WS_CHILD|WS_VISIBLE,50,10,50,20,stolyOkno,0,hInstMain,0);
    hListStoly = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 30, 170, 180, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListeStolow(hListStoly);

    hlRanking=CreateWindowEx(0,"STATIC","Top 10",WS_CHILD|WS_VISIBLE,260,10,50,20,stolyOkno,0,hInstMain,0);
    hListRanking = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 30, 125, 180, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListRanking,1);

    hlRankingPkt=CreateWindowEx(0,"STATIC","Pkt",WS_CHILD|WS_VISIBLE,360,10,50,20,stolyOkno,0,hInstMain,0);
    hListRankingPkt = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 345, 30, 50, 180, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListRankingPkt,2);

    hDolacz=CreateWindowEx(0,"BUTTON","Dolacz",WS_CHILD|WS_VISIBLE,25,205,75,20,stolyOkno,0,hInstMain,0);
    hUtworz=CreateWindowEx(0,"BUTTON","Utworz",WS_CHILD|WS_VISIBLE,110,205,75,20,stolyOkno,0,hInstMain,0);

    hlOnline=CreateWindowEx(0,"STATIC","Online",WS_CHILD|WS_VISIBLE,470,10,50,20,stolyOkno,0,hInstMain,0);
    hListGraczy = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL,WS_VSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER, 425, 30, 140, 360, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListGraczy,3);

    hStolyWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,455,400,75,20,stolyOkno,0,hInstMain,0);

    hlChat=CreateWindowEx(0,"STATIC","Czat",WS_CHILD|WS_VISIBLE,50,240,50,20,stolyOkno,0,hInstMain,0);
    hStolyChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_DISABLED|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,20,260,375,130,stolyOkno,0,hInstMain,0);
    hStolyMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,20,400,290,20,stolyOkno,0,hInstMain,0);
    hStolyWyslij=CreateWindowEx(0,"BUTTON","Wyslij",WS_CHILD|WS_VISIBLE,320,400,75,20,stolyOkno,0,hInstMain,0);


	for(;; Sleep(10))
	{
		if(recv(sock,pakiet,sizeof(pakiet),0))
		{
		    sbufferStoly=unpack(pakiet);
		}
	}

    return 0;
}


