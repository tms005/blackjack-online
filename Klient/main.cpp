#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "funkcje.h"

using namespace std;
//Okna
extern int stolyWinMain();
extern HWND stolyOkno;
extern int stolWinMain();
extern HWND stolOkno;
CONST CHAR ClassName[]="GameWindow";
CONST CHAR MenuName[]="Menu_Window";
MSG msgs; //struktura na komunikatÄ‚ÂĂ‚Ĺ‚w
HINSTANCE hInstMain;
HWND Okno;
//Zmienne danych
Buffer sbuffer;
Buffer klient;
Stol pierwszy, drugi, trzeci, czwarty;
char cKartaStol[256];
char cNazwaGracza[256];
int klientsrodki=0,klientstol=0;
//Zmienne pomocnicze
int hold=0,j=0,i=0;
int dg=0,dn=0,k=0;
//Pola tekstowe
HWND hNickWpisz;
HWND hPassWpisz;
//Buttony
HWND hLogin;
HWND hRejestr;
//Etykiety
HWND hLogo;
HWND hNick;
HWND hPass;

DWORD WINAPI NetThread(LPVOID ctx)
{
    SOCKET* sockThread=(SOCKET*)ctx;
    int     ret;

    // Receive data
    for(;;)
    {
    memset(pakiet,0,512);
    ret = recv(*sockThread, pakiet, 512, 0);
    if (ret == SOCKET_ERROR)
    {
       MessageBox(0, "recv failed", "Error", 0);
    }
    else
            {
		    unpack(&sbuffer,pakiet);
		    switch(sbuffer.ID)
                {
                case 0: //WIADOMOSC CZATU:
                        int x;
                        x=0;
                        if(sbuffer.ID_USR==klient.ID_USR)
                        {
                            SendMessage(hStolyChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );//bo po co rozkminiac gdzie wyswietlic xd
                            SendMessage(hStolChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );
                        }
                            else
                            {
                                for(i=0;i<3;i++)
                                    if(sbuffer.ID_USR==pierwszy.iIdGraczy[i]) x=1;
                                for(i=0;i<3;i++)
                                    if(sbuffer.ID_USR==drugi.iIdGraczy[i]) x=2;
                                for(i=0;i<3;i++)
                                    if(sbuffer.ID_USR==trzeci.iIdGraczy[i]) x=3;
                                for(i=0;i<3;i++)
                                    if(sbuffer.ID_USR==czwarty.iIdGraczy[i]) x=4;

                                if(x==0)
                                    SendMessage(hStolyChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );
                                    else if(klientstol==x) SendMessage(hStolChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(sbuffer.cChat) );
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
                                    pobierzListyStol();
                                    ShowWindow(stolyOkno,SW_SHOW);
                                    ShowWindow(Okno,SW_HIDE);
                                    UpdateWindow(stolyOkno);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            case 2: //serwer full
                                    MessageBox(0,"Serwer przepelniony, sprĂłbuj pĂłÂźniej!","Ha!",MB_OK);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            case 3: //bledne dane logowania
                                    MessageBox(0,"Niepoprawne dane, sprĂłbuj jeszcze raz!","Ha!",MB_OK);
                                    EnableWindow(hLogin, true);
                                    EnableWindow(hRejestr, true);
                                break;
                            default:
                                    MessageBox(0,"Wylogowano!","Ha!",MB_OK);
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
                            //zczytac do jakiego stolu dolaczylem
                            //klientstol=;
                        }
                        else
                        {
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                        }
                    break;

                case 5: //OPUSZCZENIE DANEGO STOLU
                        if(sbuffer.ID_USR==0)
                        {
                            pobierzListyStolyOkno();//dostajemy odswiezone listy
                            pobierzListyStol();
                            ShowWindow(stolyOkno,SW_SHOW);
                            ShowWindow(stolOkno,SW_HIDE);
                            UpdateWindow(stolyOkno);
                            klientstol=0;
                        }
                        else
                        {
                            pobierzListyStolyOkno();
                            pobierzListyStol();
                        }
                    break;

                case 6: //OPUSZCZENIE GRY:
                            pobierzListyStolyOkno();//gracze online dostaja odswiezona liste ziomkow
                            pobierzListyStol();
                    break;

                case 7: //GRA:
                        switch(sbuffer.iKey[0])
                            {
                            case 0: //ktos spasowal patrz nizej
                                    MessageBox(0,"Gracz o id bla bla spasowal nie wiem czy bedziem wyswietlac co i gdzie moze cos sie zrobi w dlc! case 8","Ha!",MB_OK); //for testing
                                break;
                            case 1: //ktos dobral - wartosc karty w iKey[1]
                                    switch(klientstol)//w zaleznosci przy ktorym stole siedzimy to zeby wiedzial wsrod ktorych ludzi go szukac
                                        {
                                        case 1: //pierwszy stol
                                                if(sbuffer.ID_USR==pierwszy.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==pierwszy.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==pierwszy.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 2: //drugi stol
                                                if(sbuffer.ID_USR==drugi.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==drugi.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==drugi.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 3: //trzeci stol
                                                if(sbuffer.ID_USR==trzeci.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==trzeci.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==trzeci.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 4: //czwarty stol
                                                if(sbuffer.ID_USR==czwarty.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==czwarty.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==czwarty.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        }
                                break;
                            case 2: //ktos podbil stawke, dobieramy tez karte automatycznie// nie wiem czy zrobimy wyswietlanie stawek wiec narazie pomijam
                                    switch(klientstol)//w zaleznosci przy ktorym stole siedzimy to zeby wiedzial wsrod ktorych ludzi go szukac
                                        {
                                        case 1: //pierwszy stol
                                                if(sbuffer.ID_USR==pierwszy.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==pierwszy.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==pierwszy.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 2: //drugi stol
                                                if(sbuffer.ID_USR==drugi.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==drugi.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==drugi.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 3: //trzeci stol
                                                if(sbuffer.ID_USR==trzeci.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==trzeci.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==trzeci.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        case 4: //czwarty stol
                                                if(sbuffer.ID_USR==czwarty.iIdGraczy[0])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz1, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==czwarty.iIdGraczy[1])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz2, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                                else if(sbuffer.ID_USR==czwarty.iIdGraczy[2])
                                                {
                                                    PrintCard(sbuffer.iKey[1],cKartaStol);
                                                    SendMessage(hListKartGracz3, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cKartaStol) );
                                                }
                                            break;
                                        }
                                break;
                            case 3: //koniec rozgrywki
                                    //pobranie kart krupiera czyli bedzie send
                                    if(sbuffer.iKey[2]==1)
                                    MessageBox(0,"Wygrales! case 7","Ha!",MB_OK);//jezeli bedzie potrzeba to sie zrobi w gui miejsce i licznik na wygrane i przegrane/prowizorka
                                    else MessageBox(0,"Lo0oser! case 7","Ha!",MB_OK);
                                break;
                            default:
                                    MessageBox(0,"Wystapil nieoczekiwany blad! case 8","Ha!",MB_OK);
                            }
                    break;

                case 8: //TRANSFER DANYCH O KARTACH GRACZY/KRUPIERA:
                            MessageBox(0,"do zrobienia! case 8","Ha!",MB_OK);//do zrobinia
                    break;

                case 9: //INFORMACJA O TYM, ZE SERWER OCZEKUJE NA PODJECIE DECYZJI WZG KOLEJNEJ TURY GRY:
                            MessageBox(0,"Rusz dupe! case 9","Ha!",MB_OK);//Serwer wysyla ta informacje do klienta, kiedy rozpoczyna sie jego runda.
                    break;

                case 10: //TOKEN
                        if(kolejka.empty())
                        {
                            czyscBuffer(sbuffer);
                            sbuffer.ID=10;
                            pack(&sbuffer,pakiet);
                            send(sock,pakiet,512,0);
                        }
                        else
                        {
                            send(sock,kolejka.front(),512,0);
                            kolejka.pop();
                        }
                    break;

                default:
                        MessageBox(0,"Wystapil nieoczekiwany blad! caly","Ha!",MB_OK);
                }
            }
    }

    closesocket(sock);
}


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
            czyscBuffer(sbuffer);
            sbuffer.ID=6;
            pack(&sbuffer,pakiet);
            kolejka.push(pakiet);
            closesocket(sock);
            WSACleanup();
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hLogin)
             {
                czyscBuffer(sbuffer);
                EnableWindow(hLogin, false);
                EnableWindow(hRejestr, false);
                CHAR cLogin[20];
                GetWindowText(hNickWpisz, cLogin, 20);
                CHAR cPass[20];
                GetWindowText(hPassWpisz, cPass, 20);
                sklejChary(cLogin,cPass);//skleja login z haslem oddzielajac spacja
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=2;
                pack(&sbuffer,pakiet);
                kolejka.push(pakiet);
             }
             else if((HWND)lPar==hRejestr)
             {
                czyscBuffer(sbuffer);
                EnableWindow(hLogin, false);
                EnableWindow(hRejestr, false);
                CHAR cLogin[20];
                GetWindowText(hNickWpisz, cLogin, 20);
                CHAR cPass[20];
                GetWindowText(hPassWpisz, cPass, 20);
                sklejChary(cLogin,cPass);//skleja login z haslem oddzielajac spacja
                przepiszChary(sbuffer.cChat, cLogin);
                sbuffer.ID=1;
                pack(&sbuffer,pakiet);
                kolejka.push(pakiet);
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
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyÄ‚â€¦Ă‚â€ślna obsÄ‚â€šĂ‚Ĺ‚uga reszty komunikatÄ‚ÂĂ‚Ĺ‚w
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
    wc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiÄ‚ÂĂ‚Ĺžci
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt maÄ‚â€šĂ‚Ĺ‚ej ikony okna
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - sÄ‚â€šĂ‚Ĺ‚uÄ‚â€šĂ‚ĹĽy do zaÄ‚â€šĂ‚Ĺ‚adowania kursora tzw. "strzaÄ‚â€šĂ‚Ĺ‚ki"
    wc.lpszMenuName = "Menu_Window";                                          //nazwa menu
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pÄ‚ÂĂ‚Ĺždzla" z tÄ‚â€šĂ‚Ĺ‚em
    wc.cbClsExtra = 0;                                            //dodatkowa pamiÄ‚ÂĂ‚ĹžÄ‚ÂĂ‚Â¦ dla okna klasy
    wc.cbWndExtra = 0;                                            //dodatkowa pamiÄ‚ÂĂ‚ĹžÄ‚ÂĂ‚Â¦ dla okna utworzona z tej klasy

    if(RegisterClassEx(&wc)==0) return 0;
    Okno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_VISIBLE,80,50,600,500,0,0,hInst,0);

    hLogo=CreateWindowEx(0,"STATIC","Logowanie",WS_CHILD|WS_VISIBLE,280,60,150,20,Okno,0,hInst,0);
    hNick=CreateWindowEx(0,"STATIC","Login: ",WS_CHILD|WS_VISIBLE,160,110,150,20,Okno,0,hInst,0);
    hNickWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,220,110,200,20,Okno,0,hInst,0);

    hPass=CreateWindowEx(0,"STATIC","Haslo:  ",WS_CHILD|WS_VISIBLE,160,170,50,20,Okno,0,hInst,0);
    hPassWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_PASSWORD,220,170,200,20,Okno,0,hInst,0);

    hLogin=CreateWindowEx(0,"BUTTON","Logowanie",WS_CHILD|WS_VISIBLE,265,230,100,20,Okno,0,hInst,0);
    hRejestr = CreateWindowEx(0,"BUTTON","Rejestracja",WS_VISIBLE|WS_CHILD,265,270,100,20,Okno,0,hInst,0);

    WSADATA       wsd;
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        MessageBox(0, "Can't load WinSock", "Error", 0);
        return 0;
    }

    struct sockaddr_in server;
    struct hostent    *host = NULL;
    char  szServerName[1024];

    strcpy(szServerName, "127.0.0.1");

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
       MessageBox(0, "Can't create socket", "Error", 0);
       return 1;
    }
    // Fill the structure with the server address and
    // the port number
    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
    server.sin_addr.s_addr = inet_addr(szServerName);

    // If a name is specified, convert the symbolic server
    // address to an IP address
    if (server.sin_addr.s_addr == INADDR_NONE)
    {
        host = gethostbyname(szServerName);
        if (host == NULL)
        {
           MessageBox(0, "Unable to resolve server", "Error", 0);
           return 1;
        }
        CopyMemory(&server.sin_addr, host->h_addr_list[0],
            host->h_length);
    }
// Connect to the server
    if (connect(sock, (struct sockaddr *)&server,
        sizeof(server)) == SOCKET_ERROR)
    {
        MessageBox(0, "connect failed", "Error", 0);
        return 1;
    }
// Tworzenie watku klienta
    HANDLE        hNetThread;
    DWORD         dwNetThreadId;
    hNetThread = CreateThread(NULL, 0, NetThread,
                    (LPVOID)&sock, 0, &dwNetThreadId);

    while(GetMessage(&msgs,0,0,0)) //pÄ‚ÂĂ‚Ĺžtla obsÄ‚â€šĂ‚Ĺ‚ugujÄ‚â€šĂ‚Ä…ca wymianÄ‚ÂĂ‚Ĺž komunikatÄ‚ÂĂ‚Ĺ‚w
    {
        TranslateMessage(&msgs); //funkcja tÄ‚â€šĂ‚Ĺ‚umaczÄ‚â€šĂ‚Ä…ca sygnaÄ‚â€šĂ‚Ĺ‚y z klawiatury na odpowiednie komunikaty systemowe
        DispatchMessage(&msgs); //funkcja przetwarzajÄ‚â€šĂ‚Ä…ca komunikaty systemowe przez procedury obsÄ‚â€šĂ‚Ĺ‚ugi
    }
    return msgs.wParam;
}
