package test;

import static org.junit.Assert.fail;

/**
 * Created by vlad on 08.12.16.
 */
public class RunnableAssertFound implements Runnable {

    @Override
    public void run() {
        fail("A shape was expected, nothing was found at this position.");
    }
}
