package my.monitor;

import java.awt.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import java.io.IOException;
import java.net.Socket;

import java.awt.Graphics;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.nio.ByteBuffer;

public class monitorUI extends javax.swing.JFrame {

    static int VIDEO_UNKNOWN = 0;
    static int VIDEO_FREE = 1;
    static int VIDEO_ARENA_CALIBRATION = 2;
    static int VIDEO_ARENA_OK = 3;
    static int VIDEO_ARENA_KO = 4;
    static int VIDEO_WITH_POSITION = 5;
    static int ROBOT_UNKNOWN = -1;
    static int ROBOT_CONNEXION_FAILED = 0;
    static int ROBOT_CONNECTED = 2;
    static int ROBOT_CONNEXION_TRYING = 3;
    static int BAT_UNKNONW = -1;
    static int BAT_HIGH = 1;
    static int BAT_MED = 2;
    static int BAT_LOW = 3;
    static int COMMUNICATION_DISCONNECTED = 0;
    static int COMMUNICATION_CONNECTED = 1;
    static int COMMUNICATION_FAILURE = 2;
    static int COMMUNICATION_TRYING = 3;
    static int COMMUNICATION_CONNECTION_PROBLEMS = 5;
    static int ORDER_FIND_ARENA = 1;
    static int ORDER_ARENA_FAILED = 2;
    static int ORDER_ARENA_IS_FOUND = 3;
    static int ORDER_COMPUTE_CONTINUOUSLY_POSITION = 4;
    static int ORDER_CONNECT_ROBOT = 10;
    int video_status;
    int robot_status;
    int bat_status;
    int communication_status;
    private MonitorSend ms;
    private MonitorReceive mr;
    int port;
    boolean start = false;
    String address;
    String status2;
    Socket so;
    BufferedOutputStream ps;
    BufferedInputStream input;
    int direction = 0;
    int speed = 50;
    char type;
    Graphics g;
    Image imagem;
    ImageIcon imm = null;
    int length;
    byte[] size;

    public monitorUI() {
        ms = new MonitorSend(this);
        mr = new MonitorReceive(this);

        initComponents();
        initMonitor();

    }

    private void initMonitor() {
        initRobotCommunication();

        video_status = VIDEO_UNKNOWN;

        changeRobotStatus(ROBOT_UNKNOWN);
        changeBatLevel(BAT_UNKNONW);

        jRadioManual.setSelected(true);
        for (Component component : jPanelAutomatic.getComponents()) {
            component.setEnabled(false);
        }

        jPanelCamera.setVisible(false);
        jPanelRobotStatus.setVisible(false);
        jPanelControl.setVisible(false);

        mr.stopThread();
    }

    private void initRobotCommunication() {
        direction = 0;

        ms.stopThread();

        jButtonRight.setEnabled(true);
        jButtonDown.setEnabled(true);
        jButtonLeft.setEnabled(true);
        jButtonUp.setEnabled(true);
        jButtonStop.setEnabled(false);

        speed = 50;
        jSliderSpeed.setValue(speed);

    }

    public void changeRobotStatus(int newRobotStatus) {
        robot_status = newRobotStatus;

        if (newRobotStatus == ROBOT_UNKNOWN) {
            JTextRobotStatus.setText("");
            jPanelControl.setVisible(false);
            initRobotCommunication();
        } else if (newRobotStatus == ROBOT_CONNEXION_TRYING) {
            JTextRobotStatus.setText("Waiting connexion");
            jPanelControl.setVisible(false);
            JButtonConnectRobot.setEnabled(false);
            initRobotCommunication();
        } else if (newRobotStatus == ROBOT_CONNEXION_FAILED) {
            JTextRobotStatus.setText("Not connected");
            jPanelControl.setVisible(false);
            JButtonConnectRobot.setEnabled(true);
            initRobotCommunication();
        } else if (newRobotStatus == ROBOT_CONNECTED) {
            JTextRobotStatus.setText("Connected");
            jPanelControl.setVisible(true);
            JButtonConnectRobot.setEnabled(false);
            ms.start();
        }
    }

    private void changeVideoStatus(int newVideoStatus) {
        video_status = newVideoStatus;

        if (newVideoStatus == VIDEO_UNKNOWN) {
            jTextCoordinateX.setText("");
            jTextCoordinateY.setText("");
            jTextOrientation.setText("");
        }
    }

    private void changeBatLevel(int newBatLevel) {
        bat_status = newBatLevel;

        if (newBatLevel == BAT_UNKNONW) {
            jRadioButtonHighBat.setSelected(false);
            jRadioButtonLowBat.setSelected(false);
            jRadioButtonMedBat.setSelected(false);
        } else if (newBatLevel == BAT_HIGH) {
            jRadioButtonHighBat.setSelected(true);
        } else if (newBatLevel == BAT_MED) {
            jRadioButtonMedBat.setSelected(true);
        } else if (newBatLevel == BAT_LOW) {
            jRadioButtonLowBat.setSelected(true);
        }
    }

    public void changeCommunicationStatus(int newCommunicationStatus) {
        communication_status = newCommunicationStatus;

        if (newCommunicationStatus == COMMUNICATION_CONNECTED) {
            jTextStatusCon.setText("Connected");
            jTextStatusCon2.setText("");
            jTextIp.setEditable(false);
            jTextPort.setEditable(false);

            mr.start();

            jPanelCamera.setVisible(true);
            jPanelRobotStatus.setVisible(true);
        }
        if (newCommunicationStatus == COMMUNICATION_FAILURE) {
            jTextStatusCon.setText("Disconnected");
            jTextStatusCon2.setText("Server not found");
            initMonitor();
        }
        if (newCommunicationStatus == COMMUNICATION_TRYING) {
            jTextStatusCon2.setText("Trying...");
        }
        if (newCommunicationStatus == COMMUNICATION_DISCONNECTED) {
            jTextStatusCon.setText("Disconnected");
            jTextStatusCon2.setText("");
            jTextIp.setEditable(true);
            jTextPort.setEditable(true);
            initMonitor();
        }
        if (newCommunicationStatus == COMMUNICATION_CONNECTION_PROBLEMS) {
            jTextStatusCon.setText("Connection problems");
            jTextStatusCon2.setText("");
            jTextIp.setEditable(false);
            jTextPort.setEditable(false);
            initMonitor();
        }
    }

    public byte[] intToByte(int value) {
        byte[] b = new byte[4];
        b[0] = (byte) (value >> 24);
        b[1] = (byte) (value >> 16);
        b[2] = (byte) (value >> 8);
        b[3] = (byte) (value);
        return b;
    }

    public static void printByteArray(byte[] b, int nbData) {
        if (nbData > b.length) {
            nbData = b.length;
        }
        for (int i = 0; i < nbData; i++) {
            System.out.print(Integer.toHexString(0xFF & b[i]) + " ");
            if (((i + 1) % 25) == 0) {
                System.out.println();
            }
        }
        System.out.println();
    }

    public void sendData(byte[] data) {
        try {
            ps.write(data);
            ps.flush();
        } catch (IOException ex) {
            closeConnexion();
            Logger.getLogger(monitorUI.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public void sendAction(int action) {

        length = 4;
        byte[] data = new byte[5 + length];
        data[0] = 'A';
        System.arraycopy(intToByte(length), 0, data, 1, 4);
        System.arraycopy(intToByte(action), 0, data, 5, 4);
        sendData(data);
    }

    public void closeConnexion() {
        changeCommunicationStatus(COMMUNICATION_DISCONNECTED);
        try {
            so.close();
        } catch (IOException ex) {
            Logger.getLogger(monitorUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void show(Image m) {
        imm = new ImageIcon(m);
        jLabel1.setIcon(imm);

    }

    public void setMonitorSend(MonitorSend ms) {
        this.ms = ms;
    }

    public void setMonitorReceive(MonitorReceive mr) {
        this.mr = mr;
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        buttonGroup2 = new javax.swing.ButtonGroup();
        buttonGroup3 = new javax.swing.ButtonGroup();
        buttonGroup4 = new javax.swing.ButtonGroup();
        buttonGroup5 = new javax.swing.ButtonGroup();
        buttonGroup6 = new javax.swing.ButtonGroup();
        buttonGroup7 = new javax.swing.ButtonGroup();
        buttonGroup8 = new javax.swing.ButtonGroup();
        jPanel3 = new javax.swing.JPanel();
        jLabel11 = new javax.swing.JLabel();
        jButtonConnect = new javax.swing.JButton();
        jTextPort = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jTextIp = new javax.swing.JTextField();
        jTextStatusCon = new javax.swing.JTextField();
        jTextStatusCon2 = new javax.swing.JTextField();
        jPanelCamera = new javax.swing.JPanel();
        jLabel14 = new javax.swing.JLabel();
        jTextField7 = new javax.swing.JTextField();
        jLabel15 = new javax.swing.JLabel();
        jTextField8 = new javax.swing.JTextField();
        jLabel16 = new javax.swing.JLabel();
        jTextField9 = new javax.swing.JTextField();
        jLabel17 = new javax.swing.JLabel();
        jButtonStartDetectArena = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jRadioButton1 = new javax.swing.JRadioButton();
        jRadioButton2 = new javax.swing.JRadioButton();
        jButtonComputePosition = new javax.swing.JButton();
        jPanelControl = new javax.swing.JPanel();
        jPanel6 = new javax.swing.JPanel();
        jLabel19 = new javax.swing.JLabel();
        jRadioManual = new javax.swing.JRadioButton();
        jRadioAutomatic = new javax.swing.JRadioButton();
        jPanelManual = new javax.swing.JPanel();
        jLabel5 = new javax.swing.JLabel();
        jButtonUp = new javax.swing.JButton();
        jButtonDown = new javax.swing.JButton();
        jButtonLeft = new javax.swing.JButton();
        jButtonStop = new javax.swing.JButton();
        jButtonRight = new javax.swing.JButton();
        jLabel6 = new javax.swing.JLabel();
        jSliderSpeed = new javax.swing.JSlider();
        jLabelSpeed = new javax.swing.JLabel();
        jPanelAutomatic = new javax.swing.JPanel();
        jLabelAutomatic = new javax.swing.JLabel();
        jRadioCoordinate = new javax.swing.JRadioButton();
        jRadioObjective = new javax.swing.JRadioButton();
        jTextCoordinateX = new javax.swing.JTextField();
        jTextCoordinateY = new javax.swing.JTextField();
        jLabelY = new javax.swing.JLabel();
        jLabelX = new javax.swing.JLabel();
        jComboObjective = new javax.swing.JComboBox();
        jButtonExecute = new javax.swing.JButton();
        jTextOrientation = new javax.swing.JTextField();
        jLabelOrientation = new javax.swing.JLabel();
        jTextMissionStatus = new javax.swing.JTextField();
        jLabelMission = new javax.swing.JLabel();
        jPanelRobotStatus = new javax.swing.JPanel();
        JButtonConnectRobot = new javax.swing.JButton();
        jLabel4 = new javax.swing.JLabel();
        JTextRobotStatus = new javax.swing.JTextField();
        jRadioButtonHighBat = new javax.swing.JRadioButton();
        jRadioButtonMedBat = new javax.swing.JRadioButton();
        jRadioButtonLowBat = new javax.swing.JRadioButton();
        jLabel7 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);

        jLabel11.setFont(new java.awt.Font("Liberation Sans", 1, 14));
        jLabel11.setText("Connection");

        jButtonConnect.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jButtonConnect.setText("Connect/Disconnect");
        jButtonConnect.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonConnectMouseClicked(evt);
            }
        });

        jTextPort.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jTextPort.setText("9010");
        jTextPort.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextPortActionPerformed(evt);
            }
        });

        jLabel2.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel2.setText("IP address");

        jLabel3.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel3.setText("Port");

        jTextIp.setText("localhost");

        jTextStatusCon.setEditable(false);
        jTextStatusCon.setFont(new java.awt.Font("DejaVu Sans", 1, 12));
        jTextStatusCon.setText("Disconnected");

        jTextStatusCon2.setEditable(false);
        jTextStatusCon2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextStatusCon2ActionPerformed(evt);
            }
        });

        jLabel14.setFont(new java.awt.Font("Liberation Sans", 1, 14));
        jLabel14.setText("Current Position");

        jTextField7.setEditable(false);
        jTextField7.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jLabel15.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel15.setText("X");

        jTextField8.setEditable(false);
        jTextField8.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jLabel16.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel16.setText("Y");

        jTextField9.setEditable(false);
        jTextField9.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jTextField9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField9ActionPerformed(evt);
            }
        });

        jLabel17.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel17.setText("Angle");

        jButtonStartDetectArena.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jButtonStartDetectArena.setText("Detect Arena");
        jButtonStartDetectArena.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonStartDetectArenaMouseClicked(evt);
            }
        });
        jButtonStartDetectArena.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonStartDetectArenaActionPerformed(evt);
            }
        });

        jLabel1.setBackground(new java.awt.Color(255, 255, 255));
        jLabel1.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        jLabel1.setPreferredSize(new java.awt.Dimension(640, 480));
        jLabel1.setSize(new java.awt.Dimension(640, 480));
        jLabel1.addContainerListener(new java.awt.event.ContainerAdapter() {
            public void componentAdded(java.awt.event.ContainerEvent evt) {
                jLabel1ComponentAdded(evt);
            }
        });

        buttonGroup3.add(jRadioButton1);
        jRadioButton1.setText("The arena is ok");
        jRadioButton1.setEnabled(false);
        jRadioButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton1ActionPerformed(evt);
            }
        });

        buttonGroup3.add(jRadioButton2);
        jRadioButton2.setText("The arena is not ok");
        jRadioButton2.setEnabled(false);
        jRadioButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton2ActionPerformed(evt);
            }
        });

        jButtonComputePosition.setText("Continuous Detection of Position");
        jButtonComputePosition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonComputePositionActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanelCameraLayout = new javax.swing.GroupLayout(jPanelCamera);
        jPanelCamera.setLayout(jPanelCameraLayout);
        jPanelCameraLayout.setHorizontalGroup(
            jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelCameraLayout.createSequentialGroup()
                .addGap(26, 26, 26)
                .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addGroup(jPanelCameraLayout.createSequentialGroup()
                        .addComponent(jButtonComputePosition)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanelCameraLayout.createSequentialGroup()
                                .addComponent(jLabel15)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jTextField7, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel16)
                                .addGap(3, 3, 3)
                                .addComponent(jTextField8, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel17)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jTextField9, javax.swing.GroupLayout.PREFERRED_SIZE, 71, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(jLabel14)))
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(jPanelCameraLayout.createSequentialGroup()
                            .addComponent(jButtonStartDetectArena, javax.swing.GroupLayout.PREFERRED_SIZE, 172, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGap(18, 18, 18)
                            .addComponent(jRadioButton1)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                            .addComponent(jRadioButton2))
                        .addComponent(jLabel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 640, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanelCameraLayout.setVerticalGroup(
            jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelCameraLayout.createSequentialGroup()
                .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButtonStartDetectArena)
                    .addComponent(jRadioButton1)
                    .addComponent(jRadioButton2))
                .addGap(11, 11, 11)
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 480, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel14)
                    .addComponent(jButtonComputePosition))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jTextField9, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                        .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jTextField8, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel16)
                            .addComponent(jLabel17))
                        .addGroup(jPanelCameraLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jTextField7, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel15))))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jLabel19.setFont(new java.awt.Font("Liberation Sans", 1, 14));
        jLabel19.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel19.setText("Mode Selection");

        buttonGroup1.add(jRadioManual);
        jRadioManual.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jRadioManual.setText("Manual");
        jRadioManual.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jRadioManualMouseClicked(evt);
            }
        });

        buttonGroup1.add(jRadioAutomatic);
        jRadioAutomatic.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jRadioAutomatic.setText("Automatic");
        jRadioAutomatic.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jRadioAutomaticMouseClicked(evt);
            }
        });

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addGap(41, 41, 41)
                .addComponent(jRadioManual)
                .addGap(39, 39, 39)
                .addComponent(jRadioAutomatic)
                .addContainerGap(106, Short.MAX_VALUE))
            .addComponent(jLabel19, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 348, Short.MAX_VALUE)
        );
        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel19)
                .addGap(18, 18, 18)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jRadioManual)
                    .addComponent(jRadioAutomatic))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanelManual.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jLabel5.setFont(new java.awt.Font("Liberation Sans", 1, 12));
        jLabel5.setText("Manual Mode:");

        jButtonUp.setFont(new java.awt.Font("DejaVu Sans", 1, 14));
        jButtonUp.setText("↑");
        jButtonUp.setEnabled(false);
        jButtonUp.setFocusable(false);
        jButtonUp.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonUpMouseClicked(evt);
            }
        });
        jButtonUp.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonUpActionPerformed(evt);
            }
        });

        jButtonDown.setFont(new java.awt.Font("DejaVu Sans", 1, 14));
        jButtonDown.setText("↓");
        jButtonDown.setEnabled(false);
        jButtonDown.setFocusable(false);
        jButtonDown.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonDownActionPerformed(evt);
            }
        });

        jButtonLeft.setFont(new java.awt.Font("DejaVu Sans", 1, 14));
        jButtonLeft.setText("←");
        jButtonLeft.setEnabled(false);
        jButtonLeft.setFocusable(false);
        jButtonLeft.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonLeftActionPerformed(evt);
            }
        });

        jButtonStop.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jButtonStop.setText("STOP");
        jButtonStop.setEnabled(false);
        jButtonStop.setFocusable(false);
        jButtonStop.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonStopActionPerformed(evt);
            }
        });

        jButtonRight.setFont(new java.awt.Font("DejaVu Sans", 1, 14));
        jButtonRight.setText("→");
        jButtonRight.setEnabled(false);
        jButtonRight.setFocusable(false);
        jButtonRight.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonRightActionPerformed(evt);
            }
        });

        jLabel6.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabel6.setText("Robot Speed (%)");

        jSliderSpeed.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jSliderSpeedMousePressed(evt);
            }
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                jSliderSpeedMouseReleased(evt);
            }
        });
        jSliderSpeed.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                jSliderSpeedMouseMoved(evt);
            }
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                jSliderSpeedMouseDragged(evt);
            }
        });

        jLabelSpeed.setText("50");

        javax.swing.GroupLayout jPanelManualLayout = new javax.swing.GroupLayout(jPanelManual);
        jPanelManual.setLayout(jPanelManualLayout);
        jPanelManualLayout.setHorizontalGroup(
            jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelManualLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanelManualLayout.createSequentialGroup()
                        .addGap(79, 79, 79)
                        .addGroup(jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.CENTER)
                            .addGroup(jPanelManualLayout.createSequentialGroup()
                                .addComponent(jButtonLeft)
                                .addGap(6, 6, 6)
                                .addComponent(jButtonStop)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButtonRight))
                            .addComponent(jButtonUp)
                            .addComponent(jButtonDown)))
                    .addComponent(jLabel5)
                    .addGroup(jPanelManualLayout.createSequentialGroup()
                        .addComponent(jLabel6)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 184, Short.MAX_VALUE)
                        .addComponent(jLabelSpeed, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jSliderSpeed, javax.swing.GroupLayout.PREFERRED_SIZE, 261, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        jPanelManualLayout.setVerticalGroup(
            jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelManualLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel5)
                .addGap(12, 12, 12)
                .addComponent(jButtonUp)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButtonLeft)
                    .addComponent(jButtonStop)
                    .addComponent(jButtonRight))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButtonDown)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 10, Short.MAX_VALUE)
                .addGroup(jPanelManualLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel6)
                    .addComponent(jLabelSpeed))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jSliderSpeed, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18))
        );

        jPanelAutomatic.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jLabelAutomatic.setFont(new java.awt.Font("Liberation Sans", 1, 12));
        jLabelAutomatic.setText("Automatic Mode:");

        buttonGroup2.add(jRadioCoordinate);
        jRadioCoordinate.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jRadioCoordinate.setText("Reach coordinate");
        jRadioCoordinate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioCoordinateActionPerformed(evt);
            }
        });

        buttonGroup2.add(jRadioObjective);
        jRadioObjective.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jRadioObjective.setText("Reach objective");

        jTextCoordinateX.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jTextCoordinateY.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jLabelY.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabelY.setText("Y:");

        jLabelX.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabelX.setText("X:");

        jComboObjective.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jComboObjective.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Green area", "Red area", "Get the ball" }));

        jButtonExecute.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jButtonExecute.setText("Execute");

        jTextOrientation.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jLabelOrientation.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabelOrientation.setText("Orientation:");

        jTextMissionStatus.setEditable(false);
        jTextMissionStatus.setFont(new java.awt.Font("Liberation Sans", 0, 12));

        jLabelMission.setFont(new java.awt.Font("Liberation Sans", 0, 12));
        jLabelMission.setText("Mission status");

        javax.swing.GroupLayout jPanelAutomaticLayout = new javax.swing.GroupLayout(jPanelAutomatic);
        jPanelAutomatic.setLayout(jPanelAutomaticLayout);
        jPanelAutomaticLayout.setHorizontalGroup(
            jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanelAutomaticLayout.createSequentialGroup()
                        .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                                .addComponent(jLabelOrientation)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jTextOrientation, javax.swing.GroupLayout.PREFERRED_SIZE, 68, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                                .addComponent(jRadioCoordinate)
                                .addGap(39, 39, 39)
                                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(jLabelY)
                                    .addComponent(jLabelX))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                    .addComponent(jTextCoordinateX, 0, 0, Short.MAX_VALUE)
                                    .addComponent(jTextCoordinateY, javax.swing.GroupLayout.PREFERRED_SIZE, 68, javax.swing.GroupLayout.PREFERRED_SIZE))))
                        .addGap(117, 117, 117))
                    .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                        .addComponent(jLabelAutomatic)
                        .addContainerGap(229, Short.MAX_VALUE))
                    .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                        .addComponent(jRadioObjective)
                        .addGap(18, 18, 18)
                        .addComponent(jComboObjective, 0, 118, Short.MAX_VALUE)
                        .addGap(117, 117, 117))
                    .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                        .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanelAutomaticLayout.createSequentialGroup()
                                .addComponent(jTextMissionStatus, javax.swing.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)
                                .addGap(18, 18, 18)
                                .addComponent(jButtonExecute))
                            .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                                .addComponent(jLabelMission)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 179, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(117, 117, 117))))
        );
        jPanelAutomaticLayout.setVerticalGroup(
            jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                .addComponent(jLabelAutomatic)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                        .addGap(50, 50, 50)
                        .addComponent(jRadioCoordinate))
                    .addGroup(jPanelAutomaticLayout.createSequentialGroup()
                        .addGap(27, 27, 27)
                        .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jTextCoordinateX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabelX))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jTextCoordinateY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabelY))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jTextOrientation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabelOrientation))
                .addGap(18, 18, 18)
                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jRadioObjective)
                    .addComponent(jComboObjective, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(jLabelMission)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanelAutomaticLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButtonExecute)
                    .addComponent(jTextMissionStatus, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(20, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanelControlLayout = new javax.swing.GroupLayout(jPanelControl);
        jPanelControl.setLayout(jPanelControlLayout);
        jPanelControlLayout.setHorizontalGroup(
            jPanelControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelControlLayout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addGroup(jPanelControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jPanel6, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanelManual, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanelAutomatic, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 348, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanelControlLayout.setVerticalGroup(
            jPanelControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelControlLayout.createSequentialGroup()
                .addComponent(jPanel6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanelManual, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanelAutomatic, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(20, 20, 20))
        );

        JButtonConnectRobot.setText("Connect Robot");
        JButtonConnectRobot.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                JButtonConnectRobotActionPerformed(evt);
            }
        });

        jLabel4.setText("Robot status");

        buttonGroup8.add(jRadioButtonHighBat);
        jRadioButtonHighBat.setEnabled(false);

        buttonGroup8.add(jRadioButtonMedBat);
        jRadioButtonMedBat.setEnabled(false);

        buttonGroup8.add(jRadioButtonLowBat);
        jRadioButtonLowBat.setEnabled(false);

        jLabel7.setText("Battery level");

        javax.swing.GroupLayout jPanelRobotStatusLayout = new javax.swing.GroupLayout(jPanelRobotStatus);
        jPanelRobotStatus.setLayout(jPanelRobotStatusLayout);
        jPanelRobotStatusLayout.setHorizontalGroup(
            jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelRobotStatusLayout.createSequentialGroup()
                .addGroup(jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addGroup(jPanelRobotStatusLayout.createSequentialGroup()
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jLabel7))
                    .addComponent(JButtonConnectRobot, javax.swing.GroupLayout.Alignment.LEADING))
                .addGroup(jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanelRobotStatusLayout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(jLabel4)
                        .addGap(18, 18, 18)
                        .addComponent(JTextRobotStatus, javax.swing.GroupLayout.DEFAULT_SIZE, 216, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanelRobotStatusLayout.createSequentialGroup()
                        .addGap(104, 104, 104)
                        .addComponent(jRadioButtonHighBat)
                        .addGap(21, 21, 21)
                        .addComponent(jRadioButtonMedBat)
                        .addGap(18, 18, 18)
                        .addComponent(jRadioButtonLowBat)
                        .addGap(108, 108, 108))))
        );
        jPanelRobotStatusLayout.setVerticalGroup(
            jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanelRobotStatusLayout.createSequentialGroup()
                .addGroup(jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(JButtonConnectRobot)
                    .addComponent(jLabel4)
                    .addComponent(JTextRobotStatus, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(jPanelRobotStatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jRadioButtonHighBat)
                        .addComponent(jRadioButtonMedBat)
                        .addComponent(jRadioButtonLowBat))
                    .addComponent(jLabel7))
                .addContainerGap(6, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGap(6, 6, 6)
                        .addComponent(jPanelCamera, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(14, 14, 14)
                        .addComponent(jPanelControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addGap(18, 18, 18)
                                .addComponent(jTextIp, javax.swing.GroupLayout.PREFERRED_SIZE, 115, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel3)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jTextPort, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel11)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jButtonConnect)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jTextStatusCon)
                            .addComponent(jTextStatusCon2, javax.swing.GroupLayout.PREFERRED_SIZE, 208, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jPanelRobotStatus, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                        .addGroup(jPanel3Layout.createSequentialGroup()
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(jLabel11)
                                .addComponent(jButtonConnect))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(jLabel2)
                                .addComponent(jTextIp, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jTextPort, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jLabel3)))
                        .addGroup(jPanel3Layout.createSequentialGroup()
                            .addComponent(jTextStatusCon, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(jTextStatusCon2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addComponent(jPanelRobotStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 67, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanelCamera, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jPanelControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(58, 58, 58))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(55, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, 669, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    public int getPort() {
        port = Integer.parseInt(jTextPort.getText());
        return port;
    }

    public String getAddress() {
        address = jTextIp.getText();
        return address;
    }

    public int getDirection() {
        return direction;
    }

    public int getSpeed() {
        return speed;
    }

    public String getType() {
        if (jRadioManual.isSelected()) {
            return "M";
        } else {
            return "A";
        }
    }

    public byte[] getData() {
        if (jRadioManual.isSelected()) {
            length = 2;
            type = 'M';
        }

        byte[] data = new byte[5 + length];
        data[0] = (byte) type;
        System.arraycopy(intToByte(length), 0, data, 1, 4);
        System.arraycopy(intToByte(speed), 3, data, 6, 1);
        System.arraycopy(intToByte(direction), 3, data, 5, 1);

        return data;
    }

    public void setPosition(byte[] b) {
        ByteBuffer buf = ByteBuffer.wrap(b);
        jTextField7.setText(Integer.toString(buf.getInt()));
        jTextField8.setText(Integer.toString(buf.getInt()));
        jTextField9.setText(Integer.toString(buf.getInt()));
    }

    private void jRadioManualMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jRadioManualMouseClicked

        for (Component component : jPanelAutomatic.getComponents()) {
            component.setEnabled(false);
        }

        for (Component component : jPanelManual.getComponents()) {
            component.setEnabled(true);
        }

}//GEN-LAST:event_jRadioManualMouseClicked

    private void jButtonUpActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonUpActionPerformed
        if (ms.running) {
            direction = 1;
            jButtonRight.setEnabled(true);
            jButtonDown.setEnabled(true);
            jButtonLeft.setEnabled(true);
            jButtonUp.setEnabled(false);
            jButtonStop.setEnabled(true);
        }

}//GEN-LAST:event_jButtonUpActionPerformed

    private void jButtonDownActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonDownActionPerformed
        if (ms.running) {
            direction = 4;
            jButtonRight.setEnabled(true);
            jButtonDown.setEnabled(false);
            jButtonLeft.setEnabled(true);
            jButtonUp.setEnabled(true);
            jButtonStop.setEnabled(true);
        }
}//GEN-LAST:event_jButtonDownActionPerformed

    private void jRadioCoordinateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioCoordinateActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_jRadioCoordinateActionPerformed

    private void jTextField9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextField9ActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_jTextField9ActionPerformed

    private void jRadioAutomaticMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jRadioAutomaticMouseClicked
        for (Component component : jPanelAutomatic.getComponents()) {
            component.setEnabled(true);
        }

        for (Component component : jPanelManual.getComponents()) {
            component.setEnabled(false);
        }

        direction = 0;
    }//GEN-LAST:event_jRadioAutomaticMouseClicked

    private void jButtonConnectMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonConnectMouseClicked

        if (communication_status != COMMUNICATION_CONNECTED) {
            if (jTextPort.getText().equals("") || jTextIp.getText().equals("")) {
                jTextStatusCon2.setText("Missing information");
            } else {
                changeCommunicationStatus(COMMUNICATION_TRYING);
                int Port = getPort();
                String Address = getAddress();

                try {
                    so = new Socket(Address, Port);
                    ps = new BufferedOutputStream(this.so.getOutputStream());
                    input = new BufferedInputStream(this.so.getInputStream());

                    changeCommunicationStatus(COMMUNICATION_CONNECTED);

                } catch (IOException e) {
                    changeCommunicationStatus(COMMUNICATION_FAILURE);
                }
            }
        } else {
            closeConnexion();
        }
    }//GEN-LAST:event_jButtonConnectMouseClicked

    private void jButtonUpMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonUpMouseClicked
    }//GEN-LAST:event_jButtonUpMouseClicked

    private void jTextStatusCon2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextStatusCon2ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jTextStatusCon2ActionPerformed

    private void jButtonRightActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonRightActionPerformed
        if (ms.running) {
            direction = 2;
            jButtonRight.setEnabled(false);
            jButtonDown.setEnabled(true);
            jButtonLeft.setEnabled(true);
            jButtonUp.setEnabled(true);
            jButtonStop.setEnabled(true);
        }
    }//GEN-LAST:event_jButtonRightActionPerformed

    private void jButtonLeftActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonLeftActionPerformed
        if (ms.running) {
            direction = 3;
            jButtonRight.setEnabled(true);
            jButtonDown.setEnabled(true);
            jButtonLeft.setEnabled(false);
            jButtonUp.setEnabled(true);
            jButtonStop.setEnabled(true);
        }
    }//GEN-LAST:event_jButtonLeftActionPerformed

    private void jSliderSpeedMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jSliderSpeedMouseReleased
        speed = jSliderSpeed.getValue();
    }//GEN-LAST:event_jSliderSpeedMouseReleased

    private void jButtonStopActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonStopActionPerformed
        direction = 0;

        jButtonRight.setEnabled(true);
        jButtonDown.setEnabled(true);
        jButtonLeft.setEnabled(true);
        jButtonUp.setEnabled(true);
        jButtonStop.setEnabled(false);
}//GEN-LAST:event_jButtonStopActionPerformed

    private void jSliderSpeedMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jSliderSpeedMousePressed
    }//GEN-LAST:event_jSliderSpeedMousePressed

    private void jSliderSpeedMouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jSliderSpeedMouseMoved
    }//GEN-LAST:event_jSliderSpeedMouseMoved

    private void jSliderSpeedMouseDragged(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jSliderSpeedMouseDragged
        jLabelSpeed.setText(Integer.toString(jSliderSpeed.getValue()));
    }//GEN-LAST:event_jSliderSpeedMouseDragged

    private void jLabel1ComponentAdded(java.awt.event.ContainerEvent evt) {//GEN-FIRST:event_jLabel1ComponentAdded
        // TODO add your handling code here:
    }//GEN-LAST:event_jLabel1ComponentAdded

    private void jButtonStartDetectArenaActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonStartDetectArenaActionPerformed
        int test = 1;
        //Custom button text OUI:0 NON:1 CANCEL:2
        while (test == 1) {
            sendAction(ORDER_FIND_ARENA);
            test = JOptionPane.showConfirmDialog(null,
                    "Est-ce que l'arène est délimitée par le carré vert ?",
                    "Calibration",
                    JOptionPane.YES_NO_CANCEL_OPTION);
        }

        if (test == 2) {
            sendAction(ORDER_ARENA_FAILED);
        } else if (test == 0) {
            sendAction(ORDER_ARENA_IS_FOUND);
        }

        //}
    }//GEN-LAST:event_jButtonStartDetectArenaActionPerformed

    private void jButtonStartDetectArenaMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonStartDetectArenaMouseClicked
    }//GEN-LAST:event_jButtonStartDetectArenaMouseClicked

    private void jTextPortActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextPortActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jTextPortActionPerformed

    private void jRadioButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton1ActionPerformed
        // TODO add your handling code here:
        jButtonStartDetectArena.setEnabled(true);
    }//GEN-LAST:event_jRadioButton1ActionPerformed

    private void jRadioButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton2ActionPerformed
        // TODO add your handling code here:
        jButtonStartDetectArena.setEnabled(true);
    }//GEN-LAST:event_jRadioButton2ActionPerformed

    private void JButtonConnectRobotActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_JButtonConnectRobotActionPerformed
        sendAction(ORDER_CONNECT_ROBOT);
        changeRobotStatus(ROBOT_CONNEXION_TRYING);
    }//GEN-LAST:event_JButtonConnectRobotActionPerformed

    private void jButtonComputePositionActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonComputePositionActionPerformed
        sendAction(ORDER_COMPUTE_CONTINUOUSLY_POSITION);
    }//GEN-LAST:event_jButtonComputePositionActionPerformed

    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                monitorUI monitor = new monitorUI();
                monitor.setVisible(true);
                MonitorSend monitorS = new MonitorSend(monitor);
                MonitorReceive monitorR = new MonitorReceive(monitor);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton JButtonConnectRobot;
    private javax.swing.JTextField JTextRobotStatus;
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.ButtonGroup buttonGroup2;
    private javax.swing.ButtonGroup buttonGroup3;
    private javax.swing.ButtonGroup buttonGroup4;
    private javax.swing.ButtonGroup buttonGroup5;
    private javax.swing.ButtonGroup buttonGroup6;
    private javax.swing.ButtonGroup buttonGroup7;
    private javax.swing.ButtonGroup buttonGroup8;
    private javax.swing.JButton jButtonComputePosition;
    private javax.swing.JButton jButtonConnect;
    private javax.swing.JButton jButtonDown;
    private javax.swing.JButton jButtonExecute;
    private javax.swing.JButton jButtonLeft;
    private javax.swing.JButton jButtonRight;
    private javax.swing.JButton jButtonStartDetectArena;
    private javax.swing.JButton jButtonStop;
    private javax.swing.JButton jButtonUp;
    private javax.swing.JComboBox jComboObjective;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel17;
    private javax.swing.JLabel jLabel19;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabelAutomatic;
    private javax.swing.JLabel jLabelMission;
    private javax.swing.JLabel jLabelOrientation;
    private javax.swing.JLabel jLabelSpeed;
    private javax.swing.JLabel jLabelX;
    private javax.swing.JLabel jLabelY;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanelAutomatic;
    private javax.swing.JPanel jPanelCamera;
    private javax.swing.JPanel jPanelControl;
    private javax.swing.JPanel jPanelManual;
    private javax.swing.JPanel jPanelRobotStatus;
    private javax.swing.JRadioButton jRadioAutomatic;
    private javax.swing.JRadioButton jRadioButton1;
    private javax.swing.JRadioButton jRadioButton2;
    private javax.swing.JRadioButton jRadioButtonHighBat;
    private javax.swing.JRadioButton jRadioButtonLowBat;
    private javax.swing.JRadioButton jRadioButtonMedBat;
    private javax.swing.JRadioButton jRadioCoordinate;
    private javax.swing.JRadioButton jRadioManual;
    private javax.swing.JRadioButton jRadioObjective;
    private javax.swing.JSlider jSliderSpeed;
    private javax.swing.JTextField jTextCoordinateX;
    private javax.swing.JTextField jTextCoordinateY;
    private javax.swing.JTextField jTextField7;
    private javax.swing.JTextField jTextField8;
    private javax.swing.JTextField jTextField9;
    private javax.swing.JTextField jTextIp;
    private javax.swing.JTextField jTextMissionStatus;
    private javax.swing.JTextField jTextOrientation;
    private javax.swing.JTextField jTextPort;
    private javax.swing.JTextField jTextStatusCon;
    private javax.swing.JTextField jTextStatusCon2;
    // End of variables declaration//GEN-END:variables
}
