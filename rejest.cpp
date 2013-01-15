#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInstMain;
extern HWND Okno;

extern int stolyWinMain();
extern HWND stolyOkno;

extern int rankWinMain();
extern HWND rankOkno;

extern int stolWinMain();
extern HWND stolOkno;

HWND rejestrOkno;
CONST CHAR ClassName[]="Rejestracja";
CONST CHAR MenuName[]="Menu_Window";

HWND rejLogo;
HWND rejNick;
HWND rejNickWpisz;
HWND rejPass;
HWND rejPassWpisz;
HWND rejPassPowtorz;
HWND rejPassWpiszPowtorz;
HWND rejZaloz;

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

LRESULT CALLBACK rejestrWndProc(HWND hwnd,UINT msg,WPARAM wPar,LPARAM lPar)
{
        switch(msg)
        {
         /*case WM_CREATE:
         {
             break;
         }*/
         case WM_CLOSE:       //polecenia dla komunikatu WM_CLOSE
         {
            PostQuitMessage(0);
            //Sleep(50);
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

int WINAPI rejestrWinMain()
{
    //MSG rejestrmsgs;
    WNDCLASSEX rejestrwc;

    rejestrwc.hInstance = hInstMain;
    rejestrwc.lpszClassName = ClassName;                                 //nazwa klasy. przekazanie globalne.
    rejestrwc.lpfnWndProc = rejestrWndProc;                              //
    rejestrwc.style = 0;                                                 //
    rejestrwc.cbSize = sizeof (WNDCLASSEX);                              //rozmiar klasy w bajtach w pamiêci
    rejestrwc.hIcon = LoadIcon (NULL, IDI_APPLICATION);                  //uchwyt ikony okna
    rejestrwc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);                //uchwyt ma³ej ikony okna
    rejestrwc.hCursor = LoadCursor (NULL, IDC_ARROW);                    //uchwyt kursora - s³u¿y do za³adowania kursora tzw. "strza³ki"
    rejestrwc.lpszMenuName = "Menu_Window";                              //nazwa menu
    rejestrwc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 0);               //uchwyt do "pêdzla" z t³em
    rejestrwc.cbClsExtra = 0;                                            //dodatkowa pamiêæ dla okna klasy
    rejestrwc.cbWndExtra = 0;                                            //dodatkowa pamiêæ dla okna utworzona z tej klasy

    if(RegisterClassEx(&rejestrwc)==0) return 0;
    //rejestrOkno=CreateWindowEx(0,ClassName,"BlackJack",WS_OVERLAPPEDWINDOW|WS_VISIBLE,80,50,600,500,Okno,0,hInstMain,0);

    rejLogo=CreateWindowEx(0,"STATIC","BlackJack - Rejestracja",WS_CHILD|WS_VISIBLE,250,40,170,20,rejestrOkno,0,hInstMain,0);
    //SetWindowText(rejLogo,);
    rejNick=CreateWindowEx(0,"STATIC","Imi� u�ytkownika: ",WS_CHILD|WS_VISIBLE,90,90,150,20,rejestrOkno,0,hInstMain,0);
    //SetWindowText(rejNick,"Nick : ");
    rejNickWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE,220,90,200,20,rejestrOkno,0,hInstMain,0);
    rejPass=CreateWindowEx(0,"STATIC","Has�o:  ",WS_CHILD|WS_VISIBLE,160,150,50,20,rejestrOkno,0,hInstMain,0);
    //SetWindowText(rejPass,"Has�o : ");
    rejPassWpisz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_PASSWORD,220,150,200,20,rejestrOkno,0,hInstMain,0);
    rejPassPowtorz=CreateWindowEx(0,"STATIC","Powtorz has�o:  ",WS_CHILD|WS_VISIBLE,105,210,100,20,rejestrOkno,0,hInstMain,0);
    rejPassWpiszPowtorz=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_PASSWORD,220,210,200,20,rejestrOkno,0,hInstMain,0);

    rejZaloz = CreateWindowEx(0,"BUTTON","Za��",WS_VISIBLE|WS_CHILD,265,250,100,20,rejestrOkno,0,hInstMain,0);

    return 0;
}
