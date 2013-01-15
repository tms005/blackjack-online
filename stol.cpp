#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int rankWinMain();
extern HWND rankOkno;

extern int stolyWinMain();
extern HWND stolyOkno;

extern int rejestrWinMain();
extern HWND rejestrOkno;

//HWND stolyhEdit;
HWND stolOkno;
CONST CHAR ClassName[]="StolExample";
CONST CHAR MenuName[]="Menu_Window";

HWND stol;

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
             if(wPar==10)
             {
                 //ShowWindow(Okno,SW_HIDE);
                 ShowWindow(rankOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(rejestrOkno,SW_HIDE);
                 ShowWindow(Okno,SW_SHOW);
             }
             else if(wPar==11)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(rankOkno,SW_HIDE);
                 //ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(rejestrOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_SHOW);
             }
             else if(wPar==12)
             {
                 ShowWindow(Okno,SW_HIDE);
                 //ShowWindow(rankOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(rejestrOkno,SW_HIDE);
                 ShowWindow(rankOkno,SW_SHOW);
             }
             else if(wPar==13)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(rankOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 //ShowWindow(stolOkno,SW_HIDE);
                 ShowWindow(rejestrOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_SHOW);
             }
             else if(wPar==14)
             {
                 ShowWindow(Okno,SW_HIDE);
                 ShowWindow(rankOkno,SW_HIDE);
                 ShowWindow(stolyOkno,SW_HIDE);
                 ShowWindow(stolOkno,SW_HIDE);
                 //ShowWindow(rejestrOkno,SW_HIDE);
                 ShowWindow(rejestrOkno,SW_SHOW);
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
    stolOkno=CreateWindowEx(0,ClassName,"BlackJack",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);
    //ShowWindow(dalej_Okno,SW_SHOW);
    //UpdateWindow(dalej_Okno);
    stol=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC",0,WS_CHILD|WS_VISIBLE,40,30,340,170,stolOkno,0,hInstMain,0);

    hDobierz=CreateWindowEx(0,"BUTTON","dobierz",WS_CHILD|WS_VISIBLE,22,15,40,20,stol,0,hInstMain,0);
    hPodwoj=CreateWindowEx(0,"BUTTON","double",WS_CHILD|WS_VISIBLE,22,40,40,20,stol,0,hInstMain,0);
    hStoj=CreateWindowEx(0,"BUTTON","stop",WS_CHILD|WS_VISIBLE,22,65,40,20,stol,0,hInstMain,0);

    hListKart= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 155, 22, 60, 100, stol, NULL, hInstMain, NULL);
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 1");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 2");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 3");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 4");
    SendMessage(hListKart, LB_ADDSTRING, 0, (LPARAM)"karta 5");

    hWstan=CreateWindowEx(0,"BUTTON","Wstan",WS_CHILD|WS_VISIBLE,100,210,75,20,stolOkno,0,hInstMain,0);
    hOpuscStolik=CreateWindowEx(0,"BUTTON","Opusc Stolik",WS_CHILD|WS_VISIBLE,220,210,100,20,stolOkno,0,hInstMain,0);

    SendMessage(hListGraczyOn, LB_ADDSTRING, 0, (LPARAM)"GraczOn 1");
    SendMessage(hListGraczyOn, LB_ADDSTRING, 0, (LPARAM)"GraczOn 2");
    SendMessage(hListGraczyOn, LB_ADDSTRING, 0, (LPARAM)"GraczOn 3");

    hStolChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,240,341,101,stolOkno,0,hInstMain,0);
    hStolMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,355,261,20,stolOkno,0,hInstMain,0);
    hStolWyslij=CreateWindowEx(0,"BUTTON","Wy�lij",WS_CHILD|WS_VISIBLE,320,355,61,20,stolOkno,0,hInstMain,0);

    hListGraczyOn = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 405, 30, 147, 321, stolOkno, NULL, hInstMain, NULL);

    hStolWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,450,355,70,20,stolOkno,0,hInstMain,0);

    return 0;
}
