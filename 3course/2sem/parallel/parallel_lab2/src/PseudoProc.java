public class PseudoProc {

    int num;
    int time;

    public PseudoProc(int num, int time) {
        this.num = num;
        this.time = time;
    }

    public void execute() {
        System.out.println("CPU: Proc[" + this.num + "] processed in time " + this.time + "\n");
        try {
            Thread.sleep(this.time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
