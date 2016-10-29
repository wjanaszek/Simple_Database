/* @author: Wojciech Janaszek
*	Prosta baza danych gazet. Struktura danych to drzewo binarne z list¹ w ka¿dym wêŸle tego drzewa.
*	Zapewniona funkcjonalnoœæ:
*	- dodawanie/usuwanie pojedynczych katalogow z gazetami
*	- przenoszenie gazet z jednego katalogu do innego
*	- wczytywanie bazy danych z pliku
*	- zapisywanie bazy danych do pliku
*	- zmiana nazwy dowolnego katalogu
*	- modyfikacja wprowadzonych danych
*	- wyszukiwanie i wyswietlanie listy rekordow wedlug zadanych parametrow
*	- wyswietlanie zawartosci dowolnego katalogu
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
* Defincja pojedynczego elementu listy. Pola:
* - nazwa gazety
* - czestotliwosc wydawania gazety
* - numer indentyfikacyjny gazety (nie moze powtarzac sie w calej bazie danych)
*/
typedef struct el_listy	{
	char nazwa[21];
	char czest[13];
	char num[6];
	struct el_listy *nast;
} lista;

/*
* Definicja pojedynczego wezla drzewa. Pola:
* - nazwa katalogu
* - wskaznik na poczatek listy
* - wskaznik na potomkow wezla
*/
typedef struct el_drzewa {
	char nazwa_katalogu[15];
	lista *dane;
	struct el_drzewa *lsyn;
	struct el_drzewa *psyn;
} drzewo;

void menu();
void menu_filtry();
void menu_filtry_czest();
void menu_filtry_par();
void menu_edycja();
void menu_edycja_cd();
void menu_pliki();
void menu_pliki_txt();
void menu_pliki_ula();
void nowy_kat(drzewo **nowy, char *nazwa_katalogu);
void wczytanie_katalog(char *pom);
void wczytanie_nazwa(lista *nazwa);
void wczytanie_czestotliwosc(lista *czest);
void wczytanie_numer(lista *num);
void dodaj(drzewo **dodaj, lista *do_dodania);
lista * rekord(drzewo *d);
void zmiana_nazwy(drzewo **zmien);
void wysw_kat(drzewo *d_wysw);
void usun(drzewo **do_usuniecia);
void usun_liste(lista **usunieta_lista);
void przenies(drzewo **d_przen);
int spr_nazwa(char *tekst0);
int spr_czest(char *tekst1);
int spr_num(char *tekst3);
void spr_num_pom(char *numer);
void wysw_zaw(drzewo *d_wysw);
void wysw(drzewo *d);
void wypisz(lista *wypisz);
void ile_wezlow(drzewo *d, int *licznik);
drzewo * wybor(drzewo *d);
drzewo * wybor_poprz(drzewo *d, drzewo **poprz);
drzewo * wybor_plik(drzewo *d, char * nazwa_katalogu);
drzewo * max_key(drzewo *d);
void wstaw(drzewo *d, drzewo **wybor, int *i);
void edytuj(drzewo **d);
void wysw_calosc(drzewo *d);
int liczba_el(lista *przeglad);
void wstaw_lista(lista *d, lista **tablica, int *i);
lista * wybor_l(lista *przeglad);
lista * wybor_l_przen(lista *przeglad, lista **poprz);
void wypisanie_listy(lista *wypisz);
void search_num(char* numer, drzewo *d, int* pom);
int search_num2(char* numer, drzewo *d);
void search_name(char* nazwa, drzewo *d, int* pom);
void search_name2(char* nazwa, drzewo *d, int* pom);
void search_freq(char* czest, drzewo *d, int* pom);
void eksport_txt(drzewo *d, char *nazwa_p);
void eksport_ula(drzewo *d, char *nazwa_p);
void import_txt(drzewo **d, char *nazwa_pliku);
void import_ula(drzewo **d, char *nazwa_pliku);
void rozroznienie(char *nazwa_pliku, char *pom);
int sort_nazwa_g(const void *a, const void *b);
void wpisz_bin(drzewo *d, FILE **nazwa);

/*
* Glowna petla programu.
*/
void main()
{
	char da[1];
	drzewo *drzewo_kat;
	drzewo_kat = NULL;
	menu();
	scanf("%1s", da);
	int ok = 1;
	int licznik = 0;
	while (ok == 1)
	{
		switch(da[0])
		{
			// Dodanie nowego katalogu:
			case '1':	{			
				char pom[16];
				wczytanie_katalog(pom);
				nowy_kat(&drzewo_kat, pom);
				printf("Katalog dodany pomyslnie.\n");
				menu();
				da[0] = '\0';
				fflush(stdin);
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Wyswietlenie katalogow w bazie danych:
			case '2':	{		
				if (drzewo_kat != NULL)
				{
					printf("Katalogi w bazie:\n");
					wysw_kat(drzewo_kat);
					printf("Najpierw wprowadz dane rekordu, a potem wybierz,\ndo ktorego katalogu ma on zostac dodany.\n");
					dodaj(&drzewo_kat, rekord(drzewo_kat));
					printf("Rekord zostal pomyslnie dodany do katalogu.\n");
				}
				else
				{
					printf("Brak katalogow w bazie danych. Czy chcesz teraz dodac katalog? T/N.\n");
					scanf("%1s", da);
					if ((da[0] == 't') ||(da[0] == 'T'))
					{
						char nazwa[16];
						wczytanie_katalog(nazwa);
						nowy_kat(&drzewo_kat, nazwa);
						printf("Katalog zostal pomyslnie dodany do bazy danych.\n");
					}
				}
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Zmiana nazwy wybranego katalogu:
			case '3': 	{			
				zmiana_nazwy(&drzewo_kat);
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Usuniecie katalogu/katalogow:
			case '4': 	{
				if (drzewo_kat != NULL)
				{
					usun(&drzewo_kat);
				}
				else
					printf("Brak katalogow. Operacja niemozliwa.\n");
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Modyfikacja zawartosci bazy danych:
			case '5': 	{			
				edytuj(&drzewo_kat);
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Przeniesienie pewnych pozycji z jednego katalogu do innego:
			case '6': 	{			
				przenies(&drzewo_kat);
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Wyswietlanie rekordow wedlug zadanych parametrow:
			case '7': 	{		
				if (drzewo_kat != NULL)
				{
					menu_filtry();
					da[0] = '\0';
					scanf("%1s", da);
					switch(da[0])
					{
						case '1':	{
							menu_filtry_czest();
							char c[1];
							scanf("%1s", c);
							switch(c[0])
							{
								case '1':	{
									/*gazety wydawane raz na miesiac.*/
									char* pom = {"miesiecznik"};
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									search_freq(pom, tmp, &pom1);
									fflush(stdin);
									if (pom1 == 0)
										printf("Brak wynikow wyszukiwania dla zadanej czestotliwosci.\n");
								break;
								}
								case '2':	{
									/*gazety wydawane raz na tydzien.*/
									char* pom = {"tygodnik"};
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									search_freq(pom, tmp, &pom1);
									if (pom1 == 0)
										printf("Brak wynikow wyszukiwania dla zadanej czestotliwosci.\n");
									break;
								}
								case '3':	{
									/*gazety wydawane codziennie.*/
									char* pom = {"codziennie"};
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									search_freq(pom, tmp, &pom1);
									fflush(stdin);
									if (pom1 == 0)
										printf("Brak wynikow wyszukiwania dla zadanej czestotliwosci.\n");
									break;
								}
								default:	{
									printf("Nie rozpoznano polecenia.\n");
									break;
								}
							}
							break;
						}
						case '2':	{
							menu_filtry_par();
							da[0] = '\0';
							scanf("%1s", da);
							switch(da[0])
							{
								case '1':	{			
									printf("Podaj numer gazety, ktora chcesz wyszukac w bazie danych:\n");
									char pom[6];
									fflush(stdin);
									fgets(pom, 7, stdin);
									pom[strlen(pom) - 1] = '\0';
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									printf("Krok.\n");
									search_num(pom, tmp, &pom1);
									if (pom1 == 0)
										printf("Nie znaleziono zadnego rekordu spelniajacego zadane wymagania.\n");
									break;
								}
								case '2':	{				
									printf("Podaj nazwe gazety, jaka chcesz wyszukac w bazie danych:\n");
									char pom[16];
									fflush(stdin);
									fgets(pom, 17, stdin);
									pom[strlen(pom) - 1] = '\0';
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									search_name(pom, tmp, &pom1);
									if (pom1 == 0)
										printf("Nie znaleziono zadnego rekordu spelniajacego zadane wymagania.\n");
									break;
								}
								case '3':	{				
									printf("Podaj czestotliwosc wydawania gazety, ktora chcesz wyszukac w bazie danych:\n");
									char pom[13];
									fflush(stdin);
									fgets(pom, 14, stdin);
									pom[strlen(pom) - 1] = '\0';
									int pom1 = 0;
									drzewo *tmp;
									tmp = NULL;
									tmp = drzewo_kat;
									search_freq(pom, tmp, &pom1);
									if (pom1 == 0)
										printf("Brak wynikow wyszukiwania dla zadanej czestotliwosci.\n");
									break;
								}
								case '4':	{				
									if (drzewo_kat->dane != NULL)
									{
										printf("Katalog        \t                   \tRekord\n");
										printf("               \tnazwa gazety       \tnumer\tczestotliwosc wydania\n\n");
										wysw_calosc(drzewo_kat);
									}
									else
										printf("Brak rekordow do wyswietlenia.\n");
									break;
								}
								default:	{			
									printf("Nie rozpoznano polecenia.\n");
									break;
								}
							}
							break;		/*of case '7'*/
						}
						default:	{			
							printf("Nie rozpoznano polecenia.\n");
							break;
						}
					}
				}
				else
					printf("Brak katalogow. Operacja niemozliwa.\n");
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			case '8': 	{				
				printf("Katalogi znajdujace sie w bazie danych:\n");
				if (drzewo_kat == NULL)
					printf("\t(brak)\n");
				wysw_kat(drzewo_kat);
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Wyswietlenie zawartosci bazy danych wraz z nazwami katalogow:
			case '9': 	{			
				wysw_zaw(drzewo_kat);
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// Zakonczenie dzialania programu:
			case '0': 	{			
				printf("Jestes pewien? T/N.\n");
				fflush(stdin);
				scanf("%1s", da);
				if ((da[0] == 't') || (da[0] == 'T'))
					ok = 0;
				else
				{
					menu();
					fflush(stdin);
					scanf("%1s", da);
					ok = 1;
				}
				break;
			}
			
			// Modul obslugujacy import/eksport danych z/do pliku:
			case 'P':	{
				menu_pliki();
				scanf("%1s", da);
				switch(da[0])
				{
					case '1':	{								
						/*eksport*/
						char nazwa_pliku[100];
						char pom[6];
						rozroznienie(nazwa_pliku, pom);
						if (strcmp(pom, ".txt") == 0)
						{
							eksport_txt(drzewo_kat, nazwa_pliku);
							printf("Operacja zakonczona pomyslnie.\n");
						}
						else
							if (strcmp(pom, ".ula") == 0)
							{
								eksport_ula(drzewo_kat, pom);
								printf("Operacja zakonczona pomyslnie.\n");
							}
							else
								printf("Plik nie zostal znaleziony.\n");
						break;
					}
					case '2':	{
						/*import do bazy danych.*/
						char nazwa_pliku[100];
						char pom[6];
						rozroznienie(nazwa_pliku, pom);
						if (strcmp(pom, ".txt") == 0)
						{
							import_txt(&drzewo_kat, nazwa_pliku);				
							printf("Operacja zakonczona pomyslnie.\n");
						}
						else
							if (strcmp(pom, ".ula") == 0)
							{
								import_ula(&drzewo_kat, pom);
								printf("Operacja zakonczona pomyslnie.\n");
							}
							else
								printf("Plik nie zostal znaleziony.\n");
						break;
					}
					default:	{
						printf("Niepoprawne polecenie.\n");
						break;
					}
				}
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
			
			// W przypadku nie rozpoznania polecenia:
			default: {		
				printf("Niepoprawne polecenie. Ponow probe.\n");
				menu();
				da[0] = '\0';
				scanf("%1s", da);
				ok = 1;
				break;
			}
		}
	}
	return;
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*------------------------------(poczatek) Menu--------------------------------------*/

void menu()
{
	printf("\n----------------------------||MENU||--------------------------------------------\n");
	printf("Aby program wykonal polecenie wpisz numer (lub litere), przy ktorym \nznajduje sie polecenie, ktore chcesz wykonac (wprowadz numer\ni zatwierdz klawiszem 'ENTER').\nDostepne polecenia: \n");
	printf("1. Tworzenie nowych katalogow.\n");
	printf("2. Dodanie rekordow do wybranego katalogu.\n");
	printf("3. Zmiana nazwy katalogu. Mozliwy wybor, ktory ma zostac zmieniony. \n");
	printf("4. Usuniecie wybranego, istniejacego katalogu wraz z zawartoscia. \n");
	printf("5. Edycja istniejacych rekordow bazy danych.\n");
	printf("6. Przenoszenie rekordow z jednego katalogu do innego.\n");
	printf("7. Wyszukiwanie i wyswietlanie listy rekordow. Dostepne sa rozne \n   filtry wyszukiwania.\n");
	printf("8. Wyswietlenie nazw wszystkich katalogow wedlug kolejnosci alfabetycznej.\n");
	printf("9. Wyswietlenie zawartosci dowolnie wybranego katalogu.\n");
	printf("P. Obsluga plikow.\n");
	printf("===============================================================================\n");
	printf("0. Zakoncz.\n");
	return;
}

void menu_filtry()
{
	printf("Wybierz, ktory filtr ma byc aktywny przy wyszukiwaniu. Nie zaznaczenie\nzadnej opcji spowoduje wyswietlenie wszystkich rekordow bazy danych,\nwedlug ");
	printf("kolejnosci alfabetycznej dzialow.\n");
	printf("  1. Czestotliwosc wydawania.\n");
	printf("  2. Dane o okreslonym parametrze (np wyswietlenie danych gazety\n     o zadanym numerze).\n  ");
	return;
}

void menu_filtry_czest()
{
	printf("Dostepne akcje:\n");
	printf("  1. Pokaz gazety wydawane raz na miesiac.\n");
	printf("  2. Pokaz gazety wydawane raz na tydzien.\n");
	printf("  3. Pokaz gazety wydawane codziennie.\n  ");
	return;
}

void menu_filtry_par()
{
	printf("Podaj wedlug jakiego parametru chcesz wyswietlic dane:\n");
	printf("  1. Znajdz dane gazety o zadanym numerze.\n");
	printf("  2. Znajdz dane gazety o zadanej nazwie.\n");
	printf("  3. Znajdz dane gazety o zadanej czestotliwosci wydawania.\n");
	printf("  4. Wyswietl wszystkie rekordy bazy danych.\n  ");
	return;
}
/*------------------------------------------------------------------------------*/

void menu_edycja()
{
	printf("Wybierz:\n");
	printf("  1. Czy chcesz edytowac wszystkie dane wybranego rekordu?\n");
	printf("  2. Czy chcesz edytowac jedna z danych?\n  ");
	return;
}

void menu_edycja_cd()
{
	printf("Podaj, ktora dana chcesz zedytowac:\n");
	printf("  1. Nazwe gazety.\n");
	printf("  2. Czestotliwosc wydawania gazety.\n");
	printf("  3. Numer gazety.\n  ");
	return;
}

void menu_pliki()
{
	printf("Dostepne dzialania:\n");
	printf("  1. Eksport rekordow bazy danych do pliku.\n");
	printf("  2. Import danych z pliku do bazy danych.\n  ");
	return;
}

/*----------------------------------(koniec) Menu------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*----------------------------------(poczatek) Nowy katalog--------------------------*/

void nowy_kat(drzewo **nowy, char *nazwa_katalogu)
{
	if (*nowy == NULL)
	{
		*nowy = (drzewo *)malloc(sizeof(drzewo));
		strcpy((*nowy)->nazwa_katalogu, nazwa_katalogu);
		(*nowy)->dane = NULL;
		(*nowy)->lsyn = NULL;
		(*nowy)->psyn = NULL;
	}
	else
	{
		if (strcmp(nazwa_katalogu, (*nowy)->nazwa_katalogu) < 0)
			nowy_kat(&((*nowy)->lsyn), nazwa_katalogu);
		else if (strcmp(nazwa_katalogu, (*nowy)->nazwa_katalogu) > 0)
			nowy_kat(&((*nowy)->psyn), nazwa_katalogu);
	}
	return;
}

/*----------------------(koniec) Nowy katalog----------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*----------------------(poczatek) Funkcje wczytujace--------------------------------*/

void wczytanie_katalog(char *pom)
{
	while (getchar() != '\n');
	printf("Podaj nazwe katalogu:\n");
	fgets((pom), 17, stdin);
	(pom)[strlen(pom) - 1] = '\0';
	if (islower(pom[0]) != 0)
		pom[0] = toupper(pom[0]);
	return;
}

void wczytanie_nazwa(lista *name)
{
	int pom = 0;
	while (pom == 0)
	{
		while (getchar() != '\n');
		printf("Podaj nazwe gazety:\n");
		fflush(stdin);
		fgets(name->nazwa, 22, stdin);
		(name->nazwa)[strlen(name->nazwa) - 1] = '\0';
		if (islower(name->nazwa[0]) != 0)
			name->nazwa[0] = toupper(name->nazwa[0]);
		pom = spr_nazwa(name->nazwa);
		if (pom == 0)
		{
			printf("Podaj nazwe gazety jeszcze raz. Nie moze ona zawierac cyfr,\noraz niealfabetycznych znakow, jak np. !, @ itp.\n");
		}
	}
	return;
}

void wczytanie_czestotliwosc(lista *freq)
{
	int pom = 0;
	while (pom == 0)
	{
		printf("Podaj czestotliwosc wydawania gazety:\n");
		while (getchar() != '\n');
		fflush(stdin);
		fgets(freq->czest, 14, stdin);
		(freq->czest)[strlen(freq->czest) - 1] = '\0';
		pom = spr_czest(freq->czest);
		if (pom == 0)
		{
			printf("Podaj czestotliwosc wydawania gazety jeszcze raz. Nie moze ona zawierac cyfr,\noraz niealfabetycznych znakow, jak np. !, @ itp.\n");
		}
	}
	return;
}

void wczytanie_numer(lista *numer)
{
	int pom = 0;
	while (pom == 0)
	{
		printf("Podaj numer gazety:\n");
		while (getchar() != '\n');
		fflush(stdin);
		fgets(numer->num, 7, stdin);
		(numer->num)[strlen(numer->num) - 1] = '\0';
		pom = spr_num(numer->num);
	}
	return;
}

/*------------------------(koniec) Funkcje wczytujace--------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*--------------------------(poczatek) Fukcje usuwania-------------------------------*/

void usun(drzewo **do_usuniecia)
{
	drzewo *pom, *pom2;
	pom = NULL;
	pom = *do_usuniecia;
	pom2 = *do_usuniecia;
	if (*do_usuniecia != NULL)
	{
		drzewo *tmp, *poprz;
		tmp = NULL;
		printf("Podaj, ktory katalog chcesz usunac:\n");
		*do_usuniecia = wybor_poprz(*do_usuniecia, &poprz);
		if (((*do_usuniecia)->psyn == NULL) && ((*do_usuniecia)->lsyn == NULL))			
		{
			if (poprz != NULL)
			{
				if (poprz->psyn == (*do_usuniecia))
					poprz->psyn = NULL;
				if (poprz->lsyn == (*do_usuniecia))
					poprz->lsyn = NULL;
			}
			free(*do_usuniecia);
			if (poprz == NULL)
				(*do_usuniecia) = NULL;
			else
			{
				(*do_usuniecia) = poprz;
				(*do_usuniecia) = pom2;
			}
		}
		else
			if (((*do_usuniecia)->psyn != NULL) && ((*do_usuniecia)->lsyn == NULL))		
			{
				drzewo *pom3;	
				pom3 = (*do_usuniecia)->psyn;
				free(*do_usuniecia);
				*do_usuniecia = pom3;
				if (poprz != NULL)
					if (strcmp(((*do_usuniecia)->nazwa_katalogu), (poprz->nazwa_katalogu)) < 0)
					{
						poprz->lsyn = (*do_usuniecia);
						(*do_usuniecia) = poprz;
						(*do_usuniecia) = pom;
						pom = NULL;
					}
					else
					{
						poprz->psyn = (*do_usuniecia);
						(*do_usuniecia) = poprz;
						(*do_usuniecia) = pom;
						pom = NULL;
					}
			}
			else
				if (((*do_usuniecia)->psyn == NULL) && ((*do_usuniecia)->lsyn != NULL))		
				{
					drzewo *pom3;	/*poprz to wezel poprzedzajacy*/
					pom3 = (*do_usuniecia)->lsyn;
					free(*do_usuniecia);
					*do_usuniecia = pom3;
					if (poprz != NULL)
						if (strcmp(((*do_usuniecia)->nazwa_katalogu), (poprz->nazwa_katalogu)) < 0)
						{
							poprz->psyn = (*do_usuniecia);
							(*do_usuniecia) = poprz;
							(*do_usuniecia) = pom;
							pom = NULL;
						}
						else
						{
							poprz->lsyn = (*do_usuniecia);
							(*do_usuniecia) = poprz;
							(*do_usuniecia) = pom;
							pom = NULL;
						}
				}
				else
					if (((*do_usuniecia)->psyn != NULL) && ((*do_usuniecia)->lsyn != NULL))		
					{
						drzewo *pom5, *pom6, *max;	/*poprz to wezel poprzedzajacy*/
						pom5 = (*do_usuniecia)->psyn;
						pom6 = (*do_usuniecia)->lsyn;
						free(*do_usuniecia);
						(*do_usuniecia) = pom5;
						max = max_key(pom6);		/*znajdujemy maksymalny element lewego poddrzewa.*/
						max->psyn = (*do_usuniecia);
						(*do_usuniecia) = max;
						if (poprz != NULL)
						{
							if (poprz->lsyn == NULL)
							{
								poprz->lsyn = pom6;
								(*do_usuniecia) = poprz;
								poprz = NULL;
								pom6 = NULL;
								pom5 = NULL;
							}
							if (poprz->psyn == NULL)
							{
								poprz->psyn = pom6;
								(*do_usuniecia) = poprz;
								poprz = NULL;
								pom6 = NULL;
								pom5 = NULL;
							}
						}
						else
							(*do_usuniecia) = max;
					}
		printf("Operacja zakonczona pomyslnie.\n");
	}
	return;
}

/*-------------------------(koniec) Funkcje usuwania---------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------(poczatek) Funkcje sprawdzajace dane------------------------*/

int spr_nazwa(char *tekst0)
{
	int i, pom;
	i = 0;
	if (isdigit(tekst0[i]) == 0)
		if (isupper(tekst0[i]) != 0)
		{
			i = 1;
			for (i = 1; i <= strlen(tekst0); i++)
			{
				if (isalpha(tekst0[i]) == 0)
					if ((tekst0[i] != '\0') && (isalpha(tekst0[i + 1]) != 0))
						continue;
					else
						break;
				if (islower(tekst0[i]) == 0)
					break;
				if (isdigit(tekst0[i]) != 0)
					break;
			}
			if (i != strlen(tekst0))
			{
				pom = 0;
			}
			else
			{
				pom = 1;
			}
		}
		else
			pom = 0;
	else
		pom = 0;
	if (strlen(tekst0) > 20)
		pom = 0;
	return pom;
}

/*------------------------------------------------------------------------------*/

int spr_czest(char *tekst1)
{
	int i, pom;
	i = 0;
	if (isdigit(tekst1[i]) == 0)
	{
		for (i = 0; i <= strlen(tekst1); i++)
		{
			if (isdigit(tekst1[i]) != 0)
				break;
			if (islower(tekst1[i]) == 0)
				break;
			if (isalpha(tekst1[i]) == 0)
				break;
		}
		if (i != strlen(tekst1))
		{
			pom = 0;
		}
		else
		{
			pom = 1;
		}
	}
	else
		pom = 0;
	if (strlen(tekst1) > 12)
		pom = 0;
	return pom;
}

/*------------------------------------------------------------------------------*/

int spr_num(char *tekst3)
{
	int i, pom;
	i = 0;
	pom = 1;
	if (strlen(tekst3) == 5)
	{
		for (i = 0; i <= strlen(tekst3); i++)
		{
			if (isdigit(tekst3[i]) == 0)
				break;
		}
		if (i != strlen(tekst3))
		{
			pom = 0;
		}
		else
		{
			pom = 1;
		}
	}
	else
		pom = 0;
	return pom;
}

/*------------------------------------------------------------------------------*/

/*------------------------(koniec) Funkcje sprawdzajace dane-------------------------*/
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void ile_wezlow(drzewo *d, int *licznik)
{
	if (d != NULL)
	{
		*licznik = *licznik + 1;
		ile_wezlow(d->lsyn, licznik);
		ile_wezlow(d->psyn, licznik);
	}
	return;
}

void wstaw(drzewo *d, drzewo **wybor, int *i)
{
	if (d != NULL)
	{
		wybor[*i] = d;
		*i = *i + 1;
		wstaw(d->lsyn, wybor, i);
		wstaw(d->psyn, wybor, i);
	}
	return;
}

drzewo * wybor(drzewo *d)
{
	int n = 0;
	ile_wezlow(d, &n);
	int n1;
	n1 = n;
	drzewo **tab;
	int i = 0;
	tab = NULL;
	tab = (drzewo**)malloc(n*sizeof(drzewo*));
	wstaw(d, tab, &i);
	do	{
    	for (i = 0; i < n - 1; i++)
    	{
      		if (strcmp(tab[i]->nazwa_katalogu, tab[i + 1]->nazwa_katalogu) > 0)
      		{
        		drzewo *pom;
        		pom = NULL;
        		pom = tab[i];
        		tab[i] = tab[i + 1];
        		tab[i + 1] = pom;
        		pom = NULL;
        	}
        }
    	n = n - 1;
    }
  	while (n > 1);
	i = 0;
	for (i = 0; i < n1; i++)
		printf("%d. %s\n", i, tab[i]);
	int pom;
	scanf("%d", &pom);
	drzewo *pom1;
	pom1 = tab[pom];
	free(tab);
	return pom1;
}

drzewo * wybor_poprz(drzewo *d, drzewo **poprz)
{
	int n = 0;
	ile_wezlow(d, &n);
	int n1;
	n1 = n;
	drzewo **tab;
	int i = 0;
	tab = NULL;
	tab = (drzewo**)malloc(n*sizeof(drzewo*));
	wstaw(d, tab, &i);
	i = 0;
	for (i = 0; i < n1; i++)
		printf("%d. %s\n", i, tab[i]);
	int pom;
	scanf("%d", &pom);
	drzewo *pom1;
	pom1 = tab[pom];
	if (pom - 1 < 0)
		*poprz = NULL;
	else
		*poprz = tab[pom - 1];
	free(tab);
	return pom1;
}

drzewo * wybor_plik(drzewo *d, char * nazwa_katalogu)
{
	int n = 0;
	ile_wezlow(d, &n);
	drzewo **tab;
	int i = 0;
	tab = NULL;
	tab = (drzewo**)malloc(n*sizeof(drzewo*));
	wstaw(d, tab, &i);
	for (i = 0; i < n; i++)
	{
		if (strcmp(tab[i]->nazwa_katalogu, nazwa_katalogu) == 0)
			break;
	}
	drzewo *pom1;
	pom1 = tab[i];
	free(tab);
	return pom1;
}

drzewo * max_key(drzewo *d)
{
	if (d != NULL)
	{
		drzewo *tmp;
		tmp = d;
		max_key(d->psyn);
		return tmp;
	}
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------(poczatek) Edycja-------------------------------------*/

void edytuj(drzewo **d)				
{
	drzewo *pom, *pom1;
	pom = NULL;
	pom = *d;				/*zapamietanie korzenia*/
	pom1 = *d;
	if (*d != NULL)
	{
		printf("Podaj, w ktorym katalogu znajduja sie rekordy, ktore chcesz edytowac:\n");
		*d = wybor(*d);
		menu_edycja();
		char c[1];
		scanf("%1s", c);
		switch(c[0])
		{
			case '1':	{
				lista *tmp;
				tmp = NULL;
				tmp = (*d)->dane;		/*zapamietanie poczatku listy*/
				printf("Podaj numer indeksu, przy ktorym stoi element, ktorego dane chcesz edytowac:\n");
				(*d)->dane = wybor_l((*d)->dane);
				wczytanie_nazwa((*d)->dane);
				wczytanie_czestotliwosc((*d)->dane);
				wczytanie_numer((*d)->dane);
				(*d)->dane = tmp;
				tmp = NULL;
				printf("Dane zostaly pomyslnie zaktualizowane.\n");
				break;
			}
			case '2':	{
				lista *tmp;
				tmp = NULL;
				tmp = (*d)->dane;
				menu_edycja_cd();
				char c[1];
				scanf("%1s", c);
				switch(c[0])
				{
					case '1':	{
						printf("Podaj numer indeksu, przy ktorym stoi element, ktorego dana chcesz edytowac:\n");
						(*d)->dane = wybor_l((*d)->dane);
						wczytanie_nazwa((*d)->dane);
						printf("Nazwa zostala pomyslnie zmieniona.\n");
						break;
					}
					case '2':	{
						printf("Podaj numer indeksu, przy ktorym stoi element, ktorego dana chcesz edytowac:\n");
						(*d)->dane = wybor_l((*d)->dane);
						wczytanie_czestotliwosc((*d)->dane);
						printf("Czestotliwosc wydawania zostala pomyslnie zmieniona.\n");
						break;
					}
					case '3':	{
						printf("Podaj numer indeksu, przy ktorym stoi element, ktorego dana chcesz edytowac:\n");
						(*d)->dane = wybor_l((*d)->dane);
						wczytanie_numer((*d)->dane);
						printf("Numer gazety zostal pomyslnie zmieniony.\n");
						break;
					}
				}
				(*d)->dane = tmp;
				tmp = NULL;
				break;
			}
		}
		*d = pom;
		pom = NULL;
	}
	else
		printf("Brak katalogow i rekordow. Operacja edycji niemozliwa.\n");
	return;
}

/*--------------------------(koniec) Edycja------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*--------------------------(poczatek) Zmiana nazwy----------------------------------*/

void zmiana_nazwy(drzewo **zmien)		
{
	drzewo *pom1;
	pom1 = NULL;
	pom1 = *zmien;
	if (*zmien != NULL)
	{
		printf("Podaj, ktory sposrod wymienionych nizej nazw katalogow chcesz zmienic: \n");
		*zmien = wybor(*zmien);
		fflush(stdin);
		while (getchar() != '\n');
		printf("Podaj nazwe katalogu:\n");
		printf("%s -> ", (*zmien)->nazwa_katalogu);
		fgets((*zmien)->nazwa_katalogu, 31, stdin);
		(*zmien)->nazwa_katalogu[strlen((*zmien)->nazwa_katalogu) - 1] = '\0';
		if (islower((*zmien)->nazwa_katalogu[0]) != 0)
			(*zmien)->nazwa_katalogu[0] = toupper((*zmien)->nazwa_katalogu[0]);
		printf("Nazwa zostala poprawnie zmieniona.\n");
		*zmien = pom1;
	}
	else
		printf("Brak katalogow w bazie.\n");
	return;
}

/*--------------------------(koniec) Zmiana nazwy------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*--------------------------(poczatek) Wyswietlanie----------------------------------*/

void wysw_kat(drzewo *drzewo)
{
	if (drzewo != NULL)
	{
		wysw_kat(drzewo->lsyn);
		printf("  - %s,\n", drzewo->nazwa_katalogu);
		wysw_kat(drzewo->psyn);
	}
	return;
}

void wysw(drzewo *d)				
{
	drzewo *tmp;
	tmp = NULL;
	tmp = d;
	if (d != NULL)
	{
		printf("%s", d->nazwa_katalogu);
		if (strlen(d->nazwa_katalogu) < 15)
		{
			int i = 0;
			for (i = 0; i <= 15 - strlen(d->nazwa_katalogu); i++)
				printf(" ");
		}
		else
			printf(" ");
		int pom = 1;
		int ok;
		lista *tmp_l, *pocz;
		tmp_l = NULL;
		tmp_l = d->dane;
		pocz = tmp_l;
		ok = liczba_el(tmp_l);
		lista **wskazniki;
		wskazniki = (lista**)malloc(ok*sizeof(lista*));
		int i = 0;
		while (tmp_l != NULL)
		{
			wskazniki[i] = tmp_l;
			tmp_l = tmp_l->nast;
			i = i + 1;
		}
		tmp_l = pocz;
		i = 0;
		do	{
			for (i = 0; i < ok - 1; i++)
			{
				if (strcmp(wskazniki[i]->nazwa, wskazniki[i + 1]->nazwa) > 0)
				{
					lista *pom;
       				pom = NULL;
       				pom = wskazniki[i];
       				wskazniki[i] = wskazniki[i + 1];
       				wskazniki[i + 1] = pom;
       				pom = NULL;
				}
			}
			ok = ok - 1;
		}
		while (ok > 1);
		i = 0;
		tmp_l = wskazniki[i];
		i = 1;
		if (tmp_l->nast != NULL)
		{
			tmp_l = tmp_l->nast;
			for (i = 1; i < ok; i++)
			{
				tmp_l = wskazniki[i];
				tmp_l = tmp_l->nast;
			}
		}
		tmp_l->nast = NULL;
		tmp_l = pocz;
		wypisz(tmp_l);
		tmp_l = tmp_l->nast;
		pom = 2;
		while ((tmp_l != NULL) && (pom != 1))
		{
			int i = 0;
			for (i = 0; i <= 15; i++)
				printf(" ");
			wypisz(tmp_l);
			tmp_l = tmp_l->nast;
		}
		printf("\n");
		tmp_l = pocz;
		pocz = NULL;
		tmp_l = NULL;
		free(wskazniki);
	}
	else
		printf("Nie wprowadzono jeszcze zadnych rekordow do bazy danych.\n");
	d = tmp;
	tmp = NULL;
	return;
}

void wypisz(lista *wypisz)
{
	int	pom = 1;
	int dl_czest, dl_nazwa, dl_num;
	dl_czest = strlen(wypisz->czest);
	dl_nazwa = strlen(wypisz->nazwa);
	dl_num = strlen(wypisz->num);
	printf("%s", wypisz->nazwa);
	if (dl_nazwa < 20)
	{
		for (pom = 1; pom < 20 - dl_nazwa; pom++)
		{
			printf(" ");
		}
	}
	printf("\t");
	pom = 1;
	printf("%s", wypisz->num);
	if (dl_num < 5)
	{
		for (pom = 1; pom < 5 - dl_num; pom++)
		{
			printf(" ");
		}
	}
	printf("\t");
	pom = 1;
	printf("%s", wypisz->czest);
	if (dl_czest < 12)
	{
		for (pom = 1; pom < 12 - dl_czest; pom++)
		{
			printf(" ");
		}
	}
	printf("\n");
	return;
}

void wysw_zaw(drzewo *d_wysw)			
{
	if (d_wysw != NULL)
	{
		drzewo *tmp;
		tmp = NULL;
		tmp = d_wysw;
		printf("Podaj, ktorego sposrod wyswietlonych katalogow zawartosc chcesz zobaczyc: \n");
		d_wysw = wybor(d_wysw);
		if (d_wysw->dane == NULL)
			printf("Brak rekordow w zadanym katalogu.\n");
		else
		{
			printf("Katalog        \t                   \tRekord\n");
			printf("               \tnazwa gazety       \tnumer\tczestotliwosc wydania\n\n");
			lista *pom;
			pom = NULL;
			pom = d_wysw->dane;
			if ((d_wysw)->dane != NULL)
			{
				wysw(d_wysw);
			}
			else
				printf("Brak rekordow.\n");
			d_wysw->dane = pom;
			pom = NULL;
			d_wysw = tmp;
			tmp = NULL;
		}
		d_wysw = tmp;
	}
	else
		printf("Brak katalogow.\n");
	return;
}

void wysw_calosc(drzewo *d)
{
	if (d != NULL)
	{
		wysw_calosc(d->lsyn);
		if (d->dane != NULL)
			wysw(d);
		wysw_calosc(d->psyn);
	}
	return;
}

/*------------------------------(koniec) Wyswietlanie------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*------------------------------(poczatek) Dodawanie rekordu-----------------------------------------*/

lista * rekord(drzewo *d)
{
	lista *pom;
	pom = NULL;
	pom = (lista *)malloc(sizeof(lista));
	wczytanie_nazwa(pom);
	wczytanie_czestotliwosc(pom);
	wczytanie_numer(pom);
	int zm = search_num2(pom->num, d);
	while (zm == 1)
	{
		printf("Podano numer znajdujacy sie juz w bazie danych. Podaj inny numer:\n");
		wczytanie_numer(pom);
		zm = search_num2(pom->num, d);
	}
	pom->nast = NULL;
	return pom;
}

void dodaj(drzewo **dodaj, lista *do_dodania)		
{
	int pom1 = 0;
	drzewo *pom;
	pom = NULL;
	pom = *dodaj;
	printf("Podaj, do ktorego katalogu chcesz dodac rekord.\n");
	*dodaj = wybor(*dodaj);
	if (*dodaj != NULL)
	{
		if ((*dodaj)->dane != NULL)
		{
			lista *tmp;
			tmp = NULL;
			tmp = (*dodaj)->dane;
			while ((*dodaj)->dane->nast != NULL)
				(*dodaj)->dane = (*dodaj)->dane->nast;
			(*dodaj)->dane->nast = do_dodania;
			(*dodaj)->dane = tmp;
			tmp = NULL;
		}
		else
		{
			((*dodaj)->dane) = do_dodania;
		}
		*dodaj = pom;
		pom = NULL;
	}
	else
	{
		printf("Brak katalogow. Operacja dodawania rekordow niemozliwa.\n");
		*dodaj = pom;
		pom = NULL;
	}
	return;
}

void dodaj_z_pliku(drzewo **dodaj, char* nazwa_katalogu, lista *do_dodania)
{
	int pom1 = 0;
	drzewo *pom;
	pom = NULL;
	pom = *dodaj;
	*dodaj = wybor_plik(*dodaj, nazwa_katalogu);
	if (*dodaj != NULL)
	{
		if ((*dodaj)->dane != NULL)
		{
			lista *tmp;
			tmp = NULL;
			tmp = (*dodaj)->dane;
			while ((*dodaj)->dane->nast != NULL)
				(*dodaj)->dane = (*dodaj)->dane->nast;
			(*dodaj)->dane->nast = do_dodania;
			(*dodaj)->dane = tmp;
			tmp = NULL;
		}
		else
		{
			((*dodaj)->dane) = do_dodania;
		}
		*dodaj = pom;
		pom = NULL;
	}
	else
	{
		printf("Brak katalogow. Operacja dodawania rekordow niemozliwa.\n");
		*dodaj = pom;
		pom = NULL;
	}
	return;
}

/*-------------------(koniec) Dodawanie rekordu---------------------------------------*/
/*------------------------------------------------------------------------------------*/
int liczba_el(lista *przeglad)
{
	int pom;
	if (przeglad == NULL)
	{
		pom = 0;
	}
	else
	{
		pom = 1;
		while (przeglad->nast != NULL)
		{
			pom = pom + 1;
			przeglad = przeglad->nast;
		}
	}
	return pom;
}

void wstaw_lista(lista *d, lista **tablica, int *i)
{
	if (d != NULL)
	{
		if (d != NULL)
		{
			while (d != NULL)
			{
				tablica[*i] = d;
				*i = *i + 1;
				d = d->nast;
			}
		}
	}
	return;
}

lista * wybor_l(lista *przeglad)
{
	lista **tab;
	int n, n1;
	n = liczba_el(przeglad);
	n1 = n;
	tab = (lista **)malloc(n*sizeof(lista*));
	int i = 0;
	wstaw_lista(przeglad, tab, &i);
	i = 0;
	do	{
		for (i = 0; i < n - 1; i++)
		{
			if (strcmp(tab[i]->nazwa, tab[i + 1]->nazwa) > 0)
			{
				lista *pom;
        		pom = NULL;
        		pom = tab[i];
        		tab[i] = tab[i + 1];
        		tab[i + 1] = pom;
        		pom = NULL;
			}
		}
		n = n - 1;
	}
	while (n > 1);
	i = 0;
	for (i = 0; i < n1; i++)
	{
		printf("%d. Dane rekordu:\n", i);
		printf("   - nazwa: %s\n", tab[i]->nazwa);
		printf("   - czestotliwosc wydawania: %s\n", tab[i]->czest);
		printf("   - numer: %s\n", tab[i]->num);
	}
	scanf("%d", &i);
	lista *pom;
	pom = tab[i];
	i = 0;
	free(tab);
	return pom;
}

lista * wybor_l_przen(lista *przeglad, lista **poprz)
{
	lista **tab;
	int n, n1;
	n = liczba_el(przeglad);
	n1 = n;
	tab = (lista **)malloc(n*sizeof(lista*));
	int i = 0;
	wstaw_lista(przeglad, tab, &i);
	i = 0;
	for (i = 0; i < n1; i++)
	{
		printf("%d. Dane rekordu:\n", i);
		printf("   - nazwa: %s\n", tab[i]->nazwa);
		printf("   - czestotliwosc wydawania: %s\n", tab[i]->czest);
		printf("   - numer: %s\n", tab[i]->num);
	}
	scanf("%d", &i);
	lista *pom;
	pom = tab[i];
	if (i - 1 < 0)
	{
		*poprz = NULL;
	}
	else
	{
		*poprz = tab[i - 1];
	}
	free(tab);
	return pom;
}

/*--------------------------------------------Funkcje wyszukujace----------------------------------------------*/

void search_num(char* numer, drzewo *d, int* pom)			/*Funkcja zwróci 0, jeœli w bazie jest ten numer.*/
{
	lista *tmp;
	tmp = NULL;
	if (d != NULL)
	{
		if (d->dane != NULL)
		{
			tmp = d->dane;
		}
		while (d->dane != NULL)
		{
			if (strcmp(d->dane->num, numer) == 0)
			{
				*pom = 1;
				printf("Znaleziony element:\n");
				printf("  - nazwa katalogu: %s\n", d->nazwa_katalogu);
				printf("  - nazwa gazety: %s\n", d->dane->nazwa);
				printf("  - czestotliwosc wydawania: %s\n", d->dane->czest);
				printf("  - numer gazety: %s\n", d->dane->num);
			}
			d->dane = d->dane->nast;
		}
		d->dane = tmp;
		tmp = NULL;
		search_num(numer, d->lsyn, pom);
		search_num(numer, d->psyn, pom);
	}
	return;
}

int search_num2(char* numer, drzewo *d)			/*Funkcja zwróci 1, jeœli w bazie jest ten numer.*/
{
	int pom = 0;
	lista *tmp;
	tmp = NULL;
	if (d != NULL)
	{
		search_num2(numer, d->lsyn);
		if (d->dane != NULL)
		{
			tmp = d->dane;
		}
		while (d->dane != NULL)
		{
			if (strcmp(d->dane->num, numer) == 0)
			{
				pom = 1;
				break;
			}
			d->dane = d->dane->nast;
		}
		d->dane = tmp;
		tmp = NULL;
		search_num2(numer, d->psyn);
	}
	return pom;
}

void search_name(char* nazwa, drzewo *d, int* pom)
{
	lista *tmp;
	tmp = NULL;
	if (d != NULL)
	{
		search_name(nazwa, d->lsyn, pom);
		if (d->dane != NULL)
		{
			tmp = d->dane;
		}
		while ((d)->dane != NULL)
		{
			if (strcmp(d->dane->nazwa, nazwa) == 0)
			{
				*pom = 1;
				printf("Znaleziony element:\n");
				printf("  - nazwa katalogu: %s\n", d->nazwa_katalogu);
				printf("  - nazwa gazety: %s \n", d->dane->nazwa);
				printf("  - czestotliwosc wydawania: %s \n", d->dane->czest);
				printf("  - numer gazety: %s \n", d->dane->num);
			}
			d->dane = d->dane->nast;
		}
		d->dane = tmp;
		tmp = NULL;
		search_name(nazwa, d->psyn, pom);
	}
	return;
}

void search_name2(char* nazwa, drzewo *d, int* pom)
{
	lista *tmp;
	tmp = NULL;
	if (d != NULL)
	{
		search_name2(nazwa, d->lsyn, pom);
		if (d->dane != NULL)
		{
			tmp = d->dane;
		}
		while ((d)->dane != NULL)
		{
			if (strcmp(d->dane->nazwa, nazwa) == 0)
			{
				*pom = 1;
			}
			d->dane = d->dane->nast;
		}
		d->dane = tmp;
		tmp = NULL;
		search_name2(nazwa, d->psyn, pom);
	}
	return;
}

void search_freq(char* czest, drzewo *d, int* pom)
{
	lista *tmp;
	tmp = NULL;
	if (d != NULL)
	{
		if (d->dane != NULL)
		{
			tmp = d->dane;
		}
		while ((d)->dane != NULL)
		{
			if (strcmp(d->dane->czest, czest) == 0)
			{
				*pom = 1;
				printf("Znaleziony element:\n");
				printf("  - nazwa katalogu: %s\n", d->nazwa_katalogu);
				printf("  - nazwa gazety: %s \n", d->dane->nazwa);
				printf("  - czestotliwosc wydawania: %s \n", d->dane->czest);
				printf("  - numer gazety: %s \n", d->dane->num);
			}
			d->dane = d->dane->nast;
		}
		d->dane = tmp;
		tmp = NULL;
		search_freq(czest, d->lsyn, pom);
		search_freq(czest, d->psyn, pom);
	}
	return;
}

void wpisz(drzewo *d, FILE **plik)
{
	if (d != NULL)
	{
		wpisz(d->lsyn, plik);
		while (d->dane != NULL)
		{
			fprintf(*plik, "%s ", d->dane->nazwa);
			fprintf(*plik, "%s ", d->dane->num);
			fprintf(*plik, "%s ", d->dane->czest);
			fprintf(*plik, "%s", d->nazwa_katalogu);
			fprintf(*plik, "\n");
			d->dane = d->dane->nast;
		}
		wpisz(d->psyn, plik);
	}
	return;
}

void eksport_txt(drzewo *d, char *nazwa_p)					
{											
	FILE *wynik;
	if ((wynik = fopen(nazwa_p, "wt")) == NULL)
	{
		printf("Nie udalo sie stworzyc pliku wynikowego. Nastapil nieznany blad.\n");
		return;
	}
	wpisz(d, &wynik);
	fclose(wynik);
	return;
}

void wpisz_bin(drzewo *d, FILE **nazwa)
{
	if (d != NULL)
	{
		wpisz_bin(d->lsyn, nazwa);
		while (d->dane != NULL)
		{
			int licznik;
			licznik = liczba_el(d->dane);
			fseek(*nazwa, 1, SEEK_CUR);
			fwrite(d->dane, sizeof(lista), licznik, *nazwa);
			d->dane = d->dane->nast;
		}
		wpisz_bin(d->psyn, nazwa);
	}
	return;
}

void eksport_ula(drzewo *d, char *nazwa_p)
{
	FILE *bin;
	int i;
	if ((bin = fopen(nazwa_p, "ab+")) == NULL)
	{
		printf("Nie udalo sie stworzyc/odczytac pliku. Nastapil nieznany blad.\n");
		return;
	}
	wpisz(d, &bin);
	fclose(bin);
	return;
}

void import_txt(drzewo **d, char *nazwa_pliku)
{
	FILE *plik;
	if ((plik = fopen(nazwa_pliku, "rt")) == NULL)
	{
		printf("Nie udalo sie wczytac pliku wejsciowego. Nastapil nieznany blad.\n");
		return;
	}
	while (feof(plik) == 0)
	{
		char linia[100];
		fgets(linia, 100, plik);
		/*dodanie do bazy danych*/
		char nazwa_katalogu[15];
		char nazwa[21];
		char czest[13];
		char num[6];
		int i = 0;
		int j, k;
		for (i = 0; i < 100; i++)
		{
			if (isdigit(linia[i]) != 0)
				break;
			nazwa[i] = linia[i];
		}	/*dotarlismy do numeru gazety.*/
		k = i;
		for (k = i; k < 21; k++)
			nazwa[k] = '\0';
		j = 0;
		for (i; i < 100; i++)
		{
			if (isdigit(linia[i]) == 0)
				break;
			num[j] = linia[i];
			j = j + 1;
		}	/*dotarlismy do konca numeru gazety, poczatku czestotliwosci wydawania.*/
		i = i + 1;
		k = j;
		for (k = j; k < 6; k++)
			num[k] = '\0';
		j = 0;
		for (i; i < 100; i++)
		{
			if (isalpha(linia[i]) == 0)
				break;
			czest[j] = linia[i];
			j = j + 1;
		}	/*dotarlismy do nazwy katalogu.*/
		i = i + 1;
		k = j;
		for (k = j; k < 13; k++)
			czest[k] = '\0';
		j = 0;
		for (i; i < 100; i++)
		{
			if (isalpha(linia[i]) == 0)
				break;
			nazwa_katalogu[j] = linia[i];
			j = j + 1;
		}	/*dotarlismy do konca lini.*/
		k = j;
		for (k = j; k < 15; k++)
			nazwa_katalogu[k] = '\0';
		nowy_kat(d, nazwa_katalogu);
		int zm;
		search_name2(nazwa, *d, &zm);
		if (zm != 1)
		{
			lista *tmp;
			tmp = NULL;
			tmp = (lista *)malloc(sizeof(lista));
			strcpy(tmp->nazwa, nazwa);
			strcpy(tmp->czest, czest);
			strcpy(tmp->num, num);
			tmp->nast = NULL;
			dodaj_z_pliku(d, nazwa_katalogu, tmp);
			tmp = NULL;
		}
	}
	fclose(plik);
	return;
}

void import_ula(drzewo **d, char *nazwa_pliku)
{
	FILE *plik;
	if ((plik = fopen(nazwa_pliku, "ab+")) == NULL)
	{
		printf("Nie udalo sie odczytac pliku.\n");
		return;
	}
	fseek(plik, 0, SEEK_SET);
	lista *tmp;
	tmp = NULL;
	fread(tmp, sizeof(lista), 2, plik);
	fclose(plik);
	return;
}

/*-------------------------(poczatek) Przenies---------------------------------------*/

void przenies(drzewo **d_przen)
{
	drzewo *tmp, *tmp1;
	tmp = NULL;
	tmp1 = NULL;
	tmp = *d_przen;				/*zapamietanie ojca poddrzewa.*/
	tmp1 = tmp;					/*zapamietanie przekazanego poddrzewa do dalszych operacji.*/
	if (*d_przen != NULL)
	{
		int licz1 = 0;
		ile_wezlow(*d_przen, &licz1);
		if (licz1 >= 1)
		{
			printf("Wybierz katalog, z ktorego chcesz przeniesc rekord lub rekordy:\n");
			*d_przen = wybor(*d_przen);
			printf("Wybierz, czy chcesz przeniesc jeden, czy wiecej rekordow:\n");
			printf("  1. Jeden rekord.\n");
			printf("  2. Wiele rekordow.\n  ");
			char c[1];
			scanf("%1s", c);
			switch(c[0])
			{
				case '1':	{										
					if ((*d_przen)->dane != NULL)
					{
						lista *poprz;			/*poprzednik elementu listy (skad).*/
						lista *pocz_skad;
						pocz_skad = NULL;
						lista *pocz_dokad;
						lista *przenoszony;
						pocz_skad = (*d_przen)->dane;		/*zapamietanie poczatku listy (skad).*/
						printf("Podaj, ktory rekord chcesz przeniesc (podaj jego numer i zatwierdz klawiszem\nENTER):\n");
						(*d_przen)->dane = wybor_l_przen((*d_przen)->dane, &poprz);
						przenoszony = (*d_przen)->dane;			/*wybrany rekord*/
						if (poprz != NULL)
						{
							poprz->nast = (*d_przen)->dane->nast;
							(*d_przen)->dane = poprz;
							(*d_przen)->dane = pocz_skad;
							pocz_skad = NULL;
							poprz = NULL;
						}
						else
						{
							(*d_przen)->dane = (*d_przen)->dane->nast;
							pocz_skad = NULL;
							poprz = NULL;
						}
						(*d_przen) = tmp;				/*przywrocenie calego drzewa (skad).*/
						tmp = NULL;
						tmp = (*d_przen);				/*zapamietanie na nowo poczatka drzewa.*/
						printf("Podaj, do ktorego katalogu chcesz przeniesc rekord:\n");
						(*d_przen) = wybor(*d_przen);
						/*pom3->nast = NULL;*/			/*bo przenosimy jeden rekord.*/
						pocz_dokad = (*d_przen)->dane;		/*zapamietanie poczatku listy wezla (dokad).*/
						if ((*d_przen)->dane == NULL)
						{
							przenoszony->nast = NULL;
							(*d_przen)->dane = przenoszony;
							(*d_przen)->dane->nast = NULL;
							przenoszony = NULL;
							pocz_dokad = NULL;
						}
						else
						{
							while ((*d_przen)->dane->nast != NULL)
							{
								(*d_przen)->dane = (*d_przen)->dane->nast;
							}
							przenoszony->nast = NULL;
							(*d_przen)->dane->nast = przenoszony;
							przenoszony = NULL;
							(*d_przen)->dane = pocz_dokad;
							pocz_dokad = NULL;
						}
						(*d_przen) = tmp;
						tmp = NULL;
					}
					else
					{
						printf("Brak rekordow w wybranym katalogu. Operacja niemozliwa.\n");
						(*d_przen) = tmp;
						tmp = NULL;
					}
					break;
				}
				case '2':	{										/*powinno rowniez dzialac :P - edited 26.05.2015*/
					if ((*d_przen)->dane != NULL)
					{
						int licz1 = 0;
						lista *pocz_dokad, *do_dodania, *poprz, *pocz_przen;
						do_dodania = NULL;
						lista *pocz_skad;
						pocz_skad = (*d_przen)->dane;			/*zapamietanie poczatku listy skad*/
						printf("Podaj numer indeksu, przy ktorym stoi rekord, ktory chcesz przeniesc:\n");
						(*d_przen)->dane = wybor_l_przen((*d_przen)->dane, &poprz);
						do_dodania = (*d_przen)->dane;
						if (poprz != NULL)
						{
							(*d_przen)->dane = (*d_przen)->dane->nast;
							poprz->nast = (*d_przen)->dane;
							(*d_przen)->dane = poprz;
							(*d_przen)->dane = pocz_skad;
							poprz = NULL;
						}
						else
						{
							(*d_przen)->dane = (*d_przen)->dane->nast;
							pocz_skad = NULL;
						}
						do_dodania->nast = NULL;
						char koniec[1];
						printf("Czy chcesz przeniesc jeszcze inne rekordy z tego katalogu? T/N\n");
						licz1 = liczba_el((*d_przen)->dane);
						scanf("%1s", koniec);
						while (((koniec[0] == 't') || (koniec[0] == 'T')) && (licz1 > 0))
						{
							pocz_przen = do_dodania;
							lista *pocz_skad;				/*zapamietanie poczatka listy przenoszonej.*/
							pocz_skad = (*d_przen)->dane;
							while (do_dodania != NULL)
							{
								do_dodania = do_dodania->nast;
							}
							printf("Podaj numer indeksu, przy ktorym stoi rekord, ktory chcesz przeniesc:\n");
							(*d_przen)->dane = wybor_l_przen((*d_przen)->dane, &poprz);
							if (poprz != NULL)
							{
								do_dodania = (*d_przen)->dane;
								printf("nazwa: %s\n", do_dodania->nazwa);
								(*d_przen)->dane = (*d_przen)->dane->nast;
								poprz->nast = (*d_przen)->dane;
								(*d_przen)->dane = poprz;
								(*d_przen)->dane = pocz_skad;
								poprz = NULL;
									/*do_dodania->nast = NULL;*/
									/*pocz_skad = NULL;*/
							}
							else
							{
								do_dodania = (*d_przen)->dane;
								printf("nazwa: %s\n", do_dodania->nazwa);
								(*d_przen)->dane = (*d_przen)->dane->nast;
								pocz_skad = (*d_przen)->dane;
									/*do_dodania->nast = NULL;*/
							}
							do_dodania->nast = NULL;
							licz1 = liczba_el((*d_przen)->dane);
							if (licz1 >= 1)
							{
								printf("Czy chcesz przeniesc jeszcze inne rekordy z tego katalogu? T/N\n");
								fflush(stdin);
								/*while (getchar() != '\n');*/
								scanf("%1s", koniec);
							}
							else
								printf("Wyczerpano rekordy w tym katalogu. Brak dalszych mozliwosci wyboru.\n");
						}		/*end of while*/
						lista *tral;
						do_dodania = pocz_przen;
						tral = do_dodania;
						printf("Elementy przenoszone:\n");
						while (tral != NULL)
						{
							printf("- %s\n", tral->nazwa);
							tral = tral->nast;
						}
						(*d_przen) = tmp;
						printf("Podaj, do ktorego katalogu chcesz przeniesc rekordy:\n");
						(*d_przen) = wybor(*d_przen);
						if ((*d_przen)->dane == NULL)
						{
							(*d_przen)->dane = do_dodania;
							do_dodania = NULL;
						}
						else
						{
							lista *poczatek;
							poczatek = (*d_przen)->dane;
							while ((*d_przen)->dane->nast != NULL)
							{
								printf("Krok.\n");
								(*d_przen)->dane = (*d_przen)->dane->nast;
							}
							(*d_przen)->dane->nast = do_dodania;
							(*d_przen)->dane = poczatek;
							poczatek = NULL;
							/*do_dodania = NULL;*/
						}
						printf("Operacja zakonczona pomyslnie.\n");
						(*d_przen) = tmp;
						tmp = NULL;
					}
					else
					{
						printf("Brak rekordow w wybranym katalogu. Operacja niemozliwa.\n");
						(*d_przen) = tmp;
						tmp = NULL;
					}
					break;
				}
				default:	{
					printf("Nie rozpoznano polecenia.\n");
					tmp = NULL;
					break;
				}
			}
		}
		else
		{
			printf("Brak wystarczajacej liczby katalogow. Operacja niemozliwa. \n");
			tmp = NULL;
		}
	}
	else
	{
		printf("Brak katalogow. Opercja niemozliwa.\n");
		tmp = NULL;
	}
	return;
}

/*-----------------------(koniec) Przenies-------------------------------------------*/

void rozroznienie(char *nazwa_pliku, char *pom)				
{
	printf("Podaj nazwe pliku z rozszerzeniem:\n");
	fflush(stdin);
	while (getchar() != '\n');
	fgets(nazwa_pliku, 101, stdin);
	nazwa_pliku[strlen(nazwa_pliku) - 1] = '\0';
	int i = 0;
	for (i = 0; i < strlen(nazwa_pliku); i++)
	{
		if (isalpha(nazwa_pliku[i]) == 0)
			break;
	}
	int zmienna;
	if (i == strlen(nazwa_pliku) - 4)
		zmienna = 1;
	else
		zmienna = 0;
	while (zmienna == 0)				/*zabezpieczenie na wypadek, gdyby uzytkownik nie podal rozszerzenia pliku.*/
	{
		printf("Podano nazwe bez rozszerzenia. Sprobuj ponownie.\n");
		printf("Podaj nazwe pliku z rozszerzeniem:\n");
		fflush(stdin);
		fgets(nazwa_pliku, 101, stdin);
		nazwa_pliku[strlen(nazwa_pliku) - 1] = '\0';
		int i = 0;
		for (i = 0; i < strlen(nazwa_pliku); i++)
		{
			if (isalpha(nazwa_pliku[i]) == 0)
				break;
		}
		if (i == strlen(nazwa_pliku) - 4)
			zmienna = 1;
		else
			zmienna = 0;
	}
	int j = 0;
	while (j < 6)
	{
		pom[j] = nazwa_pliku[i];
		i = i + 1;
		j = j + 1;
	}
	return;
}

