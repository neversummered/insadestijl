package my.monitor;

import java.awt.Image;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.ImageIcon;

public final class MonitorReceive extends Thread {

    monitorUI monitor;
    Image img;
    Socket s;
    BufferedInputStream input;
    boolean running;
    char type;
    int size;
    byte[] payload;
    byte[] head;

    @SuppressWarnings("LeakingThisInConstructor")
    public MonitorReceive(monitorUI m) {
        this.monitor = m;
    }

    public void setMonitor(monitorUI monitor) {
        this.monitor = monitor;
    }

    /**
     * Convert byte array into int
     * @param b the byte array to convert
     * @return the int value
     */
    public int getInt(byte[] b) {
        int i = ((b[1] & 0xFF) << 24);
        i += ((b[2] & 0xFF) << 16);
        i += ((b[3] & 0xFF) << 8);
        i += (b[4] & 0xFF);
        return i;
    }

    public void stopThread() {
        running = false;
    }

    public void run() {
        running = true;
        
        try {
            input = monitor.input;
            System.out.println("Thread receive is running");
            while (running == true) {

                head = new byte[5];
                input.read(head, 0, 5);  

                type = (char) head[0];                                      /* Lecture du type */
                size = getInt(head);                                        /* Lecture de la taille des données */

                if (size == 0) {
                    running = false;                                        /* Si la taille est 0 perte de la comm */
                    break;
                }
                payload = new byte[size];

                int size_read = 0;
                while (size_read < size) {
                    size_read += input.read(payload, size_read, size - size_read);                                   /* Lecture des données */

                }
                if (type == 'V') {                                          /* Données reçues de type video */
                    img = new ImageIcon(payload).getImage();
                    monitor.show(img);
                }

                if (type == 'P') {                                          /* Données reçues de type position */
                    monitor.setPosition(payload);
                }
                if (type == 'R'){
                    int robot_status = ByteBuffer.wrap(payload).getInt();
                    monitor.changeRobotStatus(robot_status);
                }
            }

        } catch (IOException e) {
        }
        monitor.changeCommunicationStatus(monitor.COMMUNICATION_DISCONNECTED);
        System.out.println("Trhead receive is stopped");

    }
}
