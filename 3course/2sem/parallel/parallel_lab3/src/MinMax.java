import java.util.concurrent.atomic.AtomicLong;
import java.util.stream.LongStream;
import java.util.Random;

public class MinMax {
    public MinMax(int variant){
        long[] array = new Random().longs(10000L * variant).toArray();
        AtomicLong atomicMin = new AtomicLong(Long.MAX_VALUE);

        long serialMin = LongStream.of(array).min().orElseThrow();

        LongStream.of(array).parallel().forEach( arrayElement -> {
            long oldValue;
            long newValue;
            do{
                oldValue = atomicMin.get();
                if (arrayElement < oldValue){
                    newValue = arrayElement;
                }
                else {
                    newValue = oldValue;
                }
            }while(!atomicMin.compareAndSet(oldValue , newValue));
        });

        System.out.println("Serial min:   " + serialMin);
        System.out.println("Atomic min:   " + atomicMin.get());


        array = new Random().longs(10000L * variant).toArray();
        AtomicLong atomicMax = new AtomicLong(Long.MIN_VALUE);

        long serialMax = LongStream.of(array).max().orElseThrow();

        LongStream.of(array).parallel().forEach( arrayElement -> {
            long oldValue;
            long newValue;
            do{
                oldValue = atomicMax.get();
                if (arrayElement > oldValue){
                    newValue = arrayElement;
                }
                else {
                    newValue = oldValue;
                }
            }while(!atomicMax.compareAndSet(oldValue, newValue));
        });

        System.out.println("Serial max:   " + serialMax);
        System.out.println("Atomic max:   " + atomicMax.get());
    }
}
