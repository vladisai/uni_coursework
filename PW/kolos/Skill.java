import java.util.Random;
import java.util.concurrent.Semaphore;

public class Skill {

    private static Random random = new Random();

    private static void sleepRandom() {
        try {
            Thread.sleep(random.nextInt(5000));
        } catch (Exception e) {
            // Do nothing
        }
    }

    Semaphore mutex = new Semaphore(1);
    Semaphore queue = new Semaphore(0);
    Semaphore employees = new Semaphore(1);
    Semaphore group = new Semaphore(0);
    Semaphore clients = new Semaphore(1);
    Semaphore clientGroup = new Semaphore(0);

    int M = 14;
    int N = 20;
    int C = 4;

    int clientsWaiting = 0;
    int currentClient = 0;
    int currentGroup = -1;
    int groupCounter = 1;
    int totalSkill = 0;

    public void init(int empId, int groupId) {

    }

    public void add(int groupId, int empId) {

    }

    public void work(int clientId) {

    }

    public void service(int groupId) {

    }

    private class Client implements Runnable {

        private int id;

        Client(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    mutex.acquire();
                    if (clientsWaiting > 0) {
                        System.out.println("There are clients waiting. joining the line " + id);
                        clientsWaiting++;
                        mutex.release();
                        clients.acquire();
                        clientsWaiting--;
                    }

                    System.out.println("almost getting served " + id);
                    currentClient = id;
                    mutex.release();
                    employees.release();
                    clientGroup.acquire();

                    System.out.println("Being served " + id + " by group " + currentGroup);
                    int g = currentGroup;
                    currentGroup = -1;
                    currentClient = -1;
                    mutex.release();

                    service(g);

                    mutex.acquire();
                    System.out.println("" + id + " waking up next clients if there are any");
                    if (clientsWaiting > 0) {
                        System.out.println("Being served " + id + " by group " + currentGroup);
                        clients.release();
                    } else {
                        mutex.release();
                    }
                }
            } catch (Exception e) {
                // do nothignn
            }
        }
    }

    private class Employee implements Runnable {

        private int id;
        private int skill;

        Employee(int id, int skill) {
            this.id = id;
            this.skill = skill;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    System.out.println("Another working day for " + id);
                    mutex.acquire();
                    if (clientsWaiting == 0 && currentClient == -1) {
                        System.out.println("No job to do, falling asleep " + id);
                        mutex.release();
                        employees.acquire();
                        mutex.acquire();
                    }
                    System.out.println("" + id + " has found a client " + currentClient);
                    if (currentGroup == -1) {
                        currentGroup = groupCounter++;
                        System.out.println("" + id + " is creating a group " + currentGroup + " for " + currentClient);
                        init(id, currentGroup);
                    } else {
                        System.out.println("" + id + " is joining a group " + currentGroup + " for " + currentClient);
                        add(currentGroup, id);
                    }
                    totalSkill += skill;
                    if (totalSkill < M) {
                        System.out.println("" + id + " is waking up the next employee");
                        mutex.release();
                        employees.release();
                        //System.out.println("" + id + " is working with " + currentClient);
                    } else {
                        System.out.println("" + id + " is letting the client " + currentClient + "know that everyone is good ");
                        totalSkill = 0;
                        clientGroup.release();
                    }
                }
            } catch (Exception e) {
                // do nothignn
            }
        }
    }

    public void run() {
        for (int i = 0; i < N; i++) {
            new Thread(new Employee(i, random.nextInt(3))).start();
        }
        for (int i = 0; i < C; i++) {
            new Thread(new Client(100 + i)).start();
        }
    }

    public static void main(String[] args) {
        Skill sk = new Skill();
        sk.run();
    }

}
