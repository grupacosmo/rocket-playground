package pl.edu.pk.cosmo.rakieta.service;

import com.fasterxml.jackson.dataformat.csv.CsvMapper;
import com.fasterxml.jackson.dataformat.csv.CsvSchema;
import com.fazecast.jSerialComm.SerialPort;
import pl.edu.pk.cosmo.rakieta.LoRaException;
import pl.edu.pk.cosmo.rakieta.entity.*;

import java.io.*;

public class EspRead implements Closeable {
    public static final int LORA_BOUND_RATE = 9600;
    private SerialPort serialPort;
    public BufferedReader serialInput;
    public OutputStreamWriter serialOutput;
    private CsvMapper csvMapper;
    private CsvSchema schema;
    public EspRead() {
        schema = new CsvSchema.Builder()
                .addColumn("time")
                .addColumn("temperature")
                .addColumn("humidity")
                .addColumn("pressure")
                .addColumn("latitude")
                .addColumn("longitude")
                .addColumn("altitude")
                .addColumn("accelerationX")
                .addColumn("accelerationY")
                .addColumn("accelerationZ")
                .addColumn("rotationX")
                .addColumn("rotationY")
                .addColumn("rotationZ")
                .addColumn("angularVelocityX")
                .addColumn("angularVelocityY")
                .addColumn("angularVelocityZ")
                .addColumn("relayFlags")
                .build();
        csvMapper = new CsvMapper();
    }

    public void choosePort() throws IOException {
        SerialPort[] ports = SerialPort.getCommPorts();
        System.out.println("Select a port:");
        int i = 1;
        for (SerialPort port : ports) {
            System.out.println(i++ + ": " + port.getSystemPortName());
        }
        java.util.Scanner s = new java.util.Scanner(System.in);
        int chosenPort = s.nextInt();

        serialPort = ports[chosenPort - 1];
        setupPort(serialPort);
        java.io.InputStream in = serialPort.getInputStream();
        serialInput = new BufferedReader(new InputStreamReader(in));
        serialOutput = new OutputStreamWriter(serialPort.getOutputStream());
        setupLora();
    }

    private void setupLora() throws IOException {
        String msg = "LORA SETUP";
        System.out.println("lora setup");
        System.out.println("  Test mode");
        serialOutput.write("AT+MODE=TEST\r\n");
        serialOutput.flush();
        waitForOK("+MODE: TEST", msg);

        System.out.println("  RFCFG");
        serialOutput.write("AT+TEST=RFCFG, 868, SF12, 125, 12, 15, 14, ON, OFF, OFF\r\n");
        serialOutput.flush();
        waitForOK("+TEST", msg);

        System.out.println("   RXLPRKT");
        serialOutput.write("AT+TEST=RXLRPKT\r\n");
        serialOutput.flush();
        waitForOK("+TEST", msg);
    }

    private void waitForOK(String expectedResponse, String info) throws IOException {
        String message = serialInput.readLine();
//        int b = serialInput.read();
        System.out.println("[DEBUG][LoraResponse] " + message);
        if (!message.contains(expectedResponse)) {
            throw new LoRaException("Not recived '" + expectedResponse + "' for " + info );
        }
    }


    private static void setupPort(SerialPort serialPort) {
        serialPort.clearDTR();
        serialPort.clearRTS();
        serialPort.setNumDataBits(8);
        serialPort.setNumStopBits(SerialPort.ONE_STOP_BIT);
        serialPort.setParity(SerialPort.NO_PARITY);
        serialPort.setBaudRate(LORA_BOUND_RATE);
        if (serialPort.openPort()) {
            System.out.println("Port opened successfully.");
        } else {
            System.err.println("Unable to open the port.");
            throw new RuntimeException("Unable to open the port.");
        }

        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
    }

    public SensorPacket readdata() throws IOException {
        String line = serialInput.readLine();
        if(line == null){
            return null;
        }
        System.out.println("READ: " + line);
        if(line.startsWith("+TEST: RX")) {
            String parsedData = parseRXLine(line);
            System.out.println("READ PARSED: " + parsedData);
            parsedData.indexOf("time");
            return readAsData(parsedData);
        }
        return null;
    }

    private static String parseRXLine(String line) {
        String hexData = line.split("\"")[1];
        byte[] data = new byte[hexData.length()/2];
        for(int i = 0 ; i < hexData.length() ; i+=2){
            data[i/2] = Byte.parseByte(hexData.substring(i, i+2), 16);
        }
        String parsedData = new String(data);
        return parsedData;
    }

    public void close() throws IOException {
        serialInput.close();
    }
    SensorPacket readAsData(String split) {
        SensorPacket sensorPacket = new SensorPacket();
        SensorPacketBME sensorPacketBME = new SensorPacketBME();
        SensorPacketGPS sensorPacketGPS = new SensorPacketGPS();
        SensorPacketMPU sensorPacketMPU = new SensorPacketMPU();
        try {
            CsvSensorPacket s1 = csvMapper.readerFor(CsvSensorPacket.class)
                    .with(schema)
                    .readValue(split, CsvSensorPacket.class);
            sensorPacket.setTime(s1.getTime());
            sensorPacket.setRelayFlags(s1.getRelayFlags());

            sensorPacketBME.setTemperature(s1.getTemperature());
            sensorPacketBME.setHumidity(s1.getHumidity());
            sensorPacketBME.setPressure(s1.getPressure());

            sensorPacketGPS.setAltitude(s1.getAltitude());
            sensorPacketGPS.setLongitude(s1.getLongitude());
            sensorPacketGPS.setLatitude(s1.getLatitude());

            sensorPacketMPU.setAcceleration(new Float3(s1.getAccelerationX(), s1.getAccelerationY(), s1.getAccelerationZ()));
            sensorPacketMPU.setRotation(new Float3(s1.getRotationX(), s1.getRotationY(), s1.getRotationZ()));
            sensorPacketMPU.setAngularVelocity(new Float3(s1.getAngularVelocityX(), s1.getAngularVelocityY(), s1.getAngularVelocityZ()));

            sensorPacket.setBme(sensorPacketBME);
            sensorPacket.setMpu(sensorPacketMPU);
            sensorPacket.setGps(sensorPacketGPS);
            return sensorPacket;
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
