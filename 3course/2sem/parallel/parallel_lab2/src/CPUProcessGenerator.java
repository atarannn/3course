class CPUProcessGenerator implements Runnable { // створюємо клас (генератор процесів), котрий реалізує інтерфейс Runnable

    CPUQueue queue;
    int generateNumber;
    boolean end = false;

    CPUProcessGenerator(CPUQueue q, int gN) {
        this.queue = q;
        this.generateNumber = gN;
    }

    public void run() {
        long generateDelay;
        for (int i = 0; i < generateNumber; i++) { // генеруєм випадкове число, для затримки
            int randMin = 10;
            int randMax = 40;
            generateDelay = randMin + (int) (Math.random() * randMax);

            try { // призупиємо генерацію процесів, на який рандомний час
                Thread.sleep(generateDelay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            try {
                System.out.println("Process generated with delay " + generateDelay);
                int randMinProc = 20;
                int randMaxProc = 80;
                int processingTime = randMinProc + (int) (Math.random() * randMaxProc); // час, котрий процес буде створюватись ( час виконання execute)
                queue.put(new PseudoProc(i, processingTime)); // процес додається в чергу
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("No more processes. Greatest queue size was " + queue.getMaxSize());
        end = true;
    }

    public boolean isEnd() {
        return end;
    } // чекаємо на завершення всіх процесів
}