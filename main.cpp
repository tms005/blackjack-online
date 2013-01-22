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

Buffer sbuffer;
Buffer klient;
Stol pierwszy, drugi, trzeci, czwarty;

char pakiet[512] = {0};
char cKartaStol[256] = {0};
int klientsrodki=0,klientstol=0,j=0,i=0,dg=0,dn=0,k=0;

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
         case WM_DESTROY:
         {
            closesocket(sock);
            WSACleanup();
            sbuffer.ID=6;
            pack(sbuffer,pakiet);
            send(sock,pakiet,sizeof(pakiet),0);
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
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=2;
                pack(sbuffer,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
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
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=1;
                pack(sbuffer,pakiet);
                send(sock,pakiet,sizeof(pakiet),0);
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
    wc.lpszMenuName = "Menu_Window";                                          //nazwa menu
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

    int RetVal = 0;
        WSAData wsaData;
        WORD DllVersion = MAKEWORD(2,1);
        RetVal = WSAStartup(DllVersion, &wsaData);
        if(RetVal != 0)
        {
                MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
                exit(1);
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        saddr.sin_port        = htons(1234);
        saddr.sin_family      = AF_INET;

    if ( connect(sock, (sockaddr*)&saddr, sizeof(sockaddr)) == SOCKET_ERROR )
    {
        MessageBox(0,"Blad polaczenia z serwem","Ha!",MB_OK);
        sock = 0;
        return -1;
    }

    while(GetMessage(&msgs,0,0,0)) //pÃªtla obsÂ³ugujÂ¹ca wymianÃª komunikatÃ³w
    {
        TranslateMessage(&msgs); //funkcja tÂ³umaczÂ¹ca sygnaÂ³y z klawiatury na odpowiednie komunikaty systemowe
        DispatchMessage(&msgs); //funkcja przetwarzajÂ¹ca komunikaty systemowe przez procedury obsÂ³ugi

        if(recv(sock,pakiet,sizeof(pakiet),0))
            {
		    sbuffer=unpack(pakiet);
		    switch(sbuffer.ID)
                {
                case 0: //WIADOMOSC CZATU:
                        switch(sbuffer.iKey[0])
                            {
                            case 1: //globalny
                                    SendMessage(hStolyChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );
                                break;
                            case 2: //lokalny
                                    SendMessage(hStolChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );
                                break;
                            default:
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 0","Ha!",MB_OK);
                            }
                    break;

                case 1: //UTWORZENIE KONTA:
                        if(sbuffer.ID_USR==0)
                        {
                            MessageBox(0,"Takie konto juz istnieje!","Upsss!",MB_OK);
                            EnableWindow(hLogin, true);
                            EnableWindow(hRejestr, true);
                        }
                        else
                        {
                            klient.ID_USR=sbuffer.ID_USR;
                            MessageBox(0,"Konto utworzone!\nMozesz sie zalogowac!","Ha!",MB_OK);
                            EnableWindow(hLogin, true);
                            EnableWindow(hRejestr, true);
                        }
                    break;

                case 2: //LOGOWANIE/WYLOGOWANIE:
                        switch(sbuffer.iKey[0])
                            {
                            case 1: //jestes zalogowany
                                    klient.ID_USR=sbuffer.iKey[1];
                                    klientsrodki=sbuffer.iKey[2];
                                    pobierzListyStolyOkno();
                                    ShowWindow(stolyOkno,SW_SHOW);
                                    ShowWindow(Okno,SW_HIDE);
                                    UpdateWindow(stolyOkno);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            case 2: //serwer full
                                    MessageBox(0,"Serwer przepelniony, spróbuj póniej!","Ha!",MB_OK);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            case 3: //bledne dane logowania
                                    MessageBox(0,"Niepoprawne dane, spróbuj jeszcze raz!","Ha!",MB_OK);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            default:
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 2","Ha!",MB_OK);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                            }
                    break;

                case 3: //INFORMACJE:
                        switch(sbuffer.iKey[0])
                            {
                            case 0: //stolow
                                    char cWyraz[20];
                                    char cNazwaGracza[12][20];
                                    memset(cWyraz, 0, 256);
                                    memset(cNazwaGracza, 0, 256);
                                    dn=0,dg=0;
                                    for(unsigned int d = 0;d<strlen(sbuffer.cChat);d++)
                                        {
                                            if(sbuffer.cChat[d]!=' ')
                                                {
                                                    cWyraz[dn]=sbuffer.cChat[d];
                                                    dn++;
                                                }
                                            else
                                                {
                                                    przepiszChary(cNazwaGracza[dg],cWyraz);
                                                    memset(cWyraz, 0, 256);
                                                    dn=0;
                                                    dg++;
                                                }
                                        }

                                    k=0;
                                    j=0;
                                    for(i=1;i<4;i++)
                                        {
                                            if(sbuffer.iKey[i]!=0)
                                            {
                                                pierwszy.iIdGraczy[j]=sbuffer.iKey[i];
                                                przepiszChary(pierwszy.cNazwyGraczy[j],cNazwaGracza[k]);
                                                if(klient.ID_USR==pierwszy.iIdGraczy[j]) klientstol=1;
                                                k++;
                                            }
                                        j++;
                                        }
                                    j=0;
                                    for(i=4;i<7;i++)
                                        {
                                            if(sbuffer.iKey[i]!=0)
                                            {
                                                drugi.iIdGraczy[j]=sbuffer.iKey[i];
                                                przepiszChary(drugi.cNazwyGraczy[j],cNazwaGracza[k]);
                                                if(klient.ID_USR==drugi.iIdGraczy[j]) klientstol=2;
                                                k++;
                                            }
                                        j++;
                                        }
                                    j=0;
                                    for(i=7;i<10;i++)
                                        {
                                            if(sbuffer.iKey[i]!=0)
                                            {
                                                trzeci.iIdGraczy[j]=sbuffer.iKey[i];
                                                przepiszChary(trzeci.cNazwyGraczy[j],cNazwaGracza[k]);
                                                if(klient.ID_USR==trzeci.iIdGraczy[j]) klientstol=3;
                                                k++;
                                            }
                                        j++;
                                        }
                                    j=0;
                                    for(i=10;i<13;i++)
                                        {
                                            if(sbuffer.iKey[i]!=0)
                                            {
                                                czwarty.iIdGraczy[j]=sbuffer.iKey[i];
                                                przepiszChary(czwarty.cNazwyGraczy[j],cNazwaGracza[k]);
                                                if(klient.ID_USR==czwarty.iIdGraczy[j]) klientstol=4;
                                                k++;
                                            }
                                        j++;
                                        }
                                    switch(klientstol)
                                            {
                                            case 1:
                                                    wyswietlListeGraczyOn(hListGraczyOn,pierwszy);
                                                break;
                                            case 2:
                                                    wyswietlListeGraczyOn(hListGraczyOn,drugi);
                                                break;
                                            case 3:
                                                    wyswietlListeGraczyOn(hListGraczyOn,trzeci);
                                                break;
                                            case 4:
                                                    wyswietlListeGraczyOn(hListGraczyOn,czwarty);
                                                break;
                                            }
                                break;
                            case 1: //graczy z rankingu
                                    wyswietlListe(hListRanking,sbuffer);
                                    memset(sbuffer.cChat,0,256);
                                    j=0;
                                    for(i=1;i<=10;i++)
                                    {
                                        sbuffer.cChat[j]=sbuffer.iKey[i];
                                        sbuffer.cChat[j+1]=' ';
                                        j=j+2;
                                    }
                                    wyswietlListe(hListRankingPkt,sbuffer);
                                break;
                            case 2: //nickow graczy online
                                    wyswietlListe(hListGraczy,sbuffer);
                                break;
                            default:
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 3","Ha!",MB_OK);
                            }
                    break;

                case 4: //PRZYLACZENIE SIE DO STOLU:
                        if(sbuffer.ID_USR==0)
                        {
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                            ShowWindow(stolOkno,SW_SHOW);
                            ShowWindow(stolyOkno,SW_HIDE);
                            UpdateWindow(stolOkno);
                        }
                        else
                        {
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                        }
                    break;

                case 5: //OPUSZCZENIE DANEGO STOLU
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                            ShowWindow(stolyOkno,SW_SHOW);
                            ShowWindow(stolOkno,SW_HIDE);
                            UpdateWindow(stolyOkno);
                    break;

                case 6: //OPUSZCZENIE GRY:
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                    break;
/*
                case 7: //gramy
                        switch(sbuffer.iKey[0])
                            {
                            case 0: //ktos spasowal - co odbieramy? wynik czy co?
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 8","Ha!",MB_OK); //for testing
                                break;
                            case 1: //ktos dobral - wartosc karty w iKey[1]
                                    PrintCard(sbuffer.iKey[1],cKartaStol);//trzeba dopisac komu ma przydzielac tylko jak jak nie ma polaczenia nicki-id_usr
                                break;
                            case 2: //ktos podbil stawke (sa stawki? o_O) dobieramy tez karte automatycznie
                                    PrintCard(sbuffer.iKey[1],cKartaStol);//trzeba dopisac komu ma przydzielac tylko jak jak nie ma polaczenia nicki-id_usr
                                break;
                            default:
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 8","Ha!",MB_OK);
                            }
                    break;
*/
                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad! caly","Ha!",MB_OK);
                }
            }
    }
    return msgs.wParam;
}
