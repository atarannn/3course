import java.util.Arrays;

class ThreadCacl extends Thread {

    int[][] matA;
    int startIndex;
    int endIndex;

    public ThreadCacl(int[][] matA, int startIndex, int endIndex) {
        this.matA = matA;
        this.startIndex = startIndex;
        this.endIndex = endIndex;
    }

    @Override
    public void run() { // запуск потоку
        for (int i = startIndex; i < endIndex; i++) {
            matA[i][ThreadSample.SIZE - i - 1] = Arrays.stream(matA[i]).min().orElse(-1);
        }
    }
}
