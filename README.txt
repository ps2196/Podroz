AUTHOR: Piotr S≥ysz

Program znajduje najtaÒszπ opcjÍ podrÛøy pomiÍdzy zadnymi miastami. 

Miasto jest opisane nazwπ oraz op≥atπ za wjazd.
Droga opisywana jest op≥atπ za przejazd i d≥ugoúciπ(mierzonπ w godzinach jazdy). Dodatkowo jest okreúlona sta≥a op≥ata za 1 godz, jazdy
Miasta mogπ tworzyÊ sieci miast partnerskich. Wjazd do jednego z miast partnerskich zwalnia z op≥aty 
za wjazd do pozosta≥ych miast z danej grupy.

Opcje uruchumienia programu:
-in <nazwa_pliku>	- plik z danymi wejúciowymi
-gen -c <liczba_miast>			- losowo generuje instancje problemu i rozwiπzuje go
     -r <liczba_drÛg> 
     -gp <koszt_paliwa>	
     [-ps <liczba_gr_part>] 
     [-ps <úr_wielkoúÊ_gr_part>] 
     [-mincf <min_oplata_miasto>]
     [-maxcf <max_oplata_miasto>]
     [-minrf <min_oplata_droga>]
     [-maxrf <max_oplata_droga>]
     [-minrl <min_dlugos_drogi>]
     [-maxrl <max_dlugosc_drogi>]	
-time			-pomiary czasu dla losowo generownych instancji poroblemu
-help			- pomoc

PLIKI èR”D£OWE:
Wersja VisualStudio: 2015
/AAL_Podroz
	main.cpp - obs≥uga interfejsu uøytkownika i trybÛw wykonania
	Graph.h - implementacja grafu
	City.h/City.cpp , Road.h - implementacja obiektÛw drogi i miasta
	PathFiner.h/PathFinder.cpp - klasa implementujπca algorytm znajdowania najkrÛtszej úcieøki
	Heap.h/Heap.cpp - implementacja kopca
	StringSplitter.h/StringSplitter.cpp - metoda pomocnicza wykorzystywana przy wczytywaniu pliku z grafu
/Release
	podroz.exe - skompilowany program realizujπcy algorytm 
	/in - w tym katalogu znajdujπ siÍ przyk≥adowe pliki testowe, ktÛre moøna wykorzystaÊ do zweryfikowania poprawnoúci

FORMAT PLIK”W WEJåCIOWYCH
pierwsza linia zawiera koszt paliwa
pusta linia
kolejne linie z opisem mias w formacie: nazwa;op≥ata
pusta linia
linie z opisem drÛg w formacie: nazwa_miasta1;nazwa_miasta2;op≥ata;d≥ugoúÊ
pusta linia
opis grup partnerskich w formacie - nazwy miast oddzielone úrednikami, kaøda grupa w nowej linii

Wszytskie koszty uwzgÍdniane w programie sπ liczbami ca≥kowitymi.
	

