package gra;

/**
 * Created by vlad on 08.12.16.
 */
public class Shape implements Postać {
    private int height;
    private int width;

    public Shape(int height, int width) {
        this.height = height;
        this.width = width;
    }

    @Override
    public int dajSzerokość() {
        return width;
    }

    @Override
    public int dajWysokość() {
        return height;
    }
}
