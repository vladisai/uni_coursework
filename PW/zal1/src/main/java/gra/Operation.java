package gra;

import java.util.List;
import java.util.LinkedList;

abstract class Operation {

    protected MojaPlansza field;
    private int time;
    private static int currentTime = 0;
    private OperationsManager manager;

    Operation(MojaPlansza field, OperationsManager manager) {
        this.field = field;
        this.manager = manager;
        time = currentTime++;
    }

    public abstract boolean isReady();

    public abstract void perform();

    public void waitAndPerform() throws InterruptedException {
        if (!isReady())
            wait();
        perform();
        manager.notify();
    }

    public int getTime() {
        return time;
    }
}

class MoveOperation extends Operation {

    private PositionedShape shape;
    private PositionedShape newShape;

    private static List<MoveOperation> moveOperations = new LinkedList<>();
    private static List<MoveOperation> visited = new LinkedList<>();

    public static boolean isVisited(MoveOperation operation) {
        for (MoveOperation moveOperation : moveOperations) {
            if (moveOperation == operation)
                return true;
        }
        return false;
    }

    private static void dfs(MoveOperation moveOperation) throws DeadlockException {
        visited.add(moveOperation);
        for (MoveOperation next : moveOperations) {
            if (next != moveOperation && areConnected(moveOperation, next)) {
                if (isVisited(next)) {
                    throw new DeadlockException();
                } else {
                    dfs(next);
                }
            }
        }
    }

    public static void checkForIntersections() throws DeadlockException {
        visited = new LinkedList<>();
        for (MoveOperation moveOperation : moveOperations) {
            if (!isVisited(moveOperation))
                dfs(moveOperation);
        }
    }

    public static boolean areConnected(MoveOperation move1, MoveOperation move2) {
        if (move1.newShape.intersects(move2.shape)) {
            return true;
        } else {
            return false;
        }
    }

    MoveOperation(MojaPlansza field, OperationsManager manager, PositionedShape shape, Kierunek direction) {
        super(field, manager);
        this.shape = shape;
        int nRow = transformRow(shape.getRow(), direction);
        int nColumn = transformColumn(shape.getColumn(), direction);
        this.newShape = new PositionedShape(nRow, nColumn, shape.getShape());
        if (!this.newShape.fits(field))
            throw new IllegalArgumentException();

        moveOperations.add(this);
    }

    public boolean isReady() {
        for (PositionedShape shape : field.getPositionedShapes()) {
            if (shape.intersects(newShape) && shape.getShape() != this.shape.getShape())
                return false;
        }
        return true;
    }

    public void perform() {
        this.field.removePositionedShape(shape);
        this.field.placePositionedShape(newShape);
        moveOperations.remove(this);
    }

    private int transformRow(int row, Kierunek direction) {
        switch (direction) {
        case GÓRA:
            return row - 1;
        case DÓŁ:
            return row + 1;
        default:
            return row;
        }
    }

    private int transformColumn(int column, Kierunek direction) {
        switch (direction) {
        case LEWO:
            return column - 1;
        case PRAWO:
            return column + 1;
        default:
            return column;
        }
    }
}

class PutOperation extends Operation {
    private PositionedShape newShape;

    PutOperation(MojaPlansza field, OperationsManager manager, PositionedShape newShape) {
        super(field, manager);
        this.newShape = newShape;
        if (!newShape.fits(field))
            throw new IllegalArgumentException();
    }

    public boolean isReady() {
        for (PositionedShape shape : field.getPositionedShapes()) {
            if (shape.intersects(newShape) && shape.getShape() != this.newShape.getShape())
                return false;
        }
        return true;
    }

    public void perform() {
        this.field.placePositionedShape(newShape);
    }
}
