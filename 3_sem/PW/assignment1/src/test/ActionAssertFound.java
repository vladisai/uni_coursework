package test;

import gra.Akcja;
import gra.Postać;

/**
 * Created by vlad on 08.12.16.
 */
public class ActionAssertFound implements Akcja {
    @Override
    public void wykonaj(Postać postać) {
        // Do nothing. A shape was found, just as expected.
    }
}
