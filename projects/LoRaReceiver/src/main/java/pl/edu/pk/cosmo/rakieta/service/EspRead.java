package pl.edu.pk.cosmo.rakieta.service;

import com.fazecast.jSerialComm.SerialPort;
import pl.edu.pk.cosmo.rakieta.entity.*;

import java.io.DataInputStream;

public class EspRead {
    public SerialPort choosePort() {
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
        serialPort.setBaudRate(9600);
        if (serialPort.openPort()) {
            System.out.println("Port opened successfully.");
        } else {
            System.out.println("Unable to open the port.");
            return null;
        }
        return serialPort;
    }
    public SensorPacket readdata(SerialPort serialPort){
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 0, 0);
        java.io.InputStream in = serialPort.getInputStream();
        DataInputStream dataInputStream = new DataInputStream(in);
        return readAsData(dataInputStream);
    }
    static SensorPacket readAsData(DataInputStream dis) {
        try {
            SensorPacket packet = new SensorPacket();
            packet.bme = new SensorPacketBME();
            packet.gps = new SensorPacketGPS();
            packet.mpu = new SensorPacketMPU();
            packet.mpu.acceleration = new Float3();
            packet.mpu.rotation = new Float3();
            packet.mpu.angularVelocity = new Float3();
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
                return packet;
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
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
