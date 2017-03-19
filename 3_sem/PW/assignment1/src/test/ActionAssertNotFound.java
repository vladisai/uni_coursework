package test;

import gra.Akcja;
import gra.Postać;

import static org.junit.Assert.fail;

/**
 * Created by vlad on 08.12.16.
 */
public class ActionAssertNotFound implements Akcja {
        @Override
        public void wykonaj(Postać postać) {
            fail("A shape was found when no shape was expected");
        }
}
