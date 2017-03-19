package test;

import gra.DeadlockException;
import gra.Kierunek;
import gra.MojaPlansza;
import gra.Shape;
import org.junit.Ignore;
import org.junit.Test;

import java.util.*;

import static org.junit.Assert.*;

/**
 * Created by vlad on 08.12.16.
 */
public class BoardTest {

    private void assertNotFree(MojaPlansza plansza, int row, int column) {
        plansza.sprawdź(row, column, new ActionAssertFound(), new RunnableAssertFound());
    }

    private void assertFree(MojaPlansza plansza, int row, int column) {
        plansza.sprawdź(row, column, new ActionAssertNotFound(), new RunnableAssertNotFound());
    }

    @Test
    public void initializeTest() {
        MojaPlansza board = new MojaPlansza(10, 10);
        assertEquals(10, board.getHeight());
        assertEquals(10, board.getWidth());
    }

    @Test(expected = IllegalArgumentException.class)
    public void initializeTestException() {
        MojaPlansza board = new MojaPlansza(-10, 10);
        assertEquals(10, board.getHeight());
        assertEquals(10, board.getWidth());
    }

    @Test(expected = IllegalArgumentException.class)
    public void putTest() throws InterruptedException {
        MojaPlansza board = new MojaPlansza(10, 10);
        try {
            board.postaw(new Shape(10, 10), 0, 0);
        } catch (Exception e) {
            fail("No exception expected: " + e.toString());
        }
        assertNotFree(board, 0, 0);
        board.sprawdź(1, 1, new ActionAssertFound(), new RunnableAssertFound());
        board.postaw(new Shape(10, 10), 0, 1);
    }

    @Test
    public void removeTest() throws InterruptedException {
        MojaPlansza board = new MojaPlansza(10, 10);
        Shape shape = new Shape(10, 10);
        board.postaw(shape, 0, 0);
        assertNotFree(board, 0, 0);
        board.usuń(shape);
        assertFree(board, 0, 0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void removeIllegalArgumentTest() throws InterruptedException {
        MojaPlansza board = new MojaPlansza(10, 10);
        board.postaw(new Shape(3, 3), 0, 0);
        board.usuń(new Shape(10, 10));
    }

    @Test
    public void moveTest() throws InterruptedException, DeadlockException {
        MojaPlansza board = new MojaPlansza(10, 10);
        Shape shape = new Shape(3, 3);
        board.postaw(shape, 1, 1);
        assertNotFree(board, 1, 1);
        assertFree(board, 1, 4);
        board.przesuń(shape, Kierunek.PRAWO);
        assertFree(board, 1, 1);
        assertNotFree(board, 1, 4);
    }

    @Test(expected = IllegalArgumentException.class)
    public void moveIllegalArgumentTest() throws InterruptedException, DeadlockException {
        MojaPlansza board = new MojaPlansza(10, 10);
        Shape shape = new Shape(10, 10);
        board.postaw(shape, 0, 0);
        board.przesuń(shape, Kierunek.GÓRA);
    }

    @Test(expected = DeadlockException.class)
    public void deadlockTest() throws InterruptedException, DeadlockException {
        MojaPlansza board = new MojaPlansza(10, 10);
        Shape shape1 = new Shape(3, 1);
        Shape shape2 = new Shape(4, 1);
        board.postaw(shape1, 0, 0);
        board.postaw(shape2, 1, 1);
        Runnable shape1Mover = new Runnable() {
            @Override
            public void run() {
                try {
                    board.przesuń(shape1, Kierunek.PRAWO);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (DeadlockException e) {
                    e.printStackTrace();
                }
            }
        };
        new Thread(shape1Mover).start();
        Thread.sleep(1000); // we don't want to get an exception in a different thread
        board.przesuń(shape2, Kierunek.LEWO);
    }

    @Test
    public void waitTest() throws InterruptedException, DeadlockException {
        MojaPlansza board = new MojaPlansza(10, 10);
        Shape shape1 = new Shape(3, 1);
        Shape shape2 = new Shape(4, 1);
        board.postaw(shape1, 0, 0);
        board.postaw(shape2, 1, 1);
        Runnable shape1Mover = new Runnable() {
            @Override
            public void run() {
                try {
                    board.przesuń(shape1, Kierunek.PRAWO);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (DeadlockException e) {
                    e.printStackTrace();
                }
            }
        };
        new Thread(shape1Mover).start();
        Thread.sleep(1000); // we don't want to get an exception in a different thread
        board.przesuń(shape2, Kierunek.PRAWO);
    }

    @Test
    public void waitTest2() throws InterruptedException, DeadlockException {
        MojaPlansza board = new MojaPlansza(2, 2);
        Shape shape1 = new Shape(1, 1);
        Shape shape2 = new Shape(1, 1);
        board.postaw(shape1, 0, 0);
        board.postaw(shape2, 0, 1);
        Runnable shape1Mover = new Runnable() {
            @Override
            public void run() {
                try {
                    board.przesuń(shape1, Kierunek.PRAWO);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (DeadlockException e) {
                    e.printStackTrace();
                }
            }
        };
        new Thread(shape1Mover).start();
        Thread.sleep(1000); // we don't want to get an exception in a different thread
        board.przesuń(shape2, Kierunek.DÓŁ);
    }


    @Ignore // unpredictable
    @Test
    public void randomTest() throws InterruptedException {
        // configuration
        final int shapesCount = 10;
        final int maxHeight = 10;
        final int maxWidth = 10;
        final int boardHeight = 100;
        final int boardWidth = 100;
        final int opsPerThread = 100;

        MojaPlansza board = new MojaPlansza(boardHeight, boardWidth);
        Random random = new Random();
        List<Shape> shapes = new ArrayList<>();
        List<Thread> threads = new LinkedList<>();
        for (int i = 0; i < shapesCount; i++) {
            Runnable runnable = new Runnable() {
                @Override
                public void run() {
                    int height = 1 + random.nextInt(maxHeight);
                    int width = 1 + random.nextInt(maxWidth);
                    Shape shape = new Shape(height, width);
                    int row = random.nextInt(boardHeight - height);
                    int column = random.nextInt(boardWidth - width);
                    try {
                        board.postaw(shape, row, column);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (IllegalArgumentException e) {
                        // Do nothing
                    }
                    synchronized (shapes) {
                        shapes.add(shape);
                    }
                    for (int j = 0; j < opsPerThread; j++) {
                        System.out.println("op + " + j);
                        try {
                            Thread.sleep(random.nextInt(200));
                            Shape randomShape = null;
                            synchronized (shapes) {
                                randomShape = shapes.get(random.nextInt(shapes.size()));
                            }
                            Kierunek randomDirection = Kierunek.values()[random.nextInt(Kierunek.values().length)];
                            System.out.println("moving " + j);
                            board.przesuń(randomShape, randomDirection);
                            System.out.println("moved " + j);
                        } catch (InterruptedException e) {
                            // do nothing
                        } catch (DeadlockException e) {
                            System.out.println("deadlock");
                        } catch (IllegalArgumentException e) {
                            // Do nothing
                        }
                        System.out.println("end " + j + " / " + opsPerThread);
                    }
                }
            };
            Thread thread = new Thread(runnable);
            thread.start();
            threads.add(thread);
        }
        for (Thread t : threads)
            t.join();
    }
}
