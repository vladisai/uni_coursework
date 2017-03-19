import java.util.List;
import java.util.Random;
import java.util.LinkedList;

class Problem implements Runnable {

    private static Random random = new Random();

    private static void sleepRandom() {
        try {
            Thread.sleep(random.nextInt(5000));
        } catch (Exception e) {
            // Do nothing
        }
    }

    private final int group;
    private final Server server;

    private void initialize() {
        System.out.println("Initializing...");
    }

    private void use(int resource) {
        System.out.println("Using resource " + resource);
        sleepRandom();
    }

    public Problem(final int group, final Server server) {
        this.group = group;
        this.server = server;
    }

    @Override
    public void run() {
        while (true) {
            initialize();
            System.out.println("Requesting access on behalf of group " + group);
            try {
                int resource = server.request(group);
                System.out.println("On behalf of group " + group + " gained access to resource " + resource);
                use(resource);
                server.release(group, resource);
            } catch (Exception e) {
                // Do nothing
            }
        }
    }
}

class Server {
    public static final int GROUPS_NUMBER = 2;
    public static final int RESOURCES_NUMBER = 1;

    private boolean[] groups = new boolean[GROUPS_NUMBER];
    private LinkedList<Integer> resources = new LinkedList<>();

    Server() {
        for (int i = 0; i < RESOURCES_NUMBER; i++) {
            resources.add(i);
        }
    }

    public synchronized int request(int group) throws InterruptedException {
        System.out.println("Group " + group + " has requested access to a resource");
        while (groups[group]) {
            System.out.println("Group " + group + " is busy, waiting.");
            wait();
        }
        System.out.println("Group " + group + " is free, proceeding to resources.");
        groups[group] = true;
        while (resources.isEmpty()) {
            System.out.println("No available resources at the moment. Waiting.");
            wait();
        }
        System.out.println("A resource " + resources.getLast() + " was found.");
        return resources.removeLast();
    }

    public synchronized void release(int group, int resource) {
        System.out.println("Releasing group " + group + " and resource " + resource);
        groups[group] = false;
        resources.add(resource);
        notifyAll();
    }
}

public class Problem2 {
    private static final int USERS_PER_GROUP = 3;
    public static void main(String[] args) {
        Server server = new Server();
        for (int i = 0; i < Server.GROUPS_NUMBER; i++) {
            for (int j = 0; j < USERS_PER_GROUP; j++) {
                new Thread(new Problem(i, server)).start();
            }
        }
    }
}
