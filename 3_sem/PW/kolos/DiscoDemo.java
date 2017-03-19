import java.util.Random;

class Disco {

    private static Random random = new Random();

    private static void sleepRandom() {
        try {
            Thread.sleep(random.nextInt(5000));
        } catch (Exception e) {
            // Do nothing
        }
    }

    Object notEmpty = new Object();
    Object empty = new Object();
    Object dancefloor = new Object();
    Object firstMale = new Object();
    Object firstFemale = new Object();
    Object males = new Object();
    Object females = new Object();
    Object readyToDanceMale = new Object();
    Object readyToDanceFemale = new Object();

    int malesWaiting = 0;
    int femalesWaiting = 0;
    int dancing = 0;
    int maxDancing = 10;
    int firstMaleID = -1;
    int firstFemaleID = -1;

    public int waitForPartnerAndSpot(int id, int gender) {
        try {
            if (gender == 1) {
                System.out.println("" + id + " is at the floor");
                if (femalesWaiting > 0) {
                    System.out.println("" + id + " is waiting for her turn");
                    femalesWaiting++;
                    synchronized (females) {
                        females.wait();
                    }
                    femalesWaiting--;
                }
                System.out.println("" + id + " is next to be served");
                System.out.println("" + malesWaiting + " males are waiting");
                if (malesWaiting == 0) {
                    System.out.println("" + id + " waiting for her man");
                    femalesWaiting++;
                    synchronized (firstFemale) {
                        firstFemale.wait();
                    }
                    femalesWaiting--;
                    firstFemaleID = id;
                    System.out.println("" + id + " stopped waiting for her man");
                    if (dancing + 2 > maxDancing) {
                        System.out.println("" + id + " waiting for her dancefloor");
                        synchronized (dancefloor) {
                            dancefloor.wait();
                        }
                        dancing += 2;
                    }
                    synchronized (readyToDanceMale) {
                        readyToDanceMale.notify();
                    }
                } else {
                    System.out.println("" + id + " waking up a guy");
                    firstFemaleID = id;
                    synchronized (firstMale) {
                        firstMale.notify();
                    }
                    synchronized (readyToDanceFemale) {
                        readyToDanceFemale.wait();
                    }
                }
                if (femalesWaiting > 0)
                    synchronized (females) {
                        females.notify();
                    }
                return firstMaleID;
            } else {
                System.out.println("" + id + " is at the floor");
                if (malesWaiting > 0) {
                    malesWaiting++;
                    synchronized (males) {
                        males.wait();
                    }
                    malesWaiting--;
                }
                System.out.println("" + id + " is next to be served");
                System.out.println("" + malesWaiting + " females are waiting");
                if (femalesWaiting == 0) {
                    System.out.println("" + id + " waiting for a girl");
                    malesWaiting++;
                    synchronized (firstMale) {
                        firstMale.wait();
                    }
                    malesWaiting--;
                    firstMaleID = id;
                    if (dancing + 2 > maxDancing) {
                        System.out.println("" + id + " has found a girl, but needs space on the dancefloor");
                        synchronized (dancefloor) {
                            dancefloor.wait();
                        }
                        dancing += 2;
                        if (dancing == 2)
                            synchronized (notEmpty) {
                                notEmpty.notify();
                            }
                    }
                    synchronized (readyToDanceFemale) {
                        readyToDanceFemale.notify();
                    }
                } else {
                    System.out.println("" + id + " waking up a girl");
                    firstMaleID = id;
                    synchronized (firstFemale) {
                        firstFemale.notify();
                    }
                    synchronized (readyToDanceMale) {
                        readyToDanceMale.wait();
                    }
                }
                if (malesWaiting > 0)
                    synchronized (males) {
                        males.notify();
                    }
                return firstFemaleID;
            }
        } catch (Exception e) {
            System.out.println(e.toString());
            return -5;
        }
    }

    public synchronized void stopDancing() {
        try {
            dancing--;
            if (dancing + 2 < maxDancing)
                dancefloor.notify();
            if (dancing == 0) {
                empty.notify();
            }
        } catch (Exception e) {

        }
    }

    public synchronized void waitForNonEmpty() {
        try {
            if (dancing == 0) {
                notEmpty.wait();
            }
        } catch (Exception e) {

        }
    }

    public synchronized void waitForEmpty() {
        try {
            if (dancing > 0)
                empty.wait();
        } catch (Exception e) {

        }
    }

    public void turnOnTheMusic() {
        //System.out.println("The music is on");
    }

    public void turnOffTheMusic() {
        //System.out.println("The music is off");
    }

    public void dance(int i1, int i2) {
        System.out.println("" + i1 + " and " + i2 + " are dancing");
        sleepRandom();
    }

}

class Dancer implements Runnable {
    Disco d;
    int id;
    int gender;

    Dancer(Disco d, int id, int gender) {
        this.d = d;
        this.id = id;
        this.gender = gender;
    }

    @Override
    public void run() {
        while (true) {
            System.out.println("" + id + " is waiting for parnter");
            int partner = d.waitForPartnerAndSpot(id, gender);

            d.dance(id, partner);

            System.out.println("" + id + " has stopped dancing");
            d.stopDancing();
        }
    }
}

class DJ implements Runnable {
    Disco d;

    DJ(Disco d) {
        this.d = d;
    }

    @Override
    public void run() {
        while (true) {
            d.waitForNonEmpty();
            d.turnOnTheMusic();
            d.waitForEmpty();
            d.turnOffTheMusic();
        }
    }
}

public class DiscoDemo {
    public static void main(String[] args) {
        Disco d = new Disco();
        int n = 10;
        new Thread(new DJ(d)).start();
        for (int i = 0; i < n; i++) {
            new Thread(new Dancer(d, i, i % 2)).start();
        }
    }

}
