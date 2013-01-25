#include <windows.h>
#include <commctrl.h>

extern SOCKET sock;

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolyWinMain();
extern HWND stolyOkno;

HWND stolOkno;
CONST CHAR ClassName[]="StolExample";
CONST CHAR MenuName[]="Menu_Window";

HWND stol;

HWND hKrupier;
HWND hGracz1;
HWND hGracz2;
HWND hGracz3;

HWND hDobierz;
HWND hPodwoj;
HWND hStoj;

extern HWND hStolChat;
HWND hStolMail;
HWND hStolWyslij;

HWND hWstan;
HWND hOpuscStolik;

HWND hListKart;

extern HWND hListGraczyOn;

HWND hStolWyloguj;

struct Buffer{
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

Buffer sbufferStol;///////////obsluga naszej ramki dla okna Stoly
char pakietStol[512]= {0};

/////////////////////////////////////////////////////////deklaracje////////////////////////////////////////////
void dodajEnter(char str1[]);
void sklejChary(char str1[], char str2[]);
void pack(Buffer buff, char (&ref)[512]);
void PrintCard(int iCard, char cKarta[]);//dla kompilacji xd
Buffer unpack(char ref[512]);//to tez wyjebac jak bedzie glowny switch
////////////////////////////////////////////////////////////////////
void pobierzListyStolyOkno();
void pobierzListyStol();
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK stolWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         case WM_CLOSE:
         {
            sbufferStol.ID=6;
            pack(sbufferStol,pakietStol);
            send(sock,pakietStol,sizeof(pakietStol),0);
            PostQuitMessage(0);
            break;
         }
         case WM_COMMAND:
         {
             if((HWND)lPar==hDobierz)
             {
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=1;
                pack(sbufferStol,pakietStol);
                send(sock,pakietStol,sizeof(pakietStol),0);
                /*recv(sock,pakietStol,sizeof(pakietStol),0);
                sbufferStol=unpack(pakietStol);sbufferStol.iKey[0]=24;sbufferStol.iKey[1]=24;
                if(sbufferStol.iKey[0]!=0)
                {
                char cKarta[4]={0};
                PrintCard(sbufferStol.iKey[1], cKarta);

                SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)cKarta);
                }
                */
            }

             else if((HWND)lPar==hPodwoj)
             {
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=2;
                pack(sbufferStol,pakietStol);
                send(sock,pakietStol,sizeof(pakietStol),0);
                /*recv(sock,pakietStol,sizeof(pakietStol),0);
                sbufferStol=unpack(pakietStol);
                if(sbufferStol.iKey[0]!=0)
                {
                char cKarta[4]={0};
                PrintCard(sbufferStol.iKey[1], cKarta);

                SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)cKarta);
                }
                */
             }
             else if((HWND)lPar==hStoj)
             {
                sbufferStol.ID=8;
                sbufferStol.iKey[0]=0;
                pack(sbufferStol,pakietStol);
                send(sock,pakietStol,sizeof(pakietStol),0);
             }
             else if((HWND)lPar==hOpuscStolik)
             {
                    sbufferStol.ID=5;
                    pack(sbufferStol,pakietStol);
                    send(sock,pakietStol,sizeof(pakietStol),0);
                    pobierzListyStolyOkno();
                    ShowWindow(stolyOkno,SW_SHOW);
                    ShowWindow(stolOkno,SW_HIDE);
                    UpdateWindow(stolyOkno);
             }
             else if((HWND)lPar==hStolWyloguj)
             {
                    sbufferStol.ID=2;
                    pack(sbufferStol,pakietStol);
                    send(sock,pakietStol,sizeof(pakietStol),0);
                    ShowWindow(Okno,SW_SHOW);
                    ShowWindow(stolOkno,SW_HIDE);
                    UpdateWindow(Okno);
             }
             else if((HWND)lPar==hStolWyslij)
             {
                GetWindowText(hStolMail, sbufferStol.cChat, 256);
                dodajEnter(sbufferStol.cChat);
                sbufferStol.ID=0;
                pack(sbufferStol,pakietStol);
                send(sock,pakietStol,sizeof(pakietStol),0);
                SetWindowText(hStolMail,"");
                SetFocus(hStolMail);
             }
             if(wPar==10)
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
             else if(wPar==13)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 //ShowWindow(stolOkno,SW_HIDE);
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

    if(RegisterClassEx(&stolwc)==0) return 0;
    stolOkno=CreateWindowEx(0,ClassName,"BlackJack",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,550,Okno,0,hInstMain,0);

    stol=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC",0,WS_CHILD|WS_VISIBLE,40,30,340,280,stolOkno,0,hInstMain,0);

    // gracz - krupiero
    hKrupier=CreateWindowEx(0,"STATIC","Krupier",WS_CHILD|WS_VISIBLE,20,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 40, 60, 190, stol, NULL, hInstMain, NULL);
    /*for(int i=0; i<11; i++)
    {
        // funkcja do wyswietlania kart
    }
*/
    //gracz1
    hGracz1=CreateWindowEx(0,"STATIC","Gracz 1",WS_CHILD|WS_VISIBLE,100,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 40, 60, 190, stol, NULL, hInstMain, NULL);
   /* for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");
    }
*/
    //gracz2
    hGracz2=CreateWindowEx(0,"STATIC","Gracz 2",WS_CHILD|WS_VISIBLE,180,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 180, 40, 60, 190, stol, NULL, hInstMain, NULL);
   /* for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");
    }
*/
    //gracz3
    hGracz3=CreateWindowEx(0,"STATIC","Gracz 3",WS_CHILD|WS_VISIBLE,260,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 260, 40, 60, 190, stol, NULL, hInstMain, NULL);
   /* for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");
    }
*/
    hDobierz=CreateWindowEx(0,"BUTTON","dobierz",WS_CHILD|WS_VISIBLE,60,240,60,20,stol,0,hInstMain,0);
    hPodwoj=CreateWindowEx(0,"BUTTON","double",WS_CHILD|WS_VISIBLE,140,240,60,20,stol,0,hInstMain,0);
    hStoj=CreateWindowEx(0,"BUTTON","stop",WS_CHILD|WS_VISIBLE,220,240,60,20,stol,0,hInstMain,0);

    hListGraczyOn = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 405, 30, 147, 241, stolOkno, NULL, hInstMain, NULL);

    hOpuscStolik=CreateWindowEx(0,"BUTTON","Opusc Stolik",WS_CHILD|WS_VISIBLE,430,272,100,20,stolOkno,0,hInstMain,0);

    hStolChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_DISABLED|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,40,330,510,101,stolOkno,0,hInstMain,0);
    hStolMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,441,261,20,stolOkno,0,hInstMain,0);
    hStolWyslij=CreateWindowEx(0,"BUTTON","Wylij",WS_CHILD|WS_VISIBLE,320,441,61,20,stolOkno,0,hInstMain,0);

    hStolWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,450,441,70,20,stolOkno,0,hInstMain,0);

    return 0;
}
