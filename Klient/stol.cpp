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
Buffer sbufferStol;
//Okna
extern HINSTANCE hInstMain;
extern HWND Okno;
extern int stolyWinMain();
extern HWND stolyOkno;
HWND stolOkno;
CONST CHAR ClassName[]="StolExample";
CONST CHAR MenuName[]="Menu_Window";
HWND stol;
//Listy tekstowe
extern HWND hListKartKrupier;
extern HWND hListKartGracz1;
extern HWND hListKartGracz2;
extern HWND hListKartGracz3;
extern HWND hListGraczyOn;
//Pola tekstowe
extern HWND hStolChat;
HWND hStolMail;
//Buttony
HWND hDobierz;
HWND hPodwoj;
HWND hStoj;
HWND hWstan;
HWND hOpuscStolik;
HWND hStolWyslij;
HWND hStolWyloguj;
//Etykiety
HWND hlKrupier;
HWND hlGracz1;
HWND hlGracz2;
HWND hlGracz3;
//Deklaracje funkcji
void dodajEnter(char str1[]);
void sklejChary(char str1[], char str2[]);
void pack(Buffer *sbMssg, char cMessage[]);
void czyscBuffer(Buffer tempbuff);
void pobierzListyStolyOkno();
void pobierzListyStol();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK stolWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         case WM_CLOSE:
         {
            czyscBuffer(sbufferStol);
            sbufferStol.ID=6;
            pack(&sbufferStol,pakiet);
            kolejka.push(pakiet);
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hDobierz)
             {
                czyscBuffer(sbufferStol);
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=1;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
            }

             else if((HWND)lPar==hPodwoj)
             {
                czyscBuffer(sbufferStol);
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=2;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
             }
             else if((HWND)lPar==hStoj)
             {
                czyscBuffer(sbufferStol);
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=0;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
             }
             else if((HWND)lPar==hOpuscStolik)
             {
                czyscBuffer(sbufferStol);
                sbufferStol.ID=5;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
                pobierzListyStolyOkno();
                pobierzListyStol();
                ShowWindow(stolyOkno,SW_SHOW);
                ShowWindow(stolOkno,SW_HIDE);
                UpdateWindow(stolyOkno);
             }
             else if((HWND)lPar==hStolWyloguj)
             {
                czyscBuffer(sbufferStol);
                sbufferStol.ID=2;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
                ShowWindow(Okno,SW_SHOW);
                ShowWindow(stolyOkno,SW_HIDE);
                UpdateWindow(Okno);
             }
             else if((HWND)lPar==hStolWyslij)
             {
                czyscBuffer(sbufferStol);
                GetWindowText(hStolMail, sbufferStol.cChat, 256);
                dodajEnter(sbufferStol.cChat);
                sbufferStol.ID=0;
                pack(&sbufferStol,pakiet);
                kolejka.push(pakiet);
                SetWindowText(hStolMail,"");
                SetFocus(hStolMail);
             }
             if(wPar==10)
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
             else if(wPar==13)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_SHOW);
             }
             break;
         }
         default:
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyÅlna obsÂ³uga reszty komunikatÃ³w
        }
        return 0;
}

int WINAPI stolWinMain ()
{
    WNDCLASSEX stolwc;

    stolwc.hInstance = hInstMain;                                         //uchwyt programu
    stolwc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    stolwc.lpfnWndProc = stolWndProc;                                //
    stolwc.style = 0;                                                 //
    stolwc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiÃªci
    stolwc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolwc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt maÂ³ej ikony okna
    stolwc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - sÂ³uÂ¿y do zaÂ³adowania kursora tzw. "strzaÂ³ki"
    stolwc.lpszMenuName = "Menu_Window";                                          //nazwa menu
    stolwc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pÃªdzla" z tÂ³em
    stolwc.cbClsExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna klasy
    stolwc.cbWndExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna utworzona z tej klasy

    // Okno
    if(RegisterClassEx(&stolwc)==0) return 0;
    stolOkno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,550,Okno,0,hInstMain,0);

    // Stol
    stol=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC",0,WS_CHILD|WS_VISIBLE,40,30,340,280,stolOkno,0,hInstMain,0);

    // Krupier
    hlKrupier=CreateWindowEx(0,"STATIC","Krupier",WS_CHILD|WS_VISIBLE,20,20,50,20,stol,0,hInstMain,0);
    hListKartKrupier= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 40, 60, 190, stol, NULL, hInstMain, NULL);

    // Gracz 1
    hlGracz1=CreateWindowEx(0,"STATIC","Gracz 1",WS_CHILD|WS_VISIBLE,100,20,50,20,stol,0,hInstMain,0);
    hListKartGracz1= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 40, 60, 190, stol, NULL, hInstMain, NULL);

    // Gracz 2
    hlGracz2=CreateWindowEx(0,"STATIC","Gracz 2",WS_CHILD|WS_VISIBLE,180,20,50,20,stol,0,hInstMain,0);
    hListKartGracz2= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 180, 40, 60, 190, stol, NULL, hInstMain, NULL);

    // Gracz 3
    hlGracz3=CreateWindowEx(0,"STATIC","Gracz 3",WS_CHILD|WS_VISIBLE,260,20,50,20,stol,0,hInstMain,0);
    hListKartGracz3= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 260, 40, 60, 190, stol, NULL, hInstMain, NULL);

    // Gra
    hDobierz=CreateWindowEx(0,"BUTTON","Dobierz",WS_CHILD|WS_VISIBLE,60,240,60,20,stol,0,hInstMain,0);
    hPodwoj=CreateWindowEx(0,"BUTTON","Podwoj",WS_CHILD|WS_VISIBLE,140,240,60,20,stol,0,hInstMain,0);
    hStoj=CreateWindowEx(0,"BUTTON","Pasuj",WS_CHILD|WS_VISIBLE,220,240,60,20,stol,0,hInstMain,0);

    // Lista graczy przy stole
    hListGraczyOn = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 405, 30, 147, 241, stolOkno, NULL, hInstMain, NULL);

    // Czat
    hStolChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_DISABLED|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,40,330,510,101,stolOkno,0,hInstMain,0);
    hStolMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,441,261,20,stolOkno,0,hInstMain,0);
    hStolWyslij=CreateWindowEx(0,"BUTTON","Wyslij",WS_CHILD|WS_VISIBLE,320,441,61,20,stolOkno,0,hInstMain,0);

    // Koniec gry
    hOpuscStolik=CreateWindowEx(0,"BUTTON","Opusc Stolik",WS_CHILD|WS_VISIBLE,430,272,100,20,stolOkno,0,hInstMain,0);
    hStolWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,450,441,70,20,stolOkno,0,hInstMain,0);

    return 0;
}
