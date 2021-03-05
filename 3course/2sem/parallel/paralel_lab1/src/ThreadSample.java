import java.util.Arrays;

public class ThreadSample {
    public static final int SIZE = 5; // розмір матриці
    public static final int NUMBER_THREADS = 2; // кількість потоків
    public static final int TESTS = 1; // кількість матриць для перевірки
    public static final int RAND_MAX = 10; // числа для заповлення матриці
    public static final int TABLE_HEADER = 2;
    public static final int TABLE_FOOTER = 2;

    public static void main(String[] args) throws InterruptedException {
        final Object[][] resultsTable = new String[TABLE_HEADER + TESTS + TABLE_FOOTER][];
        resultsTable[0] = new String[]{"Serial", "Parallel"}; // результуюче текстове поле
        resultsTable[1] = new String[]{"time (us)", "time (us)"}; // результуюче текстове поле
        long[] serialTimes = new long[TESTS]; // час послідовного виконання
        long[] parallelTimes = new long[TESTS]; // час паралельного виконання

        for (int test_i = 0; test_i < TESTS; test_i++) {
            int[][] matA = new int[SIZE][SIZE]; // оголошуємо нашу квадратну матрицю для послідовного виконання
            int rand_min = 1; // мінімальний елемент матриці

            for (int i = 0; i < SIZE; i++) { // заповнюємо матрицю рандомними числами
                for (int j = 0; j < SIZE; j++) {
                    matA[i][j] = rand_min + (int) (Math.random() * RAND_MAX);
                }
            }

            int[][] matAparallel = Arrays.stream(matA).map(int[]::clone).toArray(int[][]::new); // записуємо матрицю в нову змінну


            long start = System.nanoTime(); // початок виконання

            for (int i = 0; i < SIZE; i++) { // на побічній діагоналі находимо мінімальний елемент і присвоюємо елементам діагоналі значення мінімальношл
                matA[i][SIZE - i - 1] = Arrays.stream(matA[i]).min().orElse(-1);
            }

            long end = System.nanoTime(); // кінець виконання

            printMat(matA); // виводимо першу матрицю

            long serialTime = (end - start) / 1000; // час розрахунку, для посліднового виконання
            serialTimes[test_i] = serialTime;


            start = System.nanoTime(); // отримуємо найбільш точний час початку виконання

            ThreadCacl[] TreadArray = new ThreadCacl[NUMBER_THREADS]; // ініціалізуємо потік
            for (int i = 0; i < NUMBER_THREADS; i++) { // запускаємо потоки
                TreadArray[i] = new ThreadCacl(matAparallel,
                        SIZE / NUMBER_THREADS * i,
                        i == (NUMBER_THREADS - 1) ? SIZE : SIZE / NUMBER_THREADS * (i + 1));
                TreadArray[i].start(); // запускаємо потік
            }

            for (int i = 0; i < NUMBER_THREADS; i++) {
                TreadArray[i].join(); // зупиняємо виконання потоку, поки другий потік не припинить свою роботу
            }

            end = System.nanoTime(); // отримуємо найбільш точний час завершення виконання

            printMat(matAparallel); // друга матриця

            long parallelTime = (end - start) / 1000; // час розрахунку, для паралельного виконання
            parallelTimes[test_i] = parallelTime;

            resultsTable[TABLE_HEADER + test_i] = new String[]{String.valueOf(serialTime), String.valueOf(parallelTime)};
        }

        resultsTable[TABLE_HEADER + TESTS] = new String[]{"Average:", ""};
        resultsTable[TABLE_HEADER + TESTS + 1] = new String[]{
                String.valueOf(Arrays.stream(serialTimes).average().orElse(-1)),
                String.valueOf(Arrays.stream(parallelTimes).average().orElse(-1))};
        for (final Object[] row : resultsTable) {
            System.out.format("%10s%10s%n", row);
        }
    }

    public static void printMat(int[][] matA) { // функція для друку матриць
        System.out.println(); // для друку матриць в консоль
        for (int[] line : matA) {
            for (int elem : line) {
                System.out.print(elem + "\t");
            }
            System.out.println();
        }
    }
}
