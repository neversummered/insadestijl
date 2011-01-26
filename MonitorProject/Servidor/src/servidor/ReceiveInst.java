
package servidor;

import java.io.*;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class ReceiveInst extends javax.swing.JFrame {

    private sendImage si;
    ServerSocket serv=null;
    Socket s= null;
    BufferedInputStream in;
    String numbytes;
    byte[] numbytes1;
    byte[] head;
    byte[] tmp;
    char type;
    int size;
    BufferedOutputStream output;
    boolean start=false;


    public ReceiveInst() {
        initComponents();
    }
    
    public void setSendImage(sendImage si){
        this.si = si;
    }

    public int getInt(byte[] b){

         int i =  ((b[1]& 0xFF) << 24 );
            i += ((b[2]& 0xFF) << 16 );
            i += ((b[3]& 0xFF) << 8 );
            i += ( b[4]& 0xFF);
            return i;
    }



    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButtonStart = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jButtonStart.setText("Start");
        jButtonStart.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonStartMouseClicked(evt);
            }
        });
        jButtonStart.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonStartActionPerformed(evt);
            }
        });

        jTextArea1.setColumns(20);
        jTextArea1.setRows(5);
        jScrollPane1.setViewportView(jTextArea1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 400, Short.MAX_VALUE)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jButtonStart))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 256, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButtonStart)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButtonStartActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonStartActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jButtonStartActionPerformed

    private void jButtonStartMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonStartMouseClicked
        while(true){
        try{
                    while(true){

			serv = new ServerSocket(8000);
			System.out.println("Waiting client");
                        s = serv.accept();
                        output = new BufferedOutputStream(this.s.getOutputStream());
                        in = new BufferedInputStream(this.s.getInputStream());

                        if(start==false)
                        {
                            si.start();
                            start = true;
                        }
                        else{
                            si.running=true;
                        }



                        while(true){

                                head = new byte[5];
                                
                                in.read(head, 0, 5);
                                
                                type = (char)head[0];
                                
                                size = getInt(head);

                                if(size==0){
                                    break;
                                }
                                
                                tmp = new byte[size];
                                in.read(tmp,0,size);
                                System.out.println(tmp[0]+" "+tmp[1]);

                               
                        }
                        
                        try{
                            si.running=false;
                            s.close();
                            serv.close();
                        }catch(IOException e){System.out.println("error closing");}
                        System.out.println("Connection closed...");

                     }


		}catch(IOException e){
			System.out.println("Some problem trying to create the socket connection.");}
        }
    }//GEN-LAST:event_jButtonStartMouseClicked

    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                ReceiveInst receive = new ReceiveInst();
                receive.setVisible(true);
                sendImage send = new sendImage(receive);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButtonStart;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTextArea jTextArea1;
    // End of variables declaration//GEN-END:variables

}
