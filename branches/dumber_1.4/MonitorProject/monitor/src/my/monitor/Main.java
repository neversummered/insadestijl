package my.monitor;


public class Main
{
    public static void main(String[] args)
    {
                monitorUI monitor = new monitorUI();
                monitor.setVisible(true);
                MonitorSend monitorS = new MonitorSend(monitor);
                MonitorReceive monitorR = new MonitorReceive(monitor);
    }
}