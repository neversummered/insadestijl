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
    boolean running = true;
    byte[] msg;

    public MonitorSend(monitorUI m) {

        setMonitor(m);
        m.setMonitorSend(this);
        monitor = m;
    }

    public void getMsg() {
        msg = monitor.getData();
    }

    public void setMonitor(monitorUI monitor) {
        this.monitor = monitor;
    }

    public void run() {
        while (true) {

            while (running == true) {
                
                if (monitor.direction != 0){ 
                    getMsg();
                    monitor.sendData(msg);
                }

                try {
                    sleep(50);
                } catch (InterruptedException p) {
                }
            }


            while (running == false) {
                try {
                    sleep(100);
                } catch (InterruptedException p) {
                }
            }

        }
    }
}
