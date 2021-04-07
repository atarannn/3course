import java.util.concurrent.atomic.AtomicLong;
import java.util.stream.LongStream;
import java.util.stream.IntStream;
import java.util.Random;

public class Scalar {
    long serialScalar = 0;

    public Scalar(int variant) {
        int[] array = new Random().ints(10000L * variant, 0, 100).toArray();
        int[] array2 = new Random().ints(10000L * variant, 0, 100).toArray();
        AtomicLong atomicScalar = new AtomicLong();

        IntStream.range(0, array.length).forEach(index -> {
            serialScalar += (long) array[index] * array2[index];
        });

        IntStream.range(0, array.length).parallel().forEach(index -> {
            long oldValue;
            long newValue;
            do {
                oldValue = atomicScalar.get();
                newValue = oldValue + (long) array[index] * array2[index];
            } while (!atomicScalar.compareAndSet(oldValue, newValue));
        });

        System.out.println("Serial scalar:   " + serialScalar);
        System.out.println("Atomic scalar:   " + atomicScalar.get());
    }
}