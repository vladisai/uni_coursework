package gra;

import java.util.List;
import java.util.LinkedList;
import java.lang.Math;

public class MojaPlansza implements Plansza {

    private int height, width;
    private List <PositionedShape> positionedShapes = new LinkedList<>();
    private OperationsManager operationsManager;

    private synchronized PositionedShape findShapeByPosition(int row, int column) {
        for (PositionedShape shape : positionedShapes) {
            if (shape.checkIfCovers(row, column))
                return shape;
        }
        return null;
    }

    private synchronized PositionedShape findShape(Postać shape) {
        for (PositionedShape currentShape : positionedShapes) {
            if (currentShape.getShape() == shape)
                return currentShape;
        }
        return null;
    }

    public MojaPlansza(int height, int width) {
        if (height < 1 || width < 1)
            throw new IllegalArgumentException();
        this.height = height;
        this.width = width;
        operationsManager = new OperationsManager(this);
    }

    public void postaw(Postać shape, int row, int column)
    throws InterruptedException {
        PositionedShape positionedShape = new PositionedShape(row, column, shape);
        if (!positionedShape.fits(this))
            throw new IllegalArgumentException();
        try {
            operationsManager.add(new PutOperation(this, operationsManager, positionedShape));
        } catch (DeadlockException e) {
            // Do nothing.
        }
    }

    public void przesuń(Postać shape, Kierunek direction)
    throws InterruptedException, DeadlockException {
        PositionedShape cShape = findShape(shape);
        if (cShape != null) {
            operationsManager.add(new MoveOperation(this, operationsManager, cShape, direction));
        } else {
            throw new IllegalArgumentException();
        }
    }

    public void usuń(Postać shape) {
        removeShape(shape);
    }

    public void sprawdź(int row, int column,
                         Akcja ifOccupied, Runnable ifFree) {
        if (row < 0 || column < 0 || row >= height || column >= width)
            throw new IllegalArgumentException();
        PositionedShape shape = findShapeByPosition(row, column);
        if (shape != null) {
            ifOccupied.wykonaj(shape.getShape());
        } else {
            ifFree.run();
        }
    }

    public synchronized List<PositionedShape> getPositionedShapes() {
        return positionedShapes;
    }

    public synchronized void placePositionedShape(PositionedShape shape) {
        positionedShapes.add(shape);
    }

    public synchronized void removeShape(Postać shape) {
        PositionedShape cShape = findShape(shape);
        if (cShape != null) {
            positionedShapes.remove(cShape);
        } else {
            throw new IllegalArgumentException();
        }
    }

    public synchronized void removePositionedShape(PositionedShape shape) {
        removeShape(shape.getShape());
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

}

class PositionedShape {
    private int row;
    private int column;
    private Postać shape;

    public PositionedShape(int row, int column, Postać shape) {
        this.row = row;
        this.column = column;
        this.shape = shape;
    }

    boolean checkIfCovers(int row, int column) {
        return this.row <= row
               && this.row + shape.dajWysokość() - 1 >= row
               && this.column <= column
               && this.column + shape.dajSzerokość() - 1 >= column;
    }

    boolean intersects(PositionedShape shape) {
        int rowTop = Math.max(this.row, shape.getRow());
        int columnLeft = Math.max(this.column, shape.getColumn());
        int rowBottom = Math.min(this.getRow() + this.getHeight() - 1,
                                 shape.getRow() + shape.getHeight() - 1);
        int columnRight = Math.min(this.getColumn() + this.getWidth() - 1,
                                   shape.getColumn() + shape.getWidth() - 1);

        if (rowTop <= rowBottom  && columnLeft <= columnRight) {
            return true;
        } else {
            return false;
        }
    }

    boolean fits(MojaPlansza field) {
        if (getRow() + getHeight() - 1 >= field.getHeight()
                || getColumn() + getWidth() - 1 >= field.getWidth()
                || getRow() < 0
                || getColumn() < 0) {
            return false;
        } else {
            return true;
        }
    }

    int getHeight() {
        return shape.dajWysokość();
    }

    int getWidth() {
        return shape.dajSzerokość();
    }

    int getRow() {
        return row;
    }

    int getColumn() {
        return column;
    }

    Postać getShape() {
        return shape;
    }

    @Override
    public String toString() {
        return "shape: " + row + ", " + column + " (" + getHeight() + " by " + getWidth() + ")";
    }
}
