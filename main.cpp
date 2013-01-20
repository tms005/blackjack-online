#pragma comment(lib, "Ws2_32.lib")
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

MSG msgs; //struktura na komunikatÃ³w
HINSTANCE hInstMain;

HWND Okno;
HWND hLogo;
HWND hNick;
HWND hNickWpisz;
HWND hPass;
HWND hPassWpisz;
HWND hLogin;
HWND hRejestr;

Buffer sbuffer;///////////obsluga naszej ramki

/////////////////tutaj umieszczamy deklaracje funkcji bo niektore korzystaja z deklaracji hwnd///////////////////
void zaloguj(int ID);
void rejestruj(int ID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=2;
                pack(sbuffer,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbuffer=unpack(pakiet);
                */
                sbuffer.ID=1;//do testtow
                zaloguj(sbuffer.ID);//funkcja do logowania
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
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=1;
                pack(sbuffer,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
                recv(sock,pakiet,sizeof(pakiet),0);
                sbuffer=unpack(pakiet);
                */
                int ID=1;//do testtow
                rejestruj(ID);//funkcja do rejestracji
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
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyÅlna obsÂ³uga reszty komunikatÃ³w
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
    wc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiÃªci
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt maÂ³ej ikony okna
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - sÂ³uÂ¿y do zaÂ³adowania kursora tzw. "strzaÂ³ki"
    wc.lpszMenuName = "";                                          //nazwa menu
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pÃªdzla" z tÂ³em
    wc.cbClsExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna klasy
    wc.cbWndExtra = 0;                                            //dodatkowa pamiÃªÃ¦ dla okna utworzona z tej klasy

    if(RegisterClassEx(&wc)==0) return 0;
    Okno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_VISIBLE,80,50,600,500,0,0,hInst,0);

    hLogo=CreateWindowEx(0,"STATIC","Logowanie",WS_CHILD|WS_VISIBLE,280,60,150,20,Okno,0,hInst,0);
    hNick=CreateWindowEx(0,"STATIC","Login: ",WS_CHILD|WS_VISIBLE,160,110,150,20,Okno,0,hInst,0);
    hNickWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,220,110,200,20,Okno,0,hInst,0);

    hPass=CreateWindowEx(0,"STATIC","Haslo:  ",WS_CHILD|WS_VISIBLE,160,170,50,20,Okno,0,hInst,0);
    hPassWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_PASSWORD,220,170,200,20,Okno,0,hInst,0);

    hLogin=CreateWindowEx(0,"BUTTON","Logowanie",WS_CHILD|WS_VISIBLE,265,230,100,20,Okno,0,hInst,0);
    hRejestr = CreateWindowEx(0,"BUTTON","Rejestracja",WS_VISIBLE|WS_CHILD,265,270,100,20,Okno,0,hInst,0);


    //laczenie z serwerem

   /* int RetVal = 0;
        WSAData wsaData;
        WORD DllVersion = MAKEWORD(2,1);
        RetVal = WSAStartup(DllVersion, &wsaData);
        if(RetVal != 0)
        {
                MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
                exit(1);
        }

    SOCKADDR_IN saddr;
    SOCKET      sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        saddr.sin_port        = htons(27015);
        saddr.sin_family      = AF_INET;

    if ( connect(sock, (sockaddr*)&saddr, sizeof(sockaddr)) == SOCKET_ERROR )
    {
        cout<<"Błąd połączenia z serverem";
        sock = 0;
        return -1;
    }

    char pakiet[512]= {0};*/

    while(GetMessage(&msgs,0,0,0)) //pÃªtla obsÂ³ugujÂ¹ca wymianÃª komunikatÃ³w
    {
                                   TranslateMessage(&msgs); //funkcja tÂ³umaczÂ¹ca sygnaÂ³y z klawiatury na odpowiednie komunikaty systemowe
                                   DispatchMessage(&msgs); //funkcja przetwarzajÂ¹ca komunikaty systemowe przez procedury obsÂ³ugi
    }
    return msgs.wParam;


 //Przed wyslaniem i po odebraniu uzywamy pack i unpack
 // pack(sbuffer,pakiet);
 // sbuffer=unpack(pakiet);

  //Wyglad senda i recv
  //  send(sock,pakiet,sizeof(pakiet),0);
  //  recv(sock,pakiet,sizeof(pakiet),0);


  //  closesocket(sock);
  //  WSACleanup();
}

void zaloguj(int ID)
{
    switch(ID)
                {
                case 1: //jestes zalogowany
                        ShowWindow(stolyOkno,SW_SHOW);
                        ShowWindow(Okno,SW_HIDE);
                        UpdateWindow(stolyOkno);
                    break;

                case 2: //serwer full
                        MessageBox(0,"Serwer przepelniony, spróbuj póniej!","Ha!",MB_OK);
                    break;

                case 3: //bledne dane logowania
                        MessageBox(0,"Niepoprawne dane, spróbuj jeszcze raz!","Ha!",MB_OK);
                    break;

                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad!","Ha!",MB_OK);;
                }
}

void rejestruj(int ID)
{
    switch(ID)
                {
                case 1: //konto utworzone
                        MessageBox(0,"Konto utworzone!\nZostaniesz automatycznie zalogowany!","Ha!",MB_OK);
                        CHAR cLogin[20];
                        GetWindowText(hNickWpisz, cLogin, 20);
                        CHAR cPass[20];
                        GetWindowText(hPassWpisz, cPass, 20);
                        sklejChary(cLogin,cPass);//skleja login z haslem oddzielajac spacja
                        /*
                        przepiszChary(sbuffer.cChat, cLogin);
                        sbuffer.ID=2;
                        pack(sbuffer,pakiet);
                        send(sock,pakiet,sizeof(pakiet),0);
                        recv(sock,pakiet,sizeof(pakiet),0);
                        sbuffer=unpack(pakiet);
                        */
                        sbuffer.ID=1;//do testtow
                        zaloguj(sbuffer.ID);//automatyczne zalogowanie po utworzeniu konta
                    break;

                case 2: //konto istnieje
                        MessageBox(0,"Takie konto juz istnieje!","Ha!",MB_OK);
                    break;
                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad!","Ha!",MB_OK);;
                }
}
