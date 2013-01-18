#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolWinMain();
extern HWND stolOkno;

//HWND stolyhEdit;
HWND stolyOkno;
CONST CHAR ClassName[]="Lista sto��w";
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

void wyswietlListe(HWND hLista, int iKey);//pobiera odpowiednia liste poprzez iKey i wkleja do odpowiedniego hwnd
void dodajenter(char str1[]);
/*

HWND CreateWindowEx
(
   DWORD dwExStyle,                 //rozszerzony styl okna
   LPCTSTR lpClassName,             //nazwa klasy okna do jakiej ma nale¿eæ okno
   LPCTSTR lpWindowName,            //tytu³ okna, bêdzie wyœwietlany na pasku tytu³u
   DWORD dwStyle,                   //podstawowy styl okna
   INT x,                           //wspó³rzêdna x okna w pikselach
   INT y,                           //wspó³rzêdna y okna w pikselach
   INT nWidth,                      //szerokoœæ okna w pikselach
   INT nHeight,                     //wysokoœæ okna w pikselach
   HWND hWndParent,                 //uchwyt okna rodzica, ma byæ to g³ówne okno, naszym rodzicem bêdzie pulpit, wiêc podajemy 0
   HMENU hMenu,                     //uchwyt do menu okna
   hInstMainANCE hInstMainance,     //uchwyt procesu do którego ma nale¿eæ nasze okno, podajemy tu uchwyt swojego programu
   LPVOID lpParam                   //wskaŸnik na dodatkowe informacje, które zostan¹ przekazane z komunikatem tworz¹cym okno
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
            //ShowWindow(hwnd,SW_HIDE);
            //ShowWindow(Okno,SW_SHOW);
            //SendMessage(Okno,WM_DESTROY,0,0);
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hStolyWyslij)
             {
                CHAR cChat[256];
                GetWindowText(hStolyMail, cChat, 256);
                dodajenter(cChat);
                /*
                send();
                recv();
                */
                SendMessage(hStolyChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cChat) );
                SetWindowText(hStolyMail,"");
                SetFocus(hStolyMail);
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
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyœlna obs³uga reszty komunikatów
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
    stolywc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    stolywc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolywc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    stolywc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    stolywc.lpszMenuName = "Menu_Window";                              //nazwa menu
    stolywc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    stolywc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    stolywc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&stolywc)==0) return 0;
    stolyOkno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);
    //ShowWindow(dalej_Okno,SW_SHOW);
    //UpdateWindow(dalej_Okno);

    hlStoly=CreateWindowEx(0,"STATIC","Stoly",WS_CHILD|WS_VISIBLE,50,10,50,20,stolyOkno,0,hInstMain,0);
    hListStoly = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 30, 170, 180, stolyOkno, NULL, hInstMain, NULL);

    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 1");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 2");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 3");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 4");

    hlRanking=CreateWindowEx(0,"STATIC","Top 10",WS_CHILD|WS_VISIBLE,260,10,50,20,stolyOkno,0,hInstMain,0);
    hListRanking = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 30, 125, 180, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListRanking,1);

    hlRankingPkt=CreateWindowEx(0,"STATIC","Pkt",WS_CHILD|WS_VISIBLE,360,10,50,20,stolyOkno,0,hInstMain,0);
    hListRankingPkt = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 345, 30, 50, 180, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListRankingPkt,2);

    hDolacz=CreateWindowEx(0,"BUTTON","Do��cz",WS_CHILD|WS_VISIBLE,25,205,75,20,stolyOkno,0,hInstMain,0);
    hUtworz=CreateWindowEx(0,"BUTTON","Utw�rz",WS_CHILD|WS_VISIBLE,110,205,75,20,stolyOkno,0,hInstMain,0);

    hlOnline=CreateWindowEx(0,"STATIC","Online",WS_CHILD|WS_VISIBLE,470,10,50,20,stolyOkno,0,hInstMain,0);
    hListGraczy = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL,WS_VSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER, 425, 30, 140, 360, stolyOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListGraczy,3);

    hStolyWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,455,400,75,20,stolyOkno,0,hInstMain,0);

    hlChat=CreateWindowEx(0,"STATIC","Czat",WS_CHILD|WS_VISIBLE,50,240,50,20,stolyOkno,0,hInstMain,0);
    hStolyChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,20,260,375,130,stolyOkno,0,hInstMain,0);
    hStolyMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,20,400,290,20,stolyOkno,0,hInstMain,0);
    hStolyWyslij=CreateWindowEx(0,"BUTTON","Wy�lij",WS_CHILD|WS_VISIBLE,320,400,75,20,stolyOkno,0,hInstMain,0);

    return 0;
}


void wyswietlListe(HWND hLista,int iKey)
{
    char cChar[256] = "janusz zoska wakus homus janusz zoska wakus homus janusz zoska wakus homus ";//dla testow
    /*
    send();
    recv();
    */
    char cWyraz[256];
    memset(cWyraz, 0, 256);
    int hold,dn=0;
    hold = strlen(cChar);
    for(int d = 0;d<hold;d++){
        if(cChar[d]!=' ') {
                cWyraz[dn]=cChar[d];
                dn++;
            }
        else {
                SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)cWyraz);
                memset(cWyraz, 0, 256);
                dn=0;
            }
    }
}

void dodajenter(char str1[])
{
     CHAR cEnter[256]="\r\n";
     int i, hold;
     hold = strlen(str1);
	 for(i=0;i<=strlen(cEnter);i++)
		 str1[hold++] = cEnter[i];
}
