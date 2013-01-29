#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <queue>

using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Gniazda
extern SOCKET sock;
//Struktury danych
struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};
//Zmienne danych
extern char pakiet[512];
extern queue <char*> kolejka;
Buffer sbufferStoly;
//Okna
extern HINSTANCE hInstMain;
extern HWND Okno;
extern int stolWinMain();
extern HWND stolOkno;
CONST CHAR ClassName[]="Lista stoÂłĂłw";
CONST CHAR MenuName[]="Menu_Window";
HWND stolyOkno;
//Listy tekstowe
extern HWND hListStoly;
extern HWND hListGraczy;
extern HWND hListRanking;
extern HWND hListRankingPkt;
//Pola tekstowe
extern HWND hStolyChat;
HWND hStolyMail;
//Buttony
HWND hDolacz;
HWND hStolyWyslij;
HWND hStolyWyloguj;
//Etykiety
HWND hlOnline;
HWND hlRanking;
HWND hlChat;
HWND hlRankingPkt;
HWND hlStoly;
//Deklaracje funkcji
void dodajEnter(char str1[]);
void sklejChary(char str1[], char str2[]);
void pack(Buffer *sbMssg, char cMessage[]);
void czyscBuffer(Buffer tempbuff);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK stolyWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         case WM_CLOSE:       //polecenia dla komunikatu WM_CLOSE
         {
            czyscBuffer(sbufferStoly);
            sbufferStoly.ID=6;
            pack(&sbufferStoly,pakiet);
            kolejka.push(pakiet);
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hStolyWyslij)
             {
                czyscBuffer(sbufferStoly);
                GetWindowText(hStolyMail, sbufferStoly.cChat, 256);
                dodajEnter(sbufferStoly.cChat);
                sbufferStoly.ID=0;
                pack(&sbufferStoly,pakiet);
                kolejka.push(pakiet);
                SetWindowText(hStolyMail,"");
                SetFocus(hStolyMail);
             }
             else if((HWND)lPar==hDolacz)
             {
                czyscBuffer(sbufferStoly);
                int y = SendMessage(hListStoly, LB_GETANCHORINDEX ,LVNI_SELECTED,LVNI_SELECTED)+49;
                sbufferStoly.ID=4;
                sbufferStoly.iKey[0]=y;
                pack(&sbufferStoly,pakiet);
                kolejka.push(pakiet);
             }
             else if((HWND)lPar==hStolyWyloguj)
             {
                czyscBuffer(sbufferStoly);
                sbufferStoly.ID=2;
                pack(&sbufferStoly,pakiet);
                kolejka.push(pakiet);
                ShowWindow(Okno,SW_SHOW);
                ShowWindow(stolyOkno,SW_HIDE);
                UpdateWindow(Okno);
             }
             else if(wPar==10)
             {
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(Okno,SW_SHOW);
             }
             else if(wPar==11)
             {
                 ShowWindow(Okno,SW_HIDE);
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
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyĂ…â€ślna obsĂ‚Âługa reszty komunikatĂÂłw
        }
        return 0;
}

int WINAPI stolyWinMain ()
{
    WNDCLASSEX stolywc;

    stolywc.hInstance = hInstMain;                                     //uchwyt programu
    stolywc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    stolywc.lpfnWndProc = stolyWndProc;                                //
    stolywc.style = 0;                                                 //
    stolywc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiĂÂŞci
    stolywc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolywc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt maĂ‚Âłej ikony okna
    stolywc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - sĂ‚ÂłuĂ‚Âży do zaĂ‚Âładowania kursora tzw. "strzaĂ‚Âłki"
    stolywc.lpszMenuName = "Menu_Window";                              //nazwa menu
    stolywc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pĂÂŞdzla" z tĂ‚Âłem
    stolywc.cbClsExtra = 0;                                            //dodatkowa pamiĂÂŞĂÂ¦ dla okna klasy
    stolywc.cbWndExtra = 0;                                            //dodatkowa pamiĂÂŞĂÂ¦ dla okna utworzona z tej klasy

    // Okno
    if(RegisterClassEx(&stolywc)==0) return 0;
    stolyOkno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);

    // Lista stolow
    hlStoly=CreateWindowEx(0,"STATIC","Stoly",WS_CHILD|WS_VISIBLE,50,10,50,20,stolyOkno,0,hInstMain,0);
    hListStoly = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 30, 170, 180, stolyOkno, NULL, hInstMain, NULL);
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"Stol 1");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"Stol 2");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"Stol 3");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"Stol 4");

    // Ranking - nicki
    hlRanking=CreateWindowEx(0,"STATIC","Top 10",WS_CHILD|WS_VISIBLE,260,10,50,20,stolyOkno,0,hInstMain,0);
    hListRanking = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 30, 125, 180, stolyOkno, NULL, hInstMain, NULL);

    // Ranking - pkt
    hlRankingPkt=CreateWindowEx(0,"STATIC","Pkt",WS_CHILD|WS_VISIBLE,360,10,50,20,stolyOkno,0,hInstMain,0);
    hListRankingPkt = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 345, 30, 50, 180, stolyOkno, NULL, hInstMain, NULL);

    // Gracze online
    hlOnline=CreateWindowEx(0,"STATIC","Online",WS_CHILD|WS_VISIBLE,470,10,50,20,stolyOkno,0,hInstMain,0);
    hListGraczy = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL,WS_VSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER, 425, 30, 140, 360, stolyOkno, NULL, hInstMain, NULL);

    // Czat
    hlChat=CreateWindowEx(0,"STATIC","Czat",WS_CHILD|WS_VISIBLE,50,240,50,20,stolyOkno,0,hInstMain,0);
    hStolyChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_DISABLED|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,20,260,375,130,stolyOkno,0,hInstMain,0);
    hStolyMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,20,400,290,20,stolyOkno,0,hInstMain,0);
    hStolyWyslij=CreateWindowEx(0,"BUTTON","Wyslij",WS_CHILD|WS_VISIBLE,320,400,75,20,stolyOkno,0,hInstMain,0);

    // Buttony
    hStolyWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,455,400,75,20,stolyOkno,0,hInstMain,0);
    hDolacz=CreateWindowEx(0,"BUTTON","Dolacz",WS_CHILD|WS_VISIBLE,25,205,75,20,stolyOkno,0,hInstMain,0);

    return 0;
}


