import java.util.concurrent.Semaphore;
import java.util.Random;

public class test {

    private static Random random = new Random();

    private static void sleepRandom() {
        try {
            Thread.sleep(random.nextInt(5000));
        } catch (Exception e) {
            // Do nothing
        }
    }

    private Semaphore[] companies;
    private Semaphore printer = new Semaphore(0);
    private Semaphore mutex = new Semaphore(1);
    private int freePrinters;
    private boolean[] isPrinterBusy;
    private int companiesNumber;
    private int printersNumber;
    private int employeesPerCompany;

    public test() {
        companiesNumber = 2;
        printersNumber = 1;
        employeesPerCompany = 2;
        freePrinters = printersNumber;
        companies = new Semaphore[companiesNumber];
        for (int i = 0; i < companiesNumber; i++)
            companies[i] = new Semaphore(1);

        isPrinterBusy = new boolean[printersNumber];
        for (int i = 0; i < printersNumber; i++)
            isPrinterBusy[i] = false;
    }

    private void print(int e, int id) {
        System.out.println("Employee " + e + " printing at printer with id = " + id);
        sleepRandom();
    }

    private int findFreePrinter() {
        int ans = 0;
        for (int i = 0; i < printersNumber; i++)
            if (!isPrinterBusy[i])
                return i;
        return -1;
    }

    private void employee(int id, int company) {
        while (true) {
            try {
                System.out.println("Employee " + id + " is waiting for his company to finish printing");
                companies[company].acquire();
                System.out.println("Employee " + id + " is trying to get his hands on a printer");
                mutex.acquire();

                if (freePrinters == 0) {
                    System.out.println("Employee " + id + " is waiting for a free printer");
                    mutex.release();
                    printer.acquire();
                }

                freePrinters--;
                int ind = findFreePrinter();
                isPrinterBusy[ind] = true;
                mutex.release();

                print(id, ind);

                mutex.acquire();
                freePrinters++;
                isPrinterBusy[ind] = false;

                if (freePrinters == 1) {
                    System.out.println("Employee " + id + " has freed last printer " + ind + ", waking someone up");
                    printer.release();
                } else {
                    mutex.release();
                }

                companies[company].release();
                System.out.println("Employee " + id + " has finished his business for now.");
                sleepRandom();
            } catch (Exception e) {
                // Do nothing
            }
        }
    }

    private class emp implements Runnable {
        private int id, company;

        public emp(int id, int company) {
            this.id = id;
            this.company = company;
        }

        @Override
        public void run() {
            employee(id, company);
        }
    }

    private void run() {
        for (int i = 0; i < companiesNumber; i++)
            for (int j = 0; j < employeesPerCompany; j++) {
                new Thread(new emp(i * 100 + j, i)).start(); 
            }
    }

    public static void main(String[] args) {
        test t = new test();
        t.run();
    }
}
