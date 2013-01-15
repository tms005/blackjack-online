#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int rankWinMain();
extern HWND rankOkno;

extern int stolWinMain();
extern HWND stolOkno;

extern int rejestrWinMain();
extern HWND rejestrOkno;

//HWND stolyhEdit;
HWND stolyOkno;
CONST CHAR ClassName[]="Lista sto��w";
CONST CHAR MenuName[]="Menu_Window";

HWND hListStoly;
HWND hListGraczy;

HWND hDolacz;
HWND hUtworz;

HWND hStolyChat;
HWND hStolyMail;
HWND hStolyWyslij;

HWND hStolyWyloguj;

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
   hInstMainANCE hInstMainance,     //uchwyt procesu do którego ma nale¿eæ nasze okno, podajemy tu uchwyt swojego programu
   LPVOID lpParam                   //wskaŸnik na dodatkowe informacje, które zostan¹ przekazane z komunikatem tworz¹cym okno
);

*/

LRESULT CALLBACK stolyWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
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

int WINAPI stolyWinMain ()
{
    //MSG stolymsgs;
    WNDCLASSEX stolywc;

    stolywc.hInstance = hInstMain;                                     //uchwyt programu
    stolywc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    stolywc.lpfnWndProc = stolyWndProc;                                //
    stolywc.style = 0;                                                 //
    stolywc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    stolywc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    stolywc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    stolywc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    stolywc.lpszMenuName = "Menu_Window";                              //nazwa menu
    stolywc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    stolywc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    stolywc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&stolywc)==0) return 0;
    stolyOkno=CreateWindowEx(0,ClassName,"BlackJack",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,50,50,600,500,Okno,0,hInstMain,0);
    //ShowWindow(dalej_Okno,SW_SHOW);
    //UpdateWindow(dalej_Okno);

    hListStoly = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 40, 30, 341, 180, stolyOkno, NULL, hInstMain, NULL);
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 1");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 2");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 3");
    SendMessage(hListStoly, LB_ADDSTRING, 0, (LPARAM)"St� 4");

    hDolacz=CreateWindowEx(0,"BUTTON","Do��cz",WS_CHILD|WS_VISIBLE,50,205,75,23,stolyOkno,0,hInstMain,0);
    hUtworz=CreateWindowEx(0,"BUTTON","Utw�rz",WS_CHILD|WS_VISIBLE,140,205,75,23,stolyOkno,0,hInstMain,0);

    hStolyChat=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,240,341,101,stolyOkno,0,hInstMain,0);
    hStolyMail=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,40,355,261,20,stolyOkno,0,hInstMain,0);
    hStolyWyslij=CreateWindowEx(0,"BUTTON","Wy�lij",WS_CHILD|WS_VISIBLE,320,355,61,20,stolyOkno,0,hInstMain,0);

    hListGraczy = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 405, 30, 147, 321, stolyOkno, NULL, hInstMain, NULL);

    hStolyWyloguj=CreateWindowEx(0,"BUTTON","Wyloguj",WS_CHILD|WS_VISIBLE,440,355,75,20,stolyOkno,0,hInstMain,0);

    return 0;
}
