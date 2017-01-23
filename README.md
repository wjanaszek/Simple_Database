# Simple_Database
Simple newspapper database - console app written in C.

Aplikacja zawiera niestety jeden poważny błąd - usuwanie elementu z drzewa binarnego nie jest zaimplementowane do końca prawidłowo. Błąd ten wynika ze stworzenia nie do końca pełnej struktury drzewa - zabrakło wskazania na "rodzica" danego węzła. Później w toku studiów na przedmiocie "Algorytmy i struktury danych" zaimplementowałem tą procedurę. Jest to projekt TreeMap.

1. Objaśnienia:
  - struktura danych to drzewo binarne z listą jednokierunkową w każdym węźle tego drzewa
2. Funkcjonalność:
  - odczyt/zapis stanu bazy do pliku tekstowego lub binarnego
  - dodawanie/usuwanie/modyfikacja danych
  - wyświetlanie (wyszukiwanie) danych np. na podstawie częstotliwości wydawania gazety
  
