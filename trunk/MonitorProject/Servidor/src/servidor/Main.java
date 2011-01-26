package servidor;


public class Main
{
    public static void main(String[] args)
    {
                ReceiveInst receive = new ReceiveInst();
                receive.setVisible(true);
                sendImage send = new sendImage(receive);
    }
}
