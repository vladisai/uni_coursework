# Zadanie MFS

Jesteś programistą w dużej, międzynarodowej korporacji zajmującej się m.in.:

- rozwijaniem systemu plików MFS (Minix File System),
- wirtualizacją.

Powyższymi zadaniami zajmują się osobne działy. Zarząd postawił na synergię
pomiędzy nimi. Zadaniem twojego zespołu jest wprowadzenie w życie zaleceń
zarządu. Podczas burzy mózgów padła propozycja zoptymalizowania MFS, tak aby:

- zmniejszyć ilość miejsca zajmowanego przez maszyny wirtualne,
- przyśpieszyć ich działanie.

Programiści mają wspaniały pomysł, jak to osiągnąć. Twoim zadaniem będzie:

- dostarczenie łatki na MFS,
- opisanie wyników i testów w formie publikacji naukowej.


## Łatka

Obrazy maszyn wirtualnych w dużej części są puste. MFS, gdy dostanie do
zapisania blok składający się z samych zer, zapisze cały blok na dysk.
Głównym pomysłem na optymalizację przechowywania maszyn wirtualnych jest
sprawienie, że nie będziemy zapisać na dysk bloków składający się z samych zer.
Wykonywanie kopii dużego pliku również powinno być szybkie (kopiowane powinny
być tylko niezerowe bloki).

Podczas modyfikowania systemu plików przyświeca nam zasada: *można oszukiwać,
ale nie można dać się złapać*, dlatego trzeba zapewnić, że dotychczasowa
funkcjonalność działa bez zmian. Na przykład system plików musi pokazywać
prawidłowy rozmiar pliku, mimo że fizycznie plik zajmuje mniej miejsca na dysku
i zawiera metadane.

Należy przygotować łatkę ze zmianami w katalogu /usr/src/ MINIX-a.
W tym celu należy użyć polecenia diff z parametrami -rupN.
Obraz, na którym będzie testowane rozwiązanie, znajduje się w pliku
/home/students/inf/PUBLIC/SO/scenariusze/4/minix.img.
Rozwiązania wysyła się na sprawdzarkę pod adresem http://students.mimuw.edu.pl:15880.
Po wysłaniu rozwiązania sprawdzarka automatycznie nałoży zmiany za pomocą komendy

```bash
cd /usr/src/
patch -p1 < ab123456.patch
cd /usr/src/minix/
make && make install
```

wykona testy i odeśle wynik.


## Opis rozwiązania i testy wydajnościowe

Twoja firma jest zadowolona z wykonanego zadania i chce się nim pochwalić na
konferencji. Należy przygotować opis rozwiązania, przygotować testy
wydajnościowe oraz przeprowadzić dyskusję uzyskanych wyników. Pytania, na jakie
chcemy uzyskać odpowiedzi, to:

- czy opłaca się wprowadzić taką funkcjonalność,
- dla jakich plików się opłaca,
- czy rzeczywiście w ten sposób będzie szybciej, a jeśli nie, to dlaczego?

Publikację należy przygotować w taki sposób, aby nadawała się do zgłoszenia na
konferencji SYSTOR, patrz [Full Papers Track](https://www.systor.org/2017/cfp.html).


## Oddawanie rozwiązania


Plik z łatką o nazwie ab123456.path, plik z opisem o nazwie ab123456.pdf oraz
kod źródłowy testów należy umieścić w repozytorium SVN w katalogu

studenci/ab123456/zadanie7

gdzie ab123456 jest identyfikatorem studenta używanym do logowania
w laboratorium komputerowym.

Pierwszy termin upływa 1.06.2017 o godz. 19.
Drugi termin upływa 28.08.2017 o godz. 19.


## Ocena

2 pkt. – testy automatyczne
2 pkt. – przygotowanie testów wydajnościowych rozwiązania
1 pkt  – opis rozwiązania
1 pkt  – prezentacje wyników testów wydajnościowych, np. w postaci tabelki lub wykresu
1 pkt  – poprawność i powtarzalność eksperymentów
1 pkt  – jasność opisu i zgodność z formatem

Jeśli rozwiązanie z testów automatycznych otrzymuje 0 pkt., to ocena jest 0 pkt.

Próba zapchania lub zablokowania na students portów 15880 i 15881, z których
korzysta sprawdzarka, spowoduje jej wyłączenie, a testy automatyczne zostaną
wykonane dopiero po terminie oddania.

Wszelkie dwuznaczności w treści są rozwiązywane na korzyść studenta, o ile testy
automatyczne przechodzą, a opis zastosowanego rozwiązania został zawarty
w publikacji.

Pytania można zadawać na adres k.wegrzycki@mimuw.edu.pl od 5.05.2017 do 24.05.2017.
