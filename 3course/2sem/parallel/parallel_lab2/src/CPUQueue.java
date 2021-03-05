import java.util.LinkedList;
import java.util.Queue;

class CPUQueue {

    private final Queue<PseudoProc> queue = new LinkedList<>();
    private final int capacity;
    private int maxSize = 0;
    public CPUQueue overflowBuf = null;

    public CPUQueue(int capacity) {
        this.capacity = capacity;
    }

    public CPUQueue(int capacity, CPUQueue overflowBuf) {
        this.capacity = capacity;
        this.overflowBuf = overflowBuf;
    }

    public int getMaxSize() {
        return maxSize;
    }

    public boolean offset() {
        return queue.size() > capacity;
    } // переповнена черга

    public boolean isEmpty() {
        return queue.isEmpty();
    } // пуста черга

    public synchronized void put(PseudoProc element) throws InterruptedException {
        if (queue.size() == capacity && overflowBuf != null) { // якщо перша черга повна і друга черга не null
            overflowBuf.put(element); // кладемо елемент в другу чергу
        } else {
            queue.add(element);  // додаємо елемент в першу чергу

            if (queue.size() > maxSize)
                maxSize = queue.size(); // фіксуємо розмір черги
            System.out.println("Process added, queue size = [" + queue.size() + "]\n");
        }
    }

    public synchronized PseudoProc get() throws InterruptedException {
        if (queue.isEmpty()) { //  черга пуста
            return null;
        } else { // якщо не пуста
            PseudoProc item = queue.remove(); // видаляємо процес з черги
            System.out.println("Process removed, queue size = [" + queue.size() + "]");
            return item;
        }
    }
}
