import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.atomic.AtomicLong;
import java.util.stream.IntStream;
import static java.lang.Double.*;

public class EuclidNorm {
    public EuclidNorm(int variant){
        double[] array = new Random().doubles(10000L * variant).toArray();
        AtomicLong atomicDouble = new AtomicLong();
        double serialNorm = Math.sqrt(Arrays.stream(array).map(a -> a*a).sum());
        Arrays.stream(array).parallel().forEach(arrayElement -> {
            double oldValue;
            long oldValueLong;
            double newValue;
            do {
                oldValueLong = atomicDouble.get();
                oldValue = longBitsToDouble(oldValueLong);
                newValue = oldValue + arrayElement*arrayElement;
            } while (!atomicDouble.compareAndSet(oldValueLong, doubleToLongBits(newValue)));
        });
        System.out.println("Serial Norm:   " + serialNorm);
        System.out.println("Atomic Norm:   " + Math.sqrt(longBitsToDouble(atomicDouble.get())));
    }
}
