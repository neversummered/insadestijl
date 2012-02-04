package servidor;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Random;

public class sendImage extends Thread{

        ReceiveInst receive;
        boolean running = true;
        int alterna;
        File file;
        FileInputStream fin;
        byte buffer[];
        byte msg[];
        char type = 'V';
        String endIm;
        boolean imgPos = true;


   public sendImage(ReceiveInst ri){

        setReceive(ri);
        ri.setSendImage(this);
        receive = ri;
   }

    public void setReceive(ReceiveInst ri) {
        this.receive = ri;
    }




    public byte[] intToByte(int value){
        byte [] b = new byte[4];
        b[0] = (byte)(value>>24);
        b[1] = (byte)(value>>16);
        b[2] = (byte)(value>>8);
        b[3] = (byte)(value);
        return b;
    }

    public void run(){

            alterna = 1;

            
            try{
               while(true){

                   while(running==true){

                        if(imgPos){
                            if (alterna==6){
                                file = new File("/home/vboeingr/Desktop/Images/6.jpg");
                                alterna=1;
                            }
                            if (alterna==5){
                                file = new File("/home/vboeingr/Desktop/Images/5.jpg");
                                alterna=6;
                            }
                            if(alterna==4){
                                file = new File("/home/vboeingr/Desktop/Images/4.jpg");
                                alterna=5;
                            }
                              if(alterna==3){
                                file = new File("/home/vboeingr/Desktop/Images/3.jpg");
                                alterna=4;
                            }
                             if(alterna==2){
                                file = new File("/home/vboeingr/Desktop/Images/2.jpg");
                                alterna=3;
                            }
                             if(alterna==1){
                                file = new File("/home/vboeingr/Desktop/Images/1.jpg");
                                alterna=2;
                            }

                            fin = new FileInputStream(file);
                            type = 'V';
                            buffer = new byte[(int)file.length()];
                            fin.read(buffer);


                            msg = new byte[(int)file.length()+5];
                            msg[0] = (byte)type;


                            System.arraycopy(intToByte((int)file.length()), 0, msg, 1, 4);
                            System.arraycopy(buffer, 0, msg, 5, (int)file.length());
                            imgPos = false;
                        }


                        else{
                            type = 'P';
                            buffer = new byte[3];
                            Random randomGenerator = new Random();
                            buffer[0] = (byte)randomGenerator.nextInt(100);
                            buffer[1] = (byte)randomGenerator.nextInt(100);
                            buffer[2] = (byte)randomGenerator.nextInt(50);

                            msg = new byte[buffer.length + 5];
                            msg[0] = (byte)type;


                            System.arraycopy(intToByte(buffer.length), 0, msg, 1, 4);
                            System.arraycopy(buffer, 0, msg, 5, buffer.length);

                            imgPos = true;
                        }

                        receive.output.write(msg);
                        receive.output.flush();

                        try{sleep(100);}
                        catch(InterruptedException p){}
                    }

                    while(running==false){
                        try{sleep(100);}
                        catch(InterruptedException p){}

                    }

            }
        }catch(IOException e){System.out.println("error!");}
    }
}
