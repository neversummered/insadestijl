package my.monitor;

import java.awt.Image;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.net.Socket;
import javax.swing.ImageIcon;

public final class MonitorReceive extends Thread {

    monitorUI monitor;
    Image img;
    Socket s;
    BufferedInputStream input;
    boolean running = true;

    char type;
    int size;
    byte[] tmp;
    byte[] head;

    @SuppressWarnings("LeakingThisInConstructor")
    public MonitorReceive(monitorUI m) {
        setMonitor(m);
        m.setMonitorReceive(this);
        monitor = m;
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

    public void run() {
        while (true) {
            try {
                input = monitor.input;
                while (running == true) {

                    head = new byte[5];
                    input.read(head, 0, 5);                                     /* Lecture de l'en-tête */
                    type = (char) head[0];                                      /* Lecture du type */
                    size = getInt(head);                                        /* Lecture de la taille des données */

                    if (size == 0) {
                        running = false;                                        /* Si la taille est 0 perte de la comm */
                        break;
                    }

                    tmp = new byte[size];
                    input.read(tmp, 0, size);                                   /* Lecture des données */

                    if (type == 'V') {                                          /* Données reçues de type video */
                        img = new ImageIcon(tmp).getImage();
                        monitor.show(img);
                    }

                    if (type == 'P') {                                          /* Données reçues de type position */
                        monitor.setPosition(tmp);
                    }

                }

                while (running == false) {
                    try {
                        sleep(100);
                    } catch (InterruptedException p) {
                    }
                }


            } catch (IOException e) {
            }
        }

    }
}
