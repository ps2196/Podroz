AUTHOR: Piotr S�ysz

Program znajduje najta�sz� opcj� podr�y pomi�dzy zadnymi miastami. 

Miasto jest opisane nazw� oraz op�at� za wjazd.
Droga opisywana jest op�at� za przejazd i d�ugo�ci�(mierzon� w godzinach jazdy). Dodatkowo jest okre�lona sta�a op�ata za 1 godz, jazdy
Miasta mog� tworzy� sieci miast partnerskich. Wjazd do jednego z miast partnerskich zwalnia z op�aty 
za wjazd do pozosta�ych miast z danej grupy.

Opcje uruchumienia programu:
-in <nazwa_pliku>	- plik z danymi wej�ciowymi
-gen -c <liczba_miast>			- losowo generuje instancje problemu i rozwi�zuje go
     -r <liczba_dr�g> 
     -gp <koszt_paliwa>	
     [-ps <liczba_gr_part>] 
     [-ps <�r_wielko��_gr_part>] 
     [-mincf <min_oplata_miasto>]
     [-maxcf <max_oplata_miasto>]
     [-minrf <min_oplata_droga>]
     [-maxrf <max_oplata_droga>]
     [-minrl <min_dlugos_drogi>]
     [-maxrl <max_dlugosc_drogi>]	
-time			-pomiary czasu dla losowo generownych instancji poroblemu
-help			- pomoc

PLIKI �R�D�OWE:
Wersja VisualStudio: 2015
/AAL_Podroz
	main.cpp - obs�uga interfejsu u�ytkownika i tryb�w wykonania
	Graph.h - implementacja grafu
	City.h/City.cpp , Road.h - implementacja obiekt�w drogi i miasta
	PathFiner.h/PathFinder.cpp - klasa implementuj�ca algorytm znajdowania najkr�tszej �cie�ki
	Heap.h/Heap.cpp - implementacja kopca
	StringSplitter.h/StringSplitter.cpp - metoda pomocnicza wykorzystywana przy wczytywaniu pliku z grafu
/Release
	podroz.exe - skompilowany program realizuj�cy algorytm 
	/in - w tym katalogu znajduj� si� przyk�adowe pliki testowe, kt�re mo�na wykorzysta� do zweryfikowania poprawno�ci

FORMAT PLIK�W WEJ�CIOWYCH
pierwsza linia zawiera koszt paliwa
pusta linia
kolejne linie z opisem mias w formacie: nazwa;op�ata
pusta linia
linie z opisem dr�g w formacie: nazwa_miasta1;nazwa_miasta2;op�ata;d�ugo��
pusta linia
opis grup partnerskich w formacie - nazwy miast oddzielone �rednikami, ka�da grupa w nowej linii

Wszytskie koszty uwzg�dniane w programie s� liczbami ca�kowitymi.
	

