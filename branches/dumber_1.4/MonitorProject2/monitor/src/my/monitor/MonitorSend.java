package my.monitor;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public final class MonitorSend extends Thread {

    int port;
    String address;
    monitorUI monitor;
    Socket s;
    BufferedOutputStream output;
    boolean running;
    byte[] msg;

    public MonitorSend(monitorUI m) {
        this.monitor = m;
    }

    public void getMsg() {
        msg = monitor.getData();
    }

    public void setMonitor(monitorUI monitor) {
        this.monitor = monitor;
    }

    public void stopThread() {
        this.running = false;
    }

    public void run() {
        running = true;
        System.out.println("Thread send is runnig");
        while (running == true) {

            getMsg();
            monitor.sendData(msg);

            try {
                sleep(200);
            } catch (InterruptedException p) {
            }
        }
        System.out.println("Thread send is stopped");
    }
}
