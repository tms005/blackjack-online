#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolyWinMain();
extern HWND stolyOkno;

//HWND stolyhEdit;
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

HWND hStolChat;
HWND hStolMail;
HWND hStolWyslij;

HWND hWstan;
HWND hOpuscStolik;

HWND hListKart;

HWND hListGraczyOn;

HWND hStolWyloguj;

/////////////////////////////////////////////////////////deklaracje////////////////////////////////////////////
void wyswietlListe(HWND hLista, int iKey);//pobiera odpowiednia liste poprzez iKey i wkleja do odpowiedniego hwnd
void dodajEnter(char str1[]);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
   hInstMainANCE hInstMainance,             //uchwyt procesu do którego ma nale¿eæ nasze okno, podajemy tu uchwyt swojego programu
   LPVOID lpParam                   //wskaŸnik na dodatkowe informacje, które zostan¹ przekazane z komunikatem tworz¹cym okno
);

*/

LRESULT CALLBACK stolWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
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
             if((HWND)lPar==hStolWyslij)
             {
                CHAR cChat[256];
                GetWindowText(hStolMail, cChat, 256);
                dodajEnter(cChat);
                /*
                send();////////////////wysyla wiadomosc
                recv();////////////////odbiera wiadomosc z dopisanym loginem usera
                */
                SendMessage(hStolChat, EM_REPLACESEL, WPARAM(TRUE), LPARAM(cChat) );
                SetWindowText(hStolMail,"");
                SetFocus(hStolMail);
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
             break;
         }
         default:
         return DefWindowProc(hwnd,msg,wPar,lPar);       //domyœlna obs³uga reszty komunikatów
        }
        return 0;
}

int WINAPI stolWinMain ()
{
    //MSG stolmsgs;
    WNDCLASSEX stolwc;

    stolwc.hInstance = hInstMain;                                         //uchwyt programu
    stolwc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    stolwc.lpfnWndProc = stolWndProc;                                //
    stolwc.style = 0;                                                 //
    stolwc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    stolwc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolwc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    stolwc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    stolwc.lpszMenuName = "Menu_Window";                                          //nazwa menu
    stolwc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    stolwc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    stolwc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&stolwc)==0) return 0;
    stolOkno=CreateWindowEx(0,ClassName,"CzarnyJacek",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,550,Okno,0,hInstMain,0);
    //ShowWindow(dalej_Okno,SW_SHOW);
    //UpdateWindow(dalej_Okno);
    stol=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC",0,WS_CHILD|WS_VISIBLE,40,30,340,280,stolOkno,0,hInstMain,0);

    // gracz - krupiero
    hKrupier=CreateWindowEx(0,"STATIC","Krupier",WS_CHILD|WS_VISIBLE,20,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 40, 60, 190, stol, NULL, hInstMain, NULL);
    for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    /*SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");*/
    }

    //gracz1
    hGracz1=CreateWindowEx(0,"STATIC","Gracz 1",WS_CHILD|WS_VISIBLE,100,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 40, 60, 190, stol, NULL, hInstMain, NULL);
    for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    /*SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");*/
    }

    //gracz2
    hGracz2=CreateWindowEx(0,"STATIC","Gracz 2",WS_CHILD|WS_VISIBLE,180,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 180, 40, 60, 190, stol, NULL, hInstMain, NULL);
    for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    /*SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");*/
    }

    //gracz3
    hGracz3=CreateWindowEx(0,"STATIC","Gracz 3",WS_CHILD|WS_VISIBLE,260,20,50,20,stol,0,hInstMain,0);
    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 260, 40, 60, 190, stol, NULL, hInstMain, NULL);
    for(int i=0; i<11; i++)
    {
        SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta i-ta");
    /*SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");*/
    }

    hDobierz=CreateWindowEx(0,"BUTTON","dobierz",WS_CHILD|WS_VISIBLE,60,240,60,20,stol,0,hInstMain,0);
    hPodwoj=CreateWindowEx(0,"BUTTON","double",WS_CHILD|WS_VISIBLE,140,240,60,20,stol,0,hInstMain,0);
    hStoj=CreateWindowEx(0,"BUTTON","stop",WS_CHILD|WS_VISIBLE,220,240,60,20,stol,0,hInstMain,0);

    hListGraczyOn = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 405, 30, 147, 201, stolOkno, NULL, hInstMain, NULL);
    wyswietlListe(hListGraczyOn,4);

    hWstan=CreateWindowEx(0,"BUTTON","Wstan",WS_CHILD|WS_VISIBLE,442,242,75,20,stolOkno,0,hInstMain,0);
    hOpuscStolik=CreateWindowEx(0,"BUTTON","Opusc Stolik",WS_CHILD|WS_VISIBLE,430,272,100,20,stolOkno,0,hInstMain,0);

    hStolChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_DISABLED|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE,40,330,510,101,stolOkno,0,hInstMain,0); // 341
    hStolMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,441,261,20,stolOkno,0,hInstMain,0);
    hStolWyslij=CreateWindowEx(0,"BUTTON","Wy�lij",WS_CHILD|WS_VISIBLE,320,441,61,20,stolOkno,0,hInstMain,0);

    hStolWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,450,441,70,20,stolOkno,0,hInstMain,0);

    return 0;
}
