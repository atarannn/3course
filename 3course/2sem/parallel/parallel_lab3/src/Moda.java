import java.awt.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.atomic.AtomicLong;
import java.util.stream.Collectors;
import java.util.stream.LongStream;

public class Moda {
    public Moda(int variant){
        long[] array = new Random().longs(10000L * variant, 0, 1000).toArray();
        int[] freqArr = new int[1000];

        AtomicIntegerArray atomicFreqArr = new AtomicIntegerArray(1000);

        LongStream.of(array).forEach(arrayElement -> {
            freqArr[(int) arrayElement]++;
        });

        int serialModa = Arrays.stream(freqArr).boxed().collect(Collectors.toList()).indexOf(Arrays.stream(freqArr).max().orElseThrow());

        LongStream.of(array).parallel().forEach( arrayElement -> {
            int oldValue;
            int newValue;
            do{
                oldValue = atomicFreqArr.get((int) arrayElement);
                newValue = oldValue + 1;
            }while(!atomicFreqArr.compareAndSet((int) arrayElement, oldValue , newValue));
        });

        int[] freqArr1 = new int[1000];
        for(int i=0; i < 1000; i++){
            freqArr1[i] = atomicFreqArr.get(i);
        }
        int atomicModa = Arrays.stream(freqArr1).boxed().collect(Collectors.toList()).indexOf(Arrays.stream(freqArr1).max().orElseThrow());


        HashMap<Integer, Integer> hashMap = new HashMap<>();
        for (int i=0; i < 1000; i++){
            hashMap.put(i, freqArr1[i]);
        }

        List<Map.Entry<Integer, Integer>> sortedList = new ArrayList<>(hashMap.entrySet());
        sortedList.sort(Comparator.comparingInt(Map.Entry::getKey));
        int medianIndex = array.length / 2;
        int index = 0;
        long median = 0;
        for (int i = 0; i < sortedList.size(); i++) {
            index += sortedList.get(i).getValue();
            if (index >= medianIndex) {
                median = sortedList.get(i).getKey();
                break;
            }
        }


        System.out.println("Median:   " + median);

        System.out.println("Serial moda:   " + serialModa);
        System.out.println("Atomic moda:   " + atomicModa);

    }
}
