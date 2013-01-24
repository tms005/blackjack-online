struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika\
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji 
char cChat[256];
};



Karol:
w kliencie zawrzyjcie strukturę która będzie posiadała informacje o stole takie jak:

ID graczy, ich Nazwy, Stawki (obstawiona oraz stan konta), zbior kart jakie ma kazdy lacznie z krupierem, suma punktow oraz aktywnie grajacy gracz




////////////////////////////////////////////////////////////////////////////////////////////////////////
						WYSYLAMY DO SERWERA:
////////////////////////////////////////////////////////////////////////////////////////////////////////

__________________________________________________________________________
WIADOMOSC CZATU:

[ ID ]:	0	[cChat] - wiadomosc zakonczona znakiem przejscia do nowej linii

--------------------------------------------------------------------------
UTWORZENIE KONTA:

[ ID ]:	1	[cChat] : 2 -  ciągi znaków, rozpoczynające się od indeksu 0, kończone spacją. Pierwszy z ciągów to nazwa użytkownika, drugi ciąg to hasło do konta 

--------------------------------------------------------------------------
LOGOWANIE/WYLOGOWANIE:

W przypadku logowania:
[ ID ]:	2	[cChat] : 2  - ciągi znaków, rozpoczynające się od indeksu 0, kończone spacją. Pierwszy z ciągów to nazwa użytkownika, drugi ciąg to hasło do konta 

w przypadku wylogowywania:
[ ID ]:	2

--------------------------------------------------------------------------
INFORMACJE:

[ ID ]:	3	[ iKey[0] ] :	0 - przeslij informacje o stolach
				1 - przeslij informacje o 10 najlepszych graczach w rankingu (nazwa, pkt, id)
				2 - przeslij informacje o zalogowanych graczach
				
--------------------------------------------------------------------------
PRZYLACZENIE SIE DO STOLU:

[ ID ]:	4	[ iKey[0] ]:	nr stolu do ktorego chcemy dolaczyc

--------------------------------------------------------------------------
OPUSZCZENIE DANEGO STOLU

[ ID ]:	5	

--------------------------------------------------------------------------
OPUSZCZENIE GRY:

[ ID ]:	6	

--------------------------------------------------------------------------
GRA:

[ ID ]:	7	[ iKey[0] ] :	0 - pasuj
				1 - graj
				2 - podwoj (tylko w pierwszej rundzie )

--------------------------------------------------------------------------
TRANSFER DANYCH O KARTACH GRACZY/KRUPIERA:

[ ID ]:	8	[ ID_USR ]  :   ID gracza ktorego karty chcesz pobrac (rowniez wlasne id)

				0 jesli chcesz pobrac dane o kartach krupiera
				

--------------------------------------------------------------------------




////////////////////////////////////////////////////////////////////////////////////////////////////////
						SERWER ODSYLA:
////////////////////////////////////////////////////////////////////////////////////////////////////////
__________________________________________________________________________
WIADOMOSC CZATU:

[ ID ]:	0 	[ID_USR]: ID gracza wysylajacego wiadomosc  [cChat] - wiadomosc // zalozenie: jesli wysylasz wiadomosc to otrzymasz ja tez z powrotem

--------------------------------------------------------------------------
UTWORZENIE KONTA:

[ ID ]:	1   	[ID_USR]: Nowoutworzone ID gracza, przyporządkowane do konta lub 0 jeśli błąd (np już istnieje podana nazwa uzytkownika)

--------------------------------------------------------------------------
LOGOWANIE/WYLOGOWANIE:

w przypadku logowania:

[ ID ]:	2	[ iKey[0] ] :	1 - zalogowales/as sie  
				2 - serwer zapelniony, nie udalo sie zalogowac 
				3 - bledne haslo lub nazwa uzytkownika
		
		[ iKey[1] ] :   ID uzytkownika dla konta za pomoca ktorego sie logowales/as

		[ iKey[2] ] :	Wysokosc srodkow na koncie gracza	//TUTAJ SERWER AUTOMATYCZNIE PRZYJMUJE ZE GRACZ MOZE MIEC MINIMUM 1000 kredytow

w przypadku wylogowywania:
serwer sprawdza czy uzytkownik ktory sie wylogowywal bral udzial w jakiejs grze, jesli tak to rozsyla informacje o tym, ze gracz opuscil stol, patrz OPUSZCZANIE STOLU

--------------------------------------------------------------------------
INFORMACJE:

[ ID ]:	3	[ iKey[0] ] :	0 - przeslij informacje o stolach
				1 - przeslij informacje o 10 najlepszych graczach w rankingu (nazwa, pkt, id)
				2 - przeslij informacje o zalogowanych graczach
				
JESLI iKey[0] == 0
		[ iKey[1-12] ]:	ID gracza siedzacego przy stole
		
		[ cChat[ ] ] :	począwszy od pola 0 az do wyczerpania limitu oddzielone spacjami nicki graczy ktorzy siedza przy stolach

		UWAGA! Zalozenie: jesli przy ktoryms miejscu przy stoliku nie siedzi gracz, to nick nalezy po prostu do pierwszego kolejnego przy ktorym siedzi.
JESLI iKey[0] == 1

		[ iKey[1-10] ]:	punkty 10 najlepszych graczy (miejsce zaleznym od indeksu iKey, im nizszy indeks, tym gracz lepszy) 
		
		[ cChat[ ] ] :	począwszy od pola 0 az do wyczerpania limitu oddzielone spacjami nicki 10 najlepszych graczy 

JESLI iKey[0] == 2

		[ cChat[ ] ] :	począwszy od pola 0 az do wyczerpania limitu oddzielone spacjami nicki zalogowanych graczy nie siedzacych przy stolach 

--------------------------------------------------------------------------

PRZYLACZENIE SIE DO STOLU:

[ ID ]:	4	[ ID_USR ]:	0 - dolaczyles/as do stolu i wszystko jest ok
				wartosc niezerowa - do stolu dolacza gracz o podanym w tym polu ID
		Jesli ID_USR niezerowe:

		[ cChat ]:	nazwa danego gracza zakonczone spacja lub innym bialym znakiem

--------------------------------------------------------------------------
OPUSZCZENIE DANEGO STOLU

[ ID ]:	5	[ ID_USR ] :	wartosc niezerowa - uzytkownik o takim ID opuscil stol.
		[ cChat ]  :	nazwa gracza opuszczającego zakonczone spacja lub innym bialym znakiem

--------------------------------------------------------------------------
OPUSZCZENIE GRY:

[ ID ]:	6	[ ID_USR ] :	ID gracza ktory opuscil gre (praktycznie to samo co wylogowanie)
		[ cChat ]  :	nazwa gracza opuszczającego zakonczone spacja lub innym bialym znakiem

--------------------------------------------------------------------------
GRA:

[ ID ]:	7	[ iKey[0] ] :	0 - pasuj
				1 - graj
				2 - podwoj (tylko w pierwszej rundzie )
				3 - rozgrywka zakonczona - tutaj nalezy pobrac dane o kartach krupiera(!) bo dopiero po zakonczeniu dobierania przez wszystkich
				    graczy sa one losowane 



		jesli iKey[0] == 1 lub 2, serwer odsyla w iKey[1] wartosc karty ktora zalosowales/as
		jesli iKey[0] == 3 to w iKey[1] jest informacja czy wygrales czy przegrales (1 wygrana, 0 porazka)


--------------------------------------------------------------------------
TRANSFER DANYCH O KARTACH GRACZY/KRUPIERA:

[ ID ]:	8	[ ID_USR ]  :   ID gracza ktorego karty chcesz pobrac (rowniez wlasne id)

				iKey[0] - ilosc pobranych przez gracza z ID w ID_USR kart
				iKey[1-12] : wartosci kolejnych pobranych przez niego/nia kart
				
--------------------------------------------------------------------------

