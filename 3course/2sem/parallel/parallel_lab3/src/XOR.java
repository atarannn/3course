import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.IntStream;

public class XOR {
    public XOR(int variant) {
        int[] array = new Random().ints(10000L * variant).toArray();
        AtomicInteger atomicXOR = new AtomicInteger();

        int serialXOR = IntStream.of(array).reduce((a, b) -> a ^ b).orElseThrow();

        IntStream.of(array).parallel().forEach(arrayElement -> {
            int oldValue;
            int newValue;
            do {
                oldValue = atomicXOR.get();
                newValue = oldValue ^ arrayElement;
            } while (!atomicXOR.compareAndSet(oldValue, newValue));
        });

        System.out.println("Serial XOR:   " + serialXOR);
        System.out.println("Atomic XOR:   " + atomicXOR.get());
    }
}
