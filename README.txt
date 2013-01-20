struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla ka¿dego po³¹czonego z serwerem u¿ytkownika\
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji 
char cChat[256];
};


definicje kluczy ID :
WYSYLAMY DO SERWERA:

	0 - czat (w polu CChat mamy wiadomosc do przetworzenia);
	1 - utworzenie konta
	2 - logowanie/wylogowanie
	3 - wyszukiwanie dostepnych:
		iKey[0] - 0 : stolow do gier
			- 1 : graczy w rankingu
			- 2 : pkt graczy w rankingu
			- 3 : graczy online
	4 - przylaczenie sie do stolu
		iKey[0] - numer stolu do ktorego sie dolaczamy
	5 - utworzenie wlasnego stolu
	6 - opuszczenie stolu (powrot do menu gry)
	7 - opuszczenie gry
	8 - gramy (funkcja silnika gry)
		iKey[0] - 0 : pasujemy
			- 1 : dobieramy
			- 2 : podwajanie
	9 - wyst¹pienie b³êdu






SERWER ODSYLA: 
	zawsze kod klucza ID ktory wyslales jesli wszystko sie udalo (gdy serwer nie popelnil bledu)
	9 w przypadku gdy nastapil blad

	0 - ( w ID_USR : kod gracza ktory przesyla wiadomosc, w cChat - sama wiadomosc )
	1 - ( w ID_USR : Twoj kod gracza, bedacy Twoim identyfikatorem uzytkownika (od teraz stanowi sposob rozpoznania Twojego konta)
	      w ID     : (1) jesli sie zarejestrowales
			 (2) jesli takie konto juz istnieje
	
	2 - ( w ID     : (1) jesli sie zalogowales i wszystko jest ok
			 (2) to jest  jesli serwer jest zapelniony 
			 (3) dostajesz jesli haslo jest niepoprawne
	
	3 - ( w ID     : (wartosc niezerowa) jesli jakiekolwiek stoly istnieja 
			 (0) jesli nie ma zadnych stolow/graczy w rankingu/pkt/graczy online

		Jesli iKey[0] od klienta byl 0:
			 			iKey[0-3] - klucze stolu pierwszego, drugiego, trzeciego i czwartego
			 			iKey[4-15] - klucze uzytkownikow siedzacych przy stolach
						cChat[256] - nicki graczy przy stole(bo skad klient bez bazy z kluczami i nicami ma wiedziec kto ma jaki nick do ID_USR)
					     1: 
						cChat[256] - nicki graczy z rankingu
					     2: 
						cChat[256] - pkt graczy z rankingu
					     3: 
						cChat[256] - nicki graczy online
						

	4 - 	       : ID_USR - (0) jesli przylaczyles sie do stolu i wszystko jest ok
				  (wartosc niezerowa) - jesli do stolu dolacza gracz, to jest to jednoczesnie jego identyfikator
	 
	5 -	       : ID_USR - wartosc niezerowa - id stolu - jesli dalo sie utworzyc
				- 0 jesli nie udalo sie utworzyc stolu
	6 - 	       : ID_USR - (wartosc niezerowa) - uzytkownik o id podanym w tym polu opuscil stol
				  0 jesli pomyslnie opusciles stol
	7 - 	       : serwer nie odsyla zadnych danych, ma Cie w dupie
	8 - 	       : ID_USR - id uzytkownika ktory wyslal zapytanie (pomimo ze jeden klient wysyla zadanie do serwera, to serwer wysyla odpowiedz do wszystkich klientow)
		       : 	- iKey[0] - zapytanie uzytkownika jakie otrzymal serwer
		       :        - iKey[1] - wartosc dobranej karty o ile w iKey[0] jest wartosc niezerowa)

		       
