package gra;

import java.util.List;
import java.util.LinkedList;

class OperationsManager {
    private List<Operation> pendingOperations = new LinkedList<>();
    private List<Operation> visited = new LinkedList<>();

    public void add(Operation operation) throws InterruptedException {
        if (operation.isReady()) {
            operation.perform();
            tryPerform();
        } else {
            operation.waitAndPerform();
            pendingOperations.add(operation);
        }
    }

    public void tryPerform() throws InterruptedException {
        Operation nextOperation = null;
        for (Operation operation : pendingOperations) {
            if (operation.isReady() && (nextOperation == null || nextOperation.getTime() > operation.getTime()))
               nextOperation = operation; 
        }
        if (nextOperation != null) {
            nextOperation.notify();
            wait();
            tryPerform();
        }
    }

    public boolean hasCycles() {
        visited.clear();

        return false;
    } 
}
