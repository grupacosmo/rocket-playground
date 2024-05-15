package pl.edu.pk.cosmo.rakieta.receiver;


import com.fazecast.jSerialComm.SerialPort;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

public class ReceiverImpl implements ReceiverMicr{
    static final int PACKET_SIZE = 68;
    public void getPort(int BaudRate) throws IOException {
        SerialPort[] ports = SerialPort.getCommPorts();
        System.out.println("Select a port:");
        int i = 1;
        for (SerialPort port : ports) {
            System.out.println(i++ + ": " + port.getSystemPortName());
        }
        java.util.Scanner s = new java.util.Scanner(System.in);
        int chosenPort = s.nextInt();

        SerialPort serialPort = ports[chosenPort - 1];
        serialPort.clearDTR();
        serialPort.clearRTS();
        serialPort.setBaudRate(BaudRate);
        if (serialPort.openPort()) {
            System.out.println("Port opened successfully.");
        } else {
            throw new IOException("Unable to open the port.");
        }
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 0, 0);
        java.io.InputStream in = serialPort.getInputStream();

        DataInputStream dataInputStream = new DataInputStream(in);
        readAsData(dataInputStream);
//    readAsHex(in);

//    while(true) {
//      byte[] buffer = new byte[4];
//      int readBytes = 0;
//      while (readBytes < 4) {
//        int b = in.read(buffer, readBytes, 4 - readBytes);
//        if (b == -1) {
//          System.out.println("End of stream");
//          return;
//        }
//        readBytes += b;
//      }
//      for (byte buffByte : buffer) {
//        int b = buffByte & 0xFF;
//        System.out.print(toHex(b));
//        System.out.print(" ");
//      }
//      System.out.println();
//    }
        serialPort.closePort();
    }

    public void readAsData(DataInputStream dis) {
        SensorPacket packet = new SensorPacket();
        packet.bme = new SensorPacketBME();
        packet.gps = new SensorPacketGPS();
        packet.mpu = new SensorPacketMPU();
        packet.mpu.acceleration = new float3();
        packet.mpu.rotation = new float3();
        packet.mpu.angularVelocity = new float3();
        try {
            while (true) {
                packet.time =  convert32endian(dis.readInt());
                packet.bme.temperature = dis.readFloat();
                packet.bme.humidity = dis.readFloat();
                packet.bme.pressure = dis.readFloat();
                packet.gps.latitude = dis.readFloat();
                packet.gps.longitude = dis.readFloat();
                packet.gps.altitude = dis.readFloat();
                packet.mpu.acceleration.x = dis.readFloat();
                packet.mpu.acceleration.y = dis.readFloat();
                packet.mpu.acceleration.z = dis.readFloat();
                packet.mpu.rotation.x = dis.readFloat();
                packet.mpu.rotation.y = dis.readFloat();
                packet.mpu.rotation.z = dis.readFloat();
                packet.mpu.angularVelocity.x = dis.readFloat();
                packet.mpu.angularVelocity.y = dis.readFloat();
                packet.mpu.angularVelocity.z = dis.readFloat();
                packet.relayFlags = convert32endian(dis.readInt());
                printSensorPacker(packet);
                System.out.println();
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    static void readAsHex(InputStream in) {

        try {
            byte[] buffer = new byte[PACKET_SIZE];
            while(true) {
                int readBytes = 0;
                while (readBytes < PACKET_SIZE) {
                    int b = in.read(buffer, readBytes, PACKET_SIZE - readBytes);
                    if (b == -1) {
                        System.out.println("End of stream");
                        return;
                    }
                    readBytes += b;
                }
                for (byte buffByte : buffer) {
                    int b = buffByte & 0xFF;
                    System.out.print(toHex(b));
                    System.out.print(" ");
                }
                System.out.println();
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    static String toHex(int b) {
        return String.format("%02X", b);
    }

    public static class float3 {
        public float x;
        public float y;
        public float z;
    }

    public static class SensorPacketBME {
        public float temperature;
        public float humidity;
        public float pressure;
    }

    public static class SensorPacketGPS {
        public float latitude;
        public float longitude;
        public float altitude;
    }

    public static class SensorPacketMPU {
        public float3 acceleration;
        public float3 rotation;
        public float3 angularVelocity;
    }

    public static class SensorPacket {
        public long time;
        public SensorPacketBME bme;
        public SensorPacketGPS gps;
        public SensorPacketMPU mpu;
        public int relayFlags;
    }

    public static void printSensorPacker(SensorPacket packet) {
        System.out.println("Sensor Packet:");
        System.out.println("  Time: " + packet.time);
        System.out.println("  Time: " + toBin(packet.time));
        System.out.println("  BME:");
        System.out.println("    Temperature: " + packet.bme.temperature);
        System.out.println("    Humidity: " + packet.bme.humidity);
        System.out.println("    Pressure: " + packet.bme.pressure);
        System.out.println("  GPS:");
        System.out.println("    Latitude: " + packet.gps.latitude);
        System.out.println("    Longitude: " + packet.gps.longitude);
        System.out.println("    Altitude: " + packet.gps.altitude);
        System.out.println("  MPU:");
        System.out.println("    Acceleration:");
        System.out.println("      x: " + packet.mpu.acceleration.x);
        System.out.println("      y: " + packet.mpu.acceleration.y);
        System.out.println("      z: " + packet.mpu.acceleration.z);
        System.out.println("    Rotation:");
        System.out.println("      x: " + packet.mpu.rotation.x);
        System.out.println("      y: " + packet.mpu.rotation.y);
        System.out.println("      z: " + packet.mpu.rotation.z);
        System.out.println("    Angular Velocity:");
        System.out.println("      x: " + packet.mpu.angularVelocity.x);
        System.out.println("      y: " + packet.mpu.angularVelocity.y);
        System.out.println("      z: " + packet.mpu.angularVelocity.z);
        System.out.println("  Relay Flags: " + packet.relayFlags);
    }
    public static String toBin(long v) {
        return Long.toBinaryString(v);
    }

    public static int convert32endian(int i) {
        return ((i >> 24) & 0xff) | // move byte 3 to byte 0
                ((i << 8) & 0xff0000) | // move byte 1 to byte 2
                ((i >> 8) & 0xff00) | // move byte 2 to byte 1
                ((i << 24) & 0xff000000); // byte 0 to byte 3
    }

}


