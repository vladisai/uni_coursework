import java.util.Random;
import java.util.List;
import java.util.LinkedList;
import java.util.Iterator;

interface Bag<T> {
    void put(T value);
    void get(T value) throws InterruptedException;
}

class Backpack<T> implements Bag<T> {

    private List<T> contains = new LinkedList<>();

    @Override
    public synchronized void put(T value) {
        System.out.println("Putting the value " + value);
        contains.add(value);
        notifyAll();
        System.out.println("Notified all");
    }

    @Override
    public synchronized void get(T value) throws InterruptedException {
        boolean hasFound = false;
        while (!hasFound) {
            Iterator<Integer> it = contains.iterator();
            System.out.println("Searching for value " + value);
            while (it.hasNext()) {
                Integer currentValue = it.next();
                if (currentValue.equals(value)) {
                    it.remove();
                    hasFound = true;
                    System.out.println("Found " + value + "! Took it out.");
                    break;
                }
            }
            if (!hasFound) {
                System.out.println("Didn't find " + value + ". Waiting...");
                wait();
            }
        }
    }
}

class BackpackUser implements Runnable {

    private static Random random = new Random();

    private static void sleepRandom() {
        try {
            Thread.sleep(random.nextInt(5000));
        } catch (Exception e) {
            // Do nothing
        }
    }

    private Bag<Integer> bag;

    BackpackUser(Bag<Integer> bag) {
        this.bag = bag;
    }

    @Override
    public void run() {
        sleepRandom();
        Integer value = random.nextInt(5);
        if (random.nextBoolean()) {
            System.out.println("Trying to put value " + value);
            bag.put(value);
        } else {
            System.out.println("Trying to get value " + value);
            try {
                bag.get(value);
            } catch (Exception e) {
                // Do nothign
            }
        }
    }
}

public class Problem1 {
    public static void main(String[] args) {
        Bag<Integer> bag = new Backpack();
        for (int i = 0; i < 10; i++) {
            new Thread(new BackpackUser(bag)).start();
        }
    }
}
