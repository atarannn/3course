public class Main {

    public static void main(String[] args) throws InterruptedException {
        int queueCapacity = 5; // розмір першої черги
        int bufferOverflowOffset = 20; // "порогове" значення другої черги
        int processToGenerate = 20; // кількість процесів
        System.out.println("\nQueue capacity = [" + queueCapacity + "], buffer overflow offset = [" + bufferOverflowOffset + "] will be generated " + processToGenerate + " processes\n");
        CPUQueue b = new CPUQueue(bufferOverflowOffset); // створюємо другу чергу
        CPUQueue q = new CPUQueue(queueCapacity, b); // створюємо першу чергу
        CPUProcessGenerator PrG = new CPUProcessGenerator(q, processToGenerate); // генератор процесів
        CPU C = new CPU(q, b, PrG); // центральний процесор комп'ютера
        Thread th1 = new Thread(PrG);
        Thread th2 = new Thread(C);
        th1.start();
        th2.start();
        th1.join();
        th2.join();

        System.out.println("\nBuffer max size = [" + b.getMaxSize() + "]\n" + (float)C.b_count/(C.q_count+C.b_count) * 100 + " % processes (" + C.b_count + ") from buffer queue");
    }
}
