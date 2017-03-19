package test;

import gra.Akcja;
import gra.Postać;

import static org.junit.Assert.assertEquals;

/**
 * Created by vlad on 08.12.16.
 */
public class ActionAssertEquals implements Akcja {
    private Postać shape;

    ActionAssertEquals(Postać shape) {
        this.shape = shape;
    }

    @Override
    public void wykonaj(Postać postać) {
        assertEquals("The shape found is different from the expected one", shape, postać);
    }
}
