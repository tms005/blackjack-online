 struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika\
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji 
char cChat[256];
};

Moje definicje globalne (Karol):
############################################
#define DEFAULT_BUFLEN					512
#define DEFAULT_PORT						"2000"
#define MAX_CLIENTS						12
#define MAX_TABLES							4
#define MAX_PLAYERS_PER_TABLE		3
#define MAX_NAME_LEN					15
#define MAX_PASS_LEN						40
#define BEGGINERS_CASH					300
#define MIN_BID								10
############################################




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

[ ID ]:	8	[ ID_USR ]  :   ID gracza ktorego dane chcesz pobrac (rowniez wlasne id)
		[ iKey[0] ] :	0 - chcesz pobrac informacje o gotowce danego gracza
				wartosc niezerowa - chcesz pobrac informacje o kartach danego gracza

				jesli iKey[0] == -1 - chcesz pobrac dane o kartach krupiera

				jesli iKey[0] > 0 id gracza ktorego karty chcesz pobrac
				
--------------------------------------------------------------------------
INFORMACJA O TYM, ZE KLIENT NIE MA NIC DO ZAKOMUNIKOWANIA SERWEROWI

[ ID ]: 10

Klient odsyła ten pakiet do serwera w przypadku. gdy gracz nie podjął żadnej akcji wymagającej przesłania komunikatu na serwer.
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

		[ iKey[2] ] :	Wysokosc srodkow na koncie gracza	//TUTAJ SERWER AUTOMATYCZNIE PRZYJMUJE ZE GRACZ MOZE MIEC MINIMUM 300 kredytow

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

		iKey[1-10] ]:	punkty 10 najlepszych graczy (miejsce zaleznym od indeksu iKey, im nizszy indeks, tym gracz lepszy) 
		
		[ cChat[ ] ] :	począwszy od pola 0 az do wyczerpania limitu oddzielone spacjami nicki 10 najlepszych graczy (Uwaga! Szukajcie znaku końca ciągu znaków ('\0') spacji bądź przekroczenia indeksu w buforze odbioru.


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



		jesli iKey[0] bylo niezerowe, serwer odsyla w iKey[1] wartosc karty ktora zalosowales/as
		jesli iKey[0] == 3 to w iKey[2] jest informacja czy wygrales czy przegrales (1 wygrana, 0 porazka)


--------------------------------------------------------------------------
TRANSFER DANYCH O KARTACH GRACZY/KRUPIERA:

[ ID ]:	8	[ ID_USR ]  :   ID gracza ktorego karty chcesz pobrac (rowniez wlasne id)

		[ iKey[0] ] :	0 - chcesz pobrac informacje o gotowce danego gracza
				wartosc niezerowa - chcesz pobrac informacje o kartach danego gracza
		
		jesli iKey[0] == 0 	to w iKey[1] serwer zwraca wysokosc konta danego gracza		
	
		jesli iKey[0] ==-1 	to w iKey[1] - ilosc pobranych przez krupiera kart		// w praktyce krupier pobiera na poczatku rozgrywki 2 karty
					oraz iKey[2-14] - wartosci kolejnych kart (o ile istnieja)	// a pozostale po tym jak pobrali wszyscy pozostali gracze
					dodatkowo: wartosc ID_USR w przypadku gdy iKey[0]==-1 JEST NIEUSTALONA!	(w koncu pobierasz od krupiera..)				


		jesli iKey[0] >0 	to w iKey[1] - ilosc pobranych przez gracza kart
					oraz iKey[2-14] - wartosci kolejnych kart (o ile istnieja)	

--------------------------------------------------------------------------
INFORMACJA O TYM, ZE SERWER OCZEKUJE NA PODJECIE DECYZJI WZG KOLEJNEJ TURY GRY

[ ID ]: 9

Serwer wysyla ta informacje do klienta, kiedy rozpoczyna sie jego runda.

--------------------------------------------------------------------------
INFORMACJA O TYM, ZE SERWER MOŻE ODBIERAĆ RZĄDANIA OD KLIENTA

[ ID ]: 10

Serwer wysyla ta informacje do klienta, kiedy otrzymuje prawa do wykonania czynności.
--------------------------------------------------------------------------