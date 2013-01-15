#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolyWinMain();
extern HWND stolyOkno;

extern int stolWinMain();
extern HWND stolOkno;

extern int rejestrWinMain();
extern HWND rejestrOkno;

//HWND rankhEdit;
HWND rankOkno;
CONST CHAR ClassName[]="Ranking";
CONST CHAR MenuName[]="Menu_Window";

HWND hListRank;

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

LRESULT CALLBACK rankWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
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

int WINAPI rankWinMain ()
{
    //MSG rankmsgs;
    WNDCLASSEX rankwc;

    rankwc.hInstance = hInstMain;                                     //uchwyt programu
    rankwc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    rankwc.lpfnWndProc = rankWndProc;                                 //
    rankwc.style = 0;                                                 //
    rankwc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    rankwc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    rankwc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    rankwc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    rankwc.lpszMenuName = "Menu_Window";                              //nazwa menu
    rankwc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    rankwc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    rankwc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&rankwc)==0) return 0;
    rankOkno=CreateWindowEx(0,ClassName,"BlackJack",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);
    //ShowWindow(dalej_Okno,SW_SHOW);
    //UpdateWindow(dalej_Okno);

    hListRank= CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 40, 30, 500, 300, rankOkno, NULL, hInstMain, NULL);
    SendMessage(hListRank, LB_ADDSTRING, 0, (LPARAM)"rank 1");
    SendMessage(hListRank, LB_ADDSTRING, 0, (LPARAM)"rank 2");
    SendMessage(hListRank, LB_ADDSTRING, 0, (LPARAM)"rank 3");
    SendMessage(hListRank, LB_ADDSTRING, 0, (LPARAM)"rank 4");
    //ShowWindow(rankOkno,SW_SHOW);

    return 0;
}

