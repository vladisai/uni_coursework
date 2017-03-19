package gra;

import java.util.List;
import java.util.LinkedList;

class OperationsManager {
    private List<Operation> pendingOperations = new LinkedList<>();
    private List<Operation> visited = new LinkedList<>();
    private MojaPlansza field;

    OperationsManager(MojaPlansza field) {
        this.field = field;
        MoveOperation.clear();
    }

    public void add(Operation operation) throws InterruptedException, DeadlockException {
        if (!tryPerform(operation))
            putIntoPending(operation);
    }

    public void tryPerformNext() throws InterruptedException {
        Operation nextOperation = null;
        synchronized (this) {
            for (Operation operation : pendingOperations) {
                if (operation.isReady() && (nextOperation == null || nextOperation.getTime() > operation.getTime()))
                    nextOperation = operation;
            }
        }
        if (nextOperation != null) {
            synchronized (this) {
                synchronized (nextOperation) {
                    nextOperation.notify();
                }
                wait();
                pendingOperations.remove(nextOperation);
                tryPerformNext();
            }
        }
    }

    private boolean tryPerform(Operation operation) throws InterruptedException {
        boolean hasPerformed = false;
        synchronized (field) {
            if (operation.isReady()) {
                operation.perform();
                hasPerformed = true;
            }
        }
        if (hasPerformed)
            tryPerformNext();
        return hasPerformed;
    }

    private void putIntoPending(Operation operation) throws InterruptedException, DeadlockException {
        synchronized (pendingOperations) {
            pendingOperations.add(operation);
            MoveOperation.checkForIntersections();
        }
        operation.waitAndPerform();
    }

}
