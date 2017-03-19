Na fali sukcesu Horror Movie postanowiono wyprodukować sequel Horror Movie 2.

Znowu jesteś scenarzystą horroru. I znowu musisz stworzyć program, który
symuluje potencjalną fabułę i dostarczy Ci pomysłów do Twojego arcydzieła.
Jednak tym razem masz użyć innych technologii do realizacji swojego programu.

Akcja dzieje się w małym miasteczku w stanie Maine. Miasteczko zostaje
nawiedzone przez tajemniczą bestię. Ponieważ nie masz koncepcji na konkretnych
bohaterów i potwora będącego głównym zagrożeniem, musisz przygotować
rozwiązanie ogólne.

Stwórz klasy opisane poniżej oraz odpowiednie klasy pomocnicze, które ułatwią
implementację oraz umożliwią łatwą rozbudowę programu.

= Plik citizen.h oraz citizen.cc =

Klasy Adult, Teenager, Sheriff, w których używane są typy pomocnicze:
* HealthPoints będący typem przechowującym punkty życia,
* Age będący typem reprezentującym wiek,
* AttackPower reprezentujący w przypadku szeryfa siłę ataku.

Klasy Adult, Teenager przyjmują w konstruktorze HealthPoints health oraz
Age age, a Sheriff dodatkowo AttackPower attackPower. Klasy Adult i Sheriff
mogą przyjmować age w zakresie od 18 do 100, a Teenager w zakresie od 11 do 17.

Klasy Adult, Teenager, Sheriff udostępniają metody publiczne:
* HealthPoints getHealth() – zwraca liczbę punktów życia mieszkańca,
* Age getAge() – zwraca wiek mieszkańca,
* void takeDamage(AttackPower damage) – zmniejsza liczbę punktów życia
  mieszkańca o damage, ale nie więcej niż mieszkaniec ma aktualnie.

Klasa Sheriff ma dodatkowo metodę publiczną:
* AttackPower getAttackPower() – zwraca siłę ataku szeryfa.

Klasy Adult, Teenager, Sheriff powinny być podklasami ogólniejszego typu
Citizen.

Dodatkowo powinny istnieć funkcje fabrykujące dla Adult, Teenager, Sheriff
z sygnaturami pasującymi do przykładu.

= Plik monster.h oraz monster.cc =

Klasy Zombie, Vampire, Mummy, w których używane są typy pomocnicze:
* HealthPoints będący typem przechowującym punkty życia,
* AttackPower reprezentujący siłę ataku.

Klasy Zombie, Vampire i Mummy przyjmują w konstruktorze HealthPoints health
oraz AttackPower attackPower.

Klasy Vampire, Zombie i Mummy mają metody publiczne:
Health getHealth() – zwraca liczbę punktów życia potwora,
AttackPower getAttackPower() – zwraca siłę ataku potwora,
void takeDamage(AttackPower damage) – zmniejsza liczbę punktów życia potwora
o damage, ale nie więcej niż potwór ma aktualnie.

Klasy Zombie, Vampire i Mummy powinny być podklasami ogólniejszego typu Monster.

W sequelu potwory mogą również atakować grupowo. Należy zaimplementować
odpowiednią klasę do ataku grupowego GroupOfMonsters, która w konstruktorze
przyjmuje wektor (std::vector) albo listę inicjującą (std::initializer_list)
zawierającą potwory, które są w grupie. GroupOfMonsters mają metody publiczne:
Health getHealth() – zwraca liczbę punktów życia grupy (suma punktów życia
żywych potworów),
AttackPower getAttackPower() – zwraca siłę ataku grupy (suma sił ataku żywych
potworów),
void takeDamage(AttackPower damage) – zmniejsza liczbę punktów życia każdego
potwora w grupie o damage, ale nie więcej niż potwór ma aktualnie.

Dodatkowo powinny istnieć funkcje fabrykujące dla Zombie, Vampire, Mummy,
GroupOfMonsters z sygnaturami pasującymi do przykładu.

= Plik smalltown.h oraz smalltown.cc =

Klasa SmallTown tworzona za pomocą klasy Builder, dla której można ustawić co
najmniej:
* potwora, który atakuje miasto,
* czas startowy t0 typu Time,
* czas maksymalny t1 typu Time,
* obywateli miasteczka.

Klasa SmallTown ma metody publiczne:
Status getStatus() – zwraca nazwę typu potwora, liczbę jego punktów życia oraz
liczbę pozostałych przy życiu mieszkańców;
void tick(Time timeStep) – na początku sprawdza aktualny czas; jeśli jest to czas
ataku, to następuje atak na wszystkich mieszkańców; na koniec czas w miasteczku
przesuwa się o timeStep.

SmallTown operuje zgodnie z wewnętrznym zegarem. Czas liczony jest w godzinach,
od godziny 0 do godziny t1 i potem znów od godziny 0, i tak cyklicznie. Pierwsze
odliczanie zaczyna się od godziny t0. Potwór atakuje tylko o godzinach
podzielnych  przez 3 lub 13, a niepodzielnych przez 7.
Śmierć mieszkańca bądź potwora następuje w momencie, gdy jego liczba punktów
życia osiągnie zero. Wywołanie tick() na miasteczku z martwym potworem i co
najmniej jednym żywym mieszkańcem powoduje wypisanie na standardowe wyjście
komunikatu "CITIZENS WON". Wywołanie tick() na miasteczku z martwymi
mieszkańcami i żywym potworem powoduje wypisanie na standardowym wyjściu napisu
"MONSTER WON". Jeśli zarówno mieszańcy, jak i potwór są martwi, to tick()
wypisuje na standardowe wyjście "DRAW".

= Inne wymagania =

Należy zaprojektować metodę lub metody ataku na ofiarę tak, aby spełnić
wymagania funkcjonalne. Miejsce umieszczenia tej metody zależy od projektanta.
Potwór wykonuje atak na ofiarę, zabierając jej punkty życia. Należy też
uwzględnić specjalny przypadek, gdy atakowany jest Sheriff – wtedy atak
następuje w dwie strony – punkty życia traci zarówno szeryf, jak i potwór.

Strategia godzin ataków potworów na miasto powinna być łatwo podmienialna na
inną, zgodnie z zasadą Open-Closed Principle z zasad SOLID.

Przykład użycia znajduje się w pliku horror_example.cc.

Rozwiązanie powinno zawierać następujące pliki nagłówkowe: citizen.h,
monster.h, smalltown.h oraz opcjonalne pliki: citizen.cc, monster.cc,
smalltown.cc, a także opcjonalne pliki pomocnicze: helper.h oraz helper.cc.
Pliki te należy umieścić w repozytorium w katalogu

grupaN/zadanie6/ab123456+cd123456

lub

grupaN/zadanie6/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami
członków zespołu umieszczającego to rozwiązanie.
Katalog z rozwiązaniem nie powinien zawierać innych plików, ale może zawierać
podkatalog private, gdzie można umieszczać różne pliki, np. swoje testy. Pliki
umieszczone w tym podkatalogu nie będą oceniane.
