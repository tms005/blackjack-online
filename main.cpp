#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "funkcje.h"

extern int stolyWinMain();
extern HWND stolyOkno;

extern int stolWinMain();
extern HWND stolOkno;

HWND hEdit;
CONST CHAR ClassName[]="GameWindow";
CONST CHAR MenuName[]="Menu_Window";

MSG msgs; //struktura na komunikatów
HINSTANCE hInstMain;

HWND Okno;
HWND hLogo;
HWND hNick;
HWND hNickWpisz;
HWND hPass;
HWND hPassWpisz;
HWND hLogin;

HWND hRejestr;

/////////////////tutaj umieszczamy deklaracje funkcji bo niektore korzystaja z deklaracji hwnd///////////////////
void zaloguj(int iKey);
void rejestruj(int iKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
   HINSTANCE hInstance,             //uchwyt procesu do którego ma nale¿eæ nasze okno, podajemy tu uchwyt swojego programu
   LPVOID lpParam                   //wskaŸnik na dodatkowe informacje, które zostan¹ przekazane z komunikatem tworz¹cym okno
);

*/

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         case WM_CREATE:
         {
             stolyWinMain();
             stolWinMain();
             break;
         }
         case WM_DESTROY:       //polecenia dla komunikatu WM_CLOSE
         {
            PostQuitMessage(0);

            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hLogin)
             {
                EnableWindow(hLogin, false);
                EnableWindow(hRejestr, false);
                CHAR cLogin[20];
                GetWindowText(hNickWpisz, cLogin, 20);
                CHAR cPass[20];
                GetWindowText(hPassWpisz, cPass, 20);
                sklejChary(cLogin,cPass);//skleja login z haslem oddzielajac spacja
                /*
                send();
                recv();
                */
                int iKey=1;//do testtow
                zaloguj(iKey);//funkcja do logowania
                EnableWindow(hLogin, true);
                EnableWindow(hRejestr, true);
             }
             else if((HWND)lPar==hRejestr)
             {
                EnableWindow(hLogin, false);
                EnableWindow(hRejestr, false);
                CHAR cLogin[20];
                GetWindowText(hNickWpisz, cLogin, 20);
                CHAR cPass[20];
                GetWindowText(hPassWpisz, cPass, 20);
                sklejChary(cLogin,cPass);//skleja login z haslem oddzielajac spacja
                /*
                send();
                recv();
                */
               int iKey=1;//do testtow
               rejestruj(iKey);//funkcja do rejestracji
               EnableWindow(hLogin, true);
               EnableWindow(hRejestr, true);
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
                 //ShowWindow(rejestrOkno,SW_HIDE);
             }
             break;
         }
         default:
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyœlna obs³uga reszty komunikatów
        }
        return 0;
}

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShow)
{
    WNDCLASSEX wc;

    wc.hInstance = hInst;                                         //uchwyt programu
    hInstMain=hInst;
    wc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    wc.lpfnWndProc = WndProc;                                     //
    wc.style = 0;                                                 //
    wc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    wc.lpszMenuName = "";                                          //nazwa menu
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    wc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    wc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&wc)==0) return 0;
    Okno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_VISIBLE,80,50,600,500,0,0,hInst,0);

    hLogo=CreateWindowEx(0,"STATIC","Logowanie",WS_CHILD|WS_VISIBLE,280,60,150,20,Okno,0,hInst,0);
    //SetWindowText(hlogo,"BlackJack - Logowanie");
    hNick=CreateWindowEx(0,"STATIC","Login: ",WS_CHILD|WS_VISIBLE,160,110,150,20,Okno,0,hInst,0);
    //SetWindowText(hnick,"Nick : ");
    hNickWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,220,110,200,20,Okno,0,hInst,0);
    hPass=CreateWindowEx(0,"STATIC","Has�o:  ",WS_CHILD|WS_VISIBLE,160,170,50,20,Okno,0,hInst,0);
    //SetWindowText(hpass,"Has�o : ");
    hPassWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_PASSWORD,220,170,200,20,Okno,0,hInst,0);
    hLogin=CreateWindowEx(0,"BUTTON","Logowanie",WS_CHILD|WS_VISIBLE,265,230,100,20,Okno,0,hInst,0);
    hRejestr = CreateWindowEx(0,"BUTTON","Rejestracja",WS_VISIBLE|WS_CHILD,265,270,100,20,Okno,0,hInst,0);
    //SetWindowText(hRejestr,"Rejestracja");
    //ShowWindow(Okno,nShow);

    /*WSADATA wsaData;
    SOCKADDR_IN saddr;
    SOCKET      sock;
    char        *http_req = "";
    char        index[1024*20] = { 0 };

    WSAStartup( MAKEWORD(2,2), &amp;amp;wsaData );

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    saddr.sin_addr.S_un.S_addr = resolveHost ("");
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);

    if ( connect(sock, (sockaddr*)&saddr, sizeof(sockaddr)) == SOCKET_ERROR )
    {
         po��czenie si� nie powiod�o
        sock = NULL;
        return -1;
    }*/

    while(GetMessage(&msgs,0,0,0)) //pêtla obs³uguj¹ca wymianê komunikatów
    {
                                   TranslateMessage(&msgs); //funkcja t³umacz¹ca sygna³y z klawiatury na odpowiednie komunikaty systemowe
                                   DispatchMessage(&msgs); //funkcja przetwarzaj¹ca komunikaty systemowe przez procedury obs³ugi
    }
    return msgs.wParam;
}

void zaloguj(int iKey)
{
    switch(iKey)
                {
                case 1: //jestes zalogowany
                        ShowWindow(stolyOkno,SW_SHOW);
                        ShowWindow(Okno,SW_HIDE);
                        UpdateWindow(stolyOkno);
                    break;

                case 2: //serwer full
                        MessageBox(0,"Serwer przepelniony, spr�buj p�niej!","Ha!",MB_OK);
                    break;

                case 3: //bledne dane logowania
                        MessageBox(0,"Niepoprawne dane, spr�buj jeszcze raz!","Ha!",MB_OK);
                    break;

                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad!","Ha!",MB_OK);;
                }
}

void rejestruj(int iKey)
{
    switch(iKey)
                {
                case 1: //konto utworzone
                        MessageBox(0,"Konto utworzone!\nZostaniesz automatycznie zalogowany!","Ha!",MB_OK);
                        /*
                        send();
                        recv();
                        */
                        zaloguj(iKey);//automatyczne zalogowanie po utworzeniu konta
                    break;

                case 2: //konto istnieje
                        MessageBox(0,"Takie konto juz istnieje!","Ha!",MB_OK);
                    break;
                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad!","Ha!",MB_OK);;
                }
}
