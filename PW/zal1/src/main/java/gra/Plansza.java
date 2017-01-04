package gra;

public interface Plansza {

    void postaw(Postać postać, int wiersz, int kolumna)
            throws InterruptedException;

    void przesuń(Postać postać, Kierunek kierunek)
            throws InterruptedException, DeadlockException;

    void usuń(Postać postać);

    void sprawdź(int wiersz, int kolumna,
            Akcja jeśliZajęte, Runnable jeśliWolne);

}
