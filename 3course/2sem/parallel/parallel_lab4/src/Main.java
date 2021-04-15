import java.util.*;
import java.util.concurrent.*;
import java.util.stream.*;

public class Main {

    public static void main(String[] args) {

        // создаем три коллекции с рандомными числами
        final List<Integer>  a1 = new Random().ints(10, 0, 20).boxed().collect(Collectors.toList()); //first
        final List<Integer>  a2 = new Random().ints(10,0, 20).boxed().collect(Collectors.toList()); //second
        final List<Integer>  a3 = new Random().ints(10,0, 20).boxed().collect(Collectors.toList()); //third

        CompletableFuture<List<Integer>> firstFuture, secondFuture, thirdFuture, resultFuture;

        firstFuture = CompletableFuture.supplyAsync(() -> a1) // создаем 1 фьючерс методом supplyAsync
                .thenApplyAsync( first -> { // функция будет выполняться в отдельном потоке и возвращать результат, с помощью метода thenApplyAsync
                    double max02 = first.stream().max(Comparator.naturalOrder()).orElseThrow() * 0.2; // максимальный элемент массива умноженный на 0,2
                    first = first.stream().filter(v -> v > max02).collect(Collectors.toList()); // проверяем элементы (больше ли они за max02), и осталяем те, для которых этот фльтр выполняется
                    first.sort(Comparator.naturalOrder()); // сортируем результат по возрастанию
                    return first;
                });

        secondFuture = CompletableFuture.supplyAsync(() -> a2) // создаем 2 фьючерс
                .thenApplyAsync( second -> {
                    second = second.stream().filter(v -> v % 5 == 0).collect(Collectors.toList()); // проверяем элементы (делятся ли они на 5 без остатка), и осталяем те, для которых этот фльтр выполняется
                    second.sort(Comparator.naturalOrder()); // сортируем результат по возрастанию
                    return second;
                });

        thirdFuture = CompletableFuture.supplyAsync(() -> a3) // создаем 3 фьючерс
                .thenApplyAsync( third -> {
                    third = third.stream().map(v -> v / 3).collect(Collectors.toList()); // уменьшаем каждый элемент в 3 раза
                    third.sort(Comparator.naturalOrder()); // сортируем результат по возрастанию
                    return third;
                });

        resultFuture = CompletableFuture.allOf(firstFuture, secondFuture, thirdFuture) // создаем 4 фьючерс
                .thenApply(tre -> {
                    HashSet<Integer> firstSet = new HashSet<>(firstFuture.join());
                    HashSet<Integer> secondSet = new HashSet<>(secondFuture.join());

                    firstSet.retainAll(secondSet); // удаляем элементы в firstSet, которых нет в secondSet  (если элемент есть в firstSet, но нет в secondSet - удаляем)

                    List<Integer> result = new ArrayList<>(firstFuture.join().stream().filter(firstSet::contains).collect(Collectors.toList())); // создаем новую коллекцию result

                    result.addAll(secondFuture.join().stream().filter(firstSet::contains).collect(Collectors.toList())); // добавляем элементы со второго фьючерса в result
                    result.addAll(thirdFuture.join().stream().filter(firstSet::contains).collect(Collectors.toList()));

                    result.sort(Comparator.naturalOrder()); // сортируем результат по возрастанию

                    return result;
                });

        System.out.println("First (before): " + a1);
        System.out.println("First result: " + firstFuture.join());

        System.out.println("Second  (before): " + a2);
        System.out.println("Second result: " + secondFuture.join());

        System.out.println("Third  (before): " + a3);
        System.out.println("Third result: " + thirdFuture.join());

        System.out.println("Result: " + resultFuture.join());
    }
}