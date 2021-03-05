class CPU implements Runnable { // створюємо клас, котрий реалізує інтерфейс Runnable

    CPUQueue queue;
    CPUQueue buff;
    CPUProcessGenerator PrG;
    public int q_count = 0;
    public int b_count = 0;

    CPU(CPUQueue q, CPUQueue b, CPUProcessGenerator PrG) {
        this.queue = q; // перша черга
        this.buff = b; // друга черга
        this.PrG = PrG; // генератор
    }

    public void run() {
        PseudoProc process = null;
        boolean fromBuff = false;
        do {
            try {
                if (buff.offset() || queue.isEmpty()) { // якщо друга черга за "порогом" або перша черга пуста
                    process = buff.get(); // брати з другої черги
                    fromBuff = true;
                } else {
                    process = queue.get(); // брати з першої черги
                    fromBuff = false;
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if (process != null) { // з якої черги процеси були взяті до процесора
                if (fromBuff){
                    System.out.println("From buff");
                    b_count++;
                }
                else {
                    System.out.println("From queue");
                    q_count++;
                }
                process.execute();
                process = null;
            }

        } while (!queue.isEmpty() || !buff.isEmpty() || !PrG.isEnd()); // виконуємо дії поки перша черга не пуста, друга черга не пуста та генератор процесів ще працює
    }
}
